clear;
clc;
datasize = 350;
blocksize = 35;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_q7.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'int8');
  golden1(n) = mean(input);
  golden(n) = fix(golden1(n));
  fprintf('%f\n',golden(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_mean_q7.bin', 'wb');
 fwrite(ofid, golden, 'int8');
 fclose(ofid);
%-------------------END-------------------------------%
