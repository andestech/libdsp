clear;
clc;

datasize = 500;
blocksize = 10;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_q7.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'int8');
  [minimum(n),in(n)] = min(abs(input));
  intq = int2q_q7(input,blocksize);
  minq = int2q_q7(minimum(n),1);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_absmin_q7_absmin.bin', 'wb');
 o2fid = fopen('golden_absmin_q7_index.bin', 'wb');
 fwrite(ofid, minimum, 'int8');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
