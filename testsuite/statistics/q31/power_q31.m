clear;
clc;
datasize = 30000;
blocksize = 8;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_q31.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'int');
  golden = (int64)(input.*input);
  out(n) = sum(bitshift(golden,-14));
  fprintf('%d\n',out(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_power_q31.bin', 'wb');
 fwrite(ofid, out, 'int64');
 fclose(ofid);
%-------------------END-------------------------------%
