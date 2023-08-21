function out = copy(oldin, newin, tap)

datasize = tap;
out = zeros(datasize,1);

for m = 1 : datasize
  if(m == 1)
	out(m) = newin;
  else
	out(m) = oldin(m-1);
  end
end
