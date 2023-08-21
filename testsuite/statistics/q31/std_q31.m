clear;
clc;
datasize = 10000;
blocksize = 4;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_std_q31.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'int');
  a = (int64)(input.*input);
  A = (int64)(sum(a));
  b = (int64)(sum(input));
  t = (int64)((1 / (blocksize - 1)) * 1073741824);
  c = (int64)(bitshift(A, -31));
  d = (int32)(bitshift((c .* t),-30));
  t1 = ((1 / (blocksize * (blocksize - 1))) * 2147483648);
  f = (bitshift(((b .* b)),-31));
  g = (int32)(bitshift(((f .* t1)),-31));
  out(n) = (d - g);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_std_sqrt_q31.bin', 'wb');
 fwrite(ofid, out, 'int32');
 fclose(ofid);
 fclose(fid);
%-------------------END-------------------------------%
