clear;
clc;
datasize = 170;
blocksize = 17;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_mult_q7_a.bin', 'rb');
fid2 = fopen('input_mult_q7_b.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  fprintf('loop %d\n', n - 1);
  inputa = fread(fid, blocksize, 'char')
  inputb = fread(fid2, blocksize, 'char')
  inputa = bitshift(inputa, -1, 8)
  inputb = bitshift(inputb, -1, 8)
  inputa = inputa - inputb;
  inputa = min(inputa, 127);
  inputa = max(inputa, -128)
  golden = (inputa .* inputa);
  out(n) = sum(golden);
  tt = bitshift((out(n) / blocksize), -5);
  result(n) = min(tt, 127);
  result(n) = max(tt, -128);
  fprintf('%d\n', result(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_mse_q7.bin', 'wb');
 fwrite(ofid, result, 'char');
 fclose(ofid);
%-------------------END-------------------------------%
