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
  tmpacc = 0;
  for m = 1 : blocksize
    a = input(m) .* log(input(m));
    tmpacc += a;
  end
  golden(n) = -tmpacc;
end  

%-------------------Data output-----------------------%
 ofid = fopen('golden_entropy_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
