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
  golden(n)= sum(input.*input);
  fprintf('%f\n',golden(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_power_f32.bin', 'wb');
 fwrite(ofid, golden, 'float32');
 fclose(ofid);
%-------------------END-------------------------------%
