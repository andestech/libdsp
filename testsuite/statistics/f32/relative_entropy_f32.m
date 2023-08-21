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
  tmpacc = 0;
  for m = 1 : blocksize
    a = input1(m) .* log(input2(m)/input1(m));
    tmpacc += a;
  end
  golden(n) = -tmpacc;
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_relative_entropy_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
