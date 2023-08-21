function out = reverse(input, n)

for lop = 1 : n
  x(lop) = input(2*lop-1);
  y(lop) = input(2*lop);
end

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

for lop = 1 : n
  out(2*lop-1) = x(lop);
  out(2*lop) = y(lop);
end
