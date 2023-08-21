clear;
clc;
datasize = 10000;
blocksize = 1000;
loop = datasize/blocksize;
input1 = zeros(1, 8);
input2 = zeros(1, 8);

%-------------------Read data-------------------------%
fid = fopen('input_f32.bin', 'rb');
fid2 = fopen('input_f32_2.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input1 = fread(fid, blocksize, 'float');
  input2 = fread(fid2, blocksize, 'float');
  input = input1 - input2;
  golden(n) = meansq(input);
  fprintf('%f\n',golden(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_mse_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
