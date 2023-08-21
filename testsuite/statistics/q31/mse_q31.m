clear;
clc;
datasize = 9990;
blocksize = 999;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_sat_q31.bin', 'rb');
fid2 = fopen('input_std_q31.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  inputa = fread(fid, blocksize, 'int')
  inputb = fread(fid2, blocksize, 'int')
  inputa = bitshift(inputa,-1);
  inputb = bitshift(inputb,-1);
  inputa = inputa - inputb;
  golden = (int64)(inputa .* inputa);
  out(n) = sum(bitshift(golden,-14));
  result(n) = bitshift((out(n) / blocksize), -15);
  fprintf('%d\n', result(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_mse_q31.bin', 'wb');
 fwrite(ofid, result, 'int32');
 fclose(ofid);
%-------------------END-------------------------------%
