clear;
clc;
m= 3;
n = 64; % n = 2^m;
itype = 1;
%-----------------------------------%
fid = fopen('input_f32.bin', 'rb');
ofid = fopen('golden_cfft_radix4_f32.bin', 'wb');
%-----------------------------------%
for lop = 1 : n
  x(lop) = fread(fid, 1, 'float');
  y(lop) = fread(fid, 1, 'float');
end
p = 2*pi/n;
n2 = n;
ie = 1;
for k = 1 : m
  n1 = n2;
  n2 = n2 /4;
  ia1 = 0;
  for jj = 1 : n2
	ia2 = ia1 + ia1;
	ia3 = ia2 + ia1;
	a1 = ia1 * p;
	a2 = ia2 * p;
	a3 = ia3 * p;
	co1 = cos(a1);
	si1 = sin(a1);
	co2 = cos(a2);
	si2 = sin(a2);
	co3 = cos(a3);
	si3 = sin(a3);
	ia1 = ia1 + ie;
	for i0 = jj : n1: n
	  i1 = i0 + n2;
	  i2 = i1 + n2;
	  i3 = i2 + n2;
	  r1 = x(i0) + x(i2);
	  r2 = x(i0) - x(i2);
	  s1 = y(i0) + y(i2);
	  s2 = y(i0) - y(i2);
	  t1 = x(i1) + x(i3);
	  x(i0) = r1 + t1;
	  r1 = r1 - t1;
	  t2 = y(i1) + y(i3);
	  y(i0) = s1 + t2;
	  s1 = s1 - t2;
	  t1 = y(i1) - y(i3);
	  t2 = x(i1) - x(i3);
	  x(i1) = r1*co2 + s1*si2;
	  y(i1) = s1*co2 - r1*si2;
	  r1 = r2 + t1;
	  r2 = r2 - t1;
	  s1 = s2 - t2;
	  s2 = s2 + t2;
	  x(i2) = r1*co1 + s1*si1;
	  y(i2) = s1*co1 - r1*si1;
	  x(i3) = r2*co3 + s2*si3;
	  y(i3) = s2*co3 - r2*si3;
	end
  end
	ie = ie *4;
end
%------------------------------------%
h=1;
for nn = 1 : n-1
  if(nn < h)
	xt = x(h);
	x(h) = x(nn);
	x(nn) = xt;
	yt = y(h);
	y(h) = y(nn);
	y(nn) = yt;
  end
  kk = n/2;
  while(kk< h)
	h = h - kk;
	kk = kk/2;
  end
  h = h + kk;
end
for nnn = 1: n
  fwrite(ofid, x(nnn), 'float');
  fwrite(ofid, y(nnn), 'float');
end
%-----------------------------------%
fclose(fid);
fclose(ofid);
cifft_radix4_f32;
