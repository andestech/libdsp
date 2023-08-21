clear;
clc;
datasize = 13100;
blocksize = 131;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_q31.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'int');
  golden(n) = mean(input);
  fprintf('%d\n',golden(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_mean_q31.bin', 'wb');
 fwrite(ofid, golden, 'int');
 fclose(ofid);
%-------------------END-------------------------------%
