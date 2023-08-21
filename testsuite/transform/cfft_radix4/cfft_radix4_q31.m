clear;
clc;
m= 3;
datasize = 2*64;
fftlen = 64; % n = 4^m;
%-----------------------------------%
fid = fopen('input_q31.bin', 'rb');
ofid = fopen('golden_cfft_radix4_q31.bin', 'wb');
%-----------------------------------%
src = fread(fid, datasize, 'int');
p = 2*pi/fftlen;
n2 = fftlen;
%-----------------------------------%
n1 = n2;
n2 = n2 /4;
i0=1;
ia1 = 0;
j = n2;
ie = 1;
while(1)
  i1 = i0 + n2;
  i2 = i1 + n2;
  i3 = i2 + n2;
  r1 = src(2*i0-1)/16 + src(2*i2-1)/16;
  r2 = src(2*i0-1)/16 - src(2*i2-1)/16;
  t1 = src(2*i1-1)/16 + src(2*i3-1)/16;
  s1 = src(2*i0)/16 + src(2*i2)/16;
  s2 = src(2*i0)/16 - src(2*i2)/16;
  src(2*i0-1) = r1 + t1;
  r1 = r1 - t1;
  t2 = src(2*i1)/16 + src(2*i3)/16;
  src(2*i0) = s1 + t2;
  s1 = s1 - t2;
  t1 = src(2*i1)/16 - src(2*i3)/16;
  t2 = src(2*i1-1)/16 - src(2*i3-1)/16;
  ia2 = 2*ia1;
  co2 = q2int_q31(cos(ia2*p),1);
  si2 = q2int_q31(sin(ia2*p),1);
  src(2*i1-1) = ((int32)(bitshift(((int64)(r1*co2)),-32)) + (int32)(bitshift(((int64)(s1*si2)),-32)))*2;
  src(2*i1) = ((int32)(bitshift(((int64)(s1*co2)),-32)) - (int32)(bitshift(((int64)(r1*si2)),-32)))*2;
  r1 = r2 + t1;
  r2 = r2 - t1;
  s1 = s2 - t2;
  s2 = s2 + t2;
  co1 = q2int_q31(cos(ia1*p),1);
  si1 = q2int_q31(sin(ia1*p),1);
  src(2*i2-1) = ((int32)(bitshift(((int64)(r1*co1)),-32)) + (int32)(bitshift(((int64)(s1*si1)),-32)))*2;
  src(2*i2) = ((int32)(bitshift(((int64)(s1*co1)),-32)) - (int32)(bitshift(((int64)(r1*si1)),-32)))*2;
  ia3 = 3*ia1;
  co3 = q2int_q31(cos(ia3*p),1);
  si3 = q2int_q31(sin(ia3*p),1);
  src(2*i3-1) = ((int32)(bitshift(((int64)(r2*co3)),-32)) + (int32)(bitshift(((int64)(s2*si3)),-32)))*2;
  src(2*i3) = ((int32)(bitshift(((int64)(s2*co3)),-32)) - (int32)(bitshift(((int64)(r2*si3)),-32)))*2;
  ia1 = ia1 + ie;
  i0 = i0+1;
  j=j-1;
  if(j==0)
	break;
  end
end
ie = ie * 4;
%------------------------------------%
k=fftlen/4;
while(k>4)
  n1 = n2;
  n2 = n2 /4;
  ia1 = 0;
  for jj = 1 : n2
	ia2 = ia1 + ia1;
	ia3 = ia2 + ia1;
	co1 = q2int_q31(cos(ia1*p),1);
	si1 = q2int_q31(sin(ia1*p),1);
	co2 = q2int_q31(cos(ia2*p),1);
	si2 = q2int_q31(sin(ia2*p),1);
	co3 = q2int_q31(cos(ia3*p),1);
	si3 = q2int_q31(sin(ia3*p),1);
	ia1 = ia1 + ie;
	for i0 = jj : n1: fftlen
	  i1 = i0 + n2;
	  i2 = i1 + n2;
	  i3 = i2 + n2;
      r1 = src(2*i0-1) + src(2*i2-1);
      r2 = src(2*i0-1) - src(2*i2-1);
      t1 = src(2*i1-1) + src(2*i3-1);
      s1 = src(2*i0) + src(2*i2);
      s2 = src(2*i0) - src(2*i2);
      src(2*i0-1) = (r1 + t1)/4;
      r1 = r1 - t1;
      t2 = src(2*i1) + src(2*i3);
      src(2*i0) = (s1 + t2)/4;
      s1 = s1 - t2;
      t1 = src(2*i1) - src(2*i3);
      t2 = src(2*i1-1) - src(2*i3-1);
      src(2*i1-1) = ((int32)(bitshift(((int64)(r1*co2)),-32)) + (int32)(bitshift(((int64)(s1*si2)),-32)))/2;
      src(2*i1) = ((int32)(bitshift(((int64)(s1*co2)),-32)) - (int32)(bitshift(((int64)(r1*si2)),-32)))/2;
      r1 = r2 + t1;
      r2 = r2 - t1;
      s1 = s2 - t2;
      s2 = s2 + t2;
      src(2*i2-1) = ((int32)(bitshift(((int64)(r1*co1)),-32)) + (int32)(bitshift(((int64)(s1*si1)),-32)))/2;
      src(2*i2) = ((int32)(bitshift(((int64)(s1*co1)),-32)) - (int32)(bitshift(((int64)(r1*si1)),-32)))/2;
      src(2*i3-1) = ((int32)(bitshift(((int64)(r2*co3)),-32)) + (int32)(bitshift(((int64)(s2*si3)),-32)))/2;
      src(2*i3) = ((int32)(bitshift(((int64)(s2*co3)),-32)) - (int32)(bitshift(((int64)(r2*si3)),-32)))/2;
	end
  end
  ie = ie * 4;
  k=k/4;
end
%------------------------------------%
j = fftlen /4;
i=1;
while(1)
  xa = src(i);
  ya = src(i+1);
  xb = src(i+2);
  yb = src(i+3);
  xc = src(i+4);
  yc = src(i+5);
  xd = src(i+6);
  yd = src(i+7);

  xa_out = xa + xb + xc + xd;
  ya_out = ya + yb + yc + yd;

  src(i) = xa_out;
  src(i+1) = ya_out;
  xc_out = (xa - xb + xc - xd);
  yc_out = (ya - yb + yc - yd);
  src(i+2) = xc_out;
  src(i+3) = yc_out;
  xb_out = (xa + yb - xc - yd);
  yb_out = (ya - xb - yc + xd);
  src(i+4) = xb_out;
  src(i+5) = yb_out;
  xd_out = (xa - yb - xc + yd);
  yd_out = (ya + xb - yc - xd);
  src(i+6) = xd_out;
  src(i+7) = yd_out;
  j=j-1;
  if(j==0)
	break;
  end
  i=i+8;
end

%------------------------------------%
out = reverse(src, fftlen);
fwrite(ofid, out, 'int');
%-----------------------------------%
fclose(fid);
fclose(ofid);
cifft_radix4_q31;
