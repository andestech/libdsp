clear;
clc;
datasize = 9900;
blocksize = 99;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_sat_q15.bin', 'rb');
fid2 = fopen('input_q15.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  inputa = fread(fid, blocksize, 'short');
  inputb = fread(fid2, blocksize, 'short');
  inputa = bitshift(inputa, -1);
  inputb = bitshift(inputb, -1);
  inputa = inputa - inputb;
  inputa = min(inputa, 32767);
  inputa = max(inputa, -32768);
  golden = (int64)(inputa .* inputa);
  out(n) = sum(golden);
  tt = bitshift((out(n) / blocksize), -13);
  result(n) = min(tt, 32767);
  result(n) = max(tt, -32768);
  fprintf('%d\n', result(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_mse_q15.bin', 'wb');
 fwrite(ofid, result, 'short');
 fclose(ofid);
%-------------------END-------------------------------%
