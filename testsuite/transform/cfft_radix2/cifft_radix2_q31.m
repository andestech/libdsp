clear;
clc;
m= 10;
n = 1024; % n = 2^m;
itype = 1;
%-----------------------------------%
fid = fopen('golden_cfft_radix2_q31.bin', 'rb');
ofid = fopen('golden_cifft_radix2_q31.bin', 'wb');
%-----------------------------------%
for lop = 1 : n
  x(lop) = fread(fid, 1, 'int');
  y(lop) = fread(fid, 1, 'int');
end
p = 2*pi/n;
n2 = n;
n1 = n2;
n2 = n2 /2;
ie = n / n1;
ia = 1;
for jj = 1 : n2
  a = (ia-1) * p;
  c_q = cos(a);
  s_q = sin(a);
  c = q2int_q31(c_q,1);
  s = q2int_q31(s_q,1);
  ia = ia + ie;
  for q = jj : n1: n
    ll = q + n2;
    xt = (x(q)/4) - (x(ll)/4);
	  x(q) = ((x(q)/4) + (x(ll)/4))/2;
	  yt = (y(q)/4) -(y(ll)/4);
	  y(q) = ((y(q)/4) + (y(ll)/4))/2;
	  cxt_t = (int64)(c*xt);
	  syt_t = (int64)(s*yt);
	  cyt_t = (int64)(c*yt);
	  sxt_t = (int64)(s*xt);
	  x(ll) =(int32)(bitshift(cxt_t,-32)) -(int32)(bitshift(syt_t,-32));
	  y(ll) =(int32)(bitshift(cyt_t,-32)) +(int32)(bitshift(sxt_t,-32));
  end
end
%------------------------------------------%
for k = 1 : m-2
  n1 = n2;
  n2 = n2 /2;
  ie = n / n1;
  ia = 1;
  for jj = 1 : n2
	a = (ia-1) * p;
	c_q = cos(a);
	s_q = sin(a);
    c = q2int_q31(c_q,1);
    s = q2int_q31(s_q,1);
	ia = ia + ie;
	for q = jj : n1: n
	  ll = q + n2;
      xt = x(q) - x(ll);
	  x(q) = (x(q) + x(ll))/2;
	  yt = y(q) -y(ll);
	  y(q) = (y(q) + y(ll))/2;
	  cxt_t = (int64)(c*xt);
	  syt_t = (int64)(s*yt);
	  cyt_t = (int64)(c*yt);
	  sxt_t = (int64)(s*xt);
	  x(ll) =(int32)(bitshift(cxt_t,-32)) -(int32)(bitshift(syt_t,-32));
	  y(ll) =(int32)(bitshift(cyt_t,-32)) +(int32)(bitshift(sxt_t,-32));
	end
  end
end
%------------------------------------%
n1 = n2;
n2 = n2 /2;
ie = n / n1;
ia = 1;
  a = (ia-1) * p;
  c_q = cos(a);
  s_q = sin(a);
  c = q2int_q31(c_q,1);
  s = q2int_q31(s_q,1);
  ia = ia + ie;
  q=1;
  while( q < n)
	ll = q + n2;
	xt = x(q) - x(ll);
	x(q) = x(q) + x(ll);
	yt = y(q) -y(ll);
	y(q) = y(q) + y(ll);
	x(ll) = xt;
    y(ll) = yt;
	q = q+n1;
	ll = q+n2;
	xt = x(q) - x(ll);
	x(q) = x(q) + x(ll);
	yt = y(q) -y(ll);
	y(q) = y(q) + y(ll);
	x(ll) = xt;
	y(ll) = yt;
	q = q+n1;
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
  fwrite(ofid, x(nnn), 'int');
  fwrite(ofid, y(nnn), 'int');
end
%-----------------------------------%
fclose(fid);
fclose(ofid);