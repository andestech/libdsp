clear;
clc;
blocksize = 255;
loop = 100;
datasize=blocksize*loop;

%-------------------Read data-------------------------%
fid = fopen('input_q31.bin', 'rb');
data = fread(fid, datasize, 'int');
fclose(fid);

%set corner values%
data(floor(blocksize/2)) = 2147483647;
data(blocksize + floor(blocksize/2)) = -2147483648;
%-------------------Data output-----------------------%
 ofid = fopen('input_with_corner_q31.bin', 'wb');
 fwrite(ofid, data, 'int');
 fclose(ofid);

%-------------------Read data-------------------------%
fid = fopen('input_with_corner_q31.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'int');
  [maximum(n),in(n)] = max(abs(input));
  intq = int2q_q31(input,blocksize);
  maxq = int2q_q31(maximum(n),1);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_absmax_q31_absmax.bin', 'wb');
 o2fid = fopen('golden_absmax_q31_index.bin', 'wb');
 fwrite(ofid, maximum, 'int');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
