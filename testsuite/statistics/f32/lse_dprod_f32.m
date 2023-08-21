clear;
clc;
datasize = 10000;
blocksize = 8;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_f32.bin', 'rb');
fid2 = fopen('input_f32_2.bin', 'rb');
%-------------------Data transform--------------------%

for n = 1 : loop
  input1 = fread(fid, blocksize, 'float');
  input2 = fread(fid2, blocksize, 'float');
  buffer = input1 + input2;
  maxval = max(buffer);
  tmpacc = 0;

  for m = 1 : blocksize
    a = exp(buffer(m) - maxval);
    tmpacc += a;
  end
  golden(n) = maxval + log(tmpacc);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_lse_dprod_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
