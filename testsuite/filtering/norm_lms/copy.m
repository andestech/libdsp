function out = copy(x0, x, blocksize, numtaps)

datasize = blocksize + numtaps -1;
out = zeros(datasize,1);

for m = 1: numtaps-1
  out(m) = x0(m);
end
k=1;
for m = numtaps : datasize
  out(m) = x(k);
  k=k+1;
end
