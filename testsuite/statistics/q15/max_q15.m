clear;
clc;
blocksize = 255;
loop = 100;

%-------------------Read data-------------------------%
fid = fopen('input_q15.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'short');
  [maximum(n),in(n)] = max(input);
  intq = int2q_q15(input,blocksize);
  %fprintf('%d\n',input);
  %fprintf('%f\n',intq);
  maxq = int2q_q15(maximum(n),1);
  %fprintf('%d %d %f\n',maximum(n), in(n),maxq);
  %fprintf('-----------------\n');
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_max_q15_max.bin', 'wb');
 o2fid = fopen('golden_max_q15_index.bin', 'wb');
 fwrite(ofid, maximum, 'short');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
