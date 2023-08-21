clear;
clc;
datasize = 350;
blocksize = 35;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_u8_a.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'uint8');
  fprintf('%d\n',input);
  golden(n) = floor(mean(input));
  fprintf('%d\n',golden(n));
  fprintf('---------------\n');
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_mean_u8.bin', 'wb');
 fwrite(ofid, golden, 'uint8');
 fclose(ofid);
%-------------------END-------------------------------%
