clear;
clc;
datasize = 1500;
blocksize = 15;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_var_q31.bin', 'rb');
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
  out1(n) = (int64)(d - g);
  out(n) = fix(out1(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_var_q31.bin', 'wb');
 fwrite(ofid, out, 'int64');
 fclose(ofid);
 fclose(fid);
%-------------------END-------------------------------%
