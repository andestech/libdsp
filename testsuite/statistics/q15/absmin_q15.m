clear;
clc;
blocksize = 255;
loop = 100;

%-------------------Read data-------------------------%
fid = fopen('input_q15.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'short');
  [minimum(n),in(n)] = min(abs(input));
  intq = int2q_q15(input,blocksize);
  minq = int2q_q15(minimum(n),1);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_absmin_q15_absmin.bin', 'wb');
 o2fid = fopen('golden_absmin_q15_index.bin', 'wb');
 fwrite(ofid, minimum, 'short');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
