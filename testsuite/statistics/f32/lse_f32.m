clear;
clc;
datasize = 10000;
blocksize = 8;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_f32.bin', 'rb');
%-------------------Data transform--------------------%

for n = 1 : loop
  input = fread(fid, blocksize, 'float');
  maxval = max(input);
  tmpacc = 0;
  
  for m = 1 : blocksize
    a = exp(input(m) - maxval);
    tmpacc += a;
  end
  golden(n) = maxval + log(tmpacc);
end  

%-------------------Data output-----------------------%
 ofid = fopen('golden_lse_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
