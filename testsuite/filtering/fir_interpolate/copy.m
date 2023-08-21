function out = copy(oldin, newin, blocksize, plength)

datasize = blocksize + plength -1;
out = zeros(datasize,1);

for m = 1: plength-1
  out(m) = oldin(m);
end
k=1;
for m = plength : datasize
  out(m) = newin(k);
  k=k+1;
end
