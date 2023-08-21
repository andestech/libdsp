function out = copy(oldin, newin, blocksize, tap)

datasize = blocksize + tap -1;
out = zeros(datasize,1);

for m = 1: tap-1
  out(m) = oldin(m);
end
k=1;
for m = tap : datasize
  out(m) = newin(k);
  k=k+1;
end
