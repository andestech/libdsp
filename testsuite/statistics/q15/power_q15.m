clear;
clc;
datasize = 30000;
blocksize = 15;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_sat_q15.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'short');
  golden(n) = sum(input.*input);
  fprintf('%d\n',golden(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_power_q15.bin', 'wb');
 fwrite(ofid, golden, 'int64');
 fclose(ofid);
%-------------------END-------------------------------%
