clear;
clc;
blocksize = 255;
loop = 100;

%-------------------Read data-------------------------%
fid = fopen('input_q31.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'int');
  [maximum(n),in(n)] = max(input);
  intq = int2q_q31(input,blocksize);
  %fprintf('%d\n',input);
  %fprintf('%f\n',intq);
  maxq = int2q_q31(maximum(n),1);
  %fprintf('%d %d %f\n',maximum(n), in(n),maxq);
  %fprintf('-----------------\n');
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_max_q31_max.bin', 'wb');
 o2fid = fopen('golden_max_q31_index.bin', 'wb');
 fwrite(ofid, maximum, 'int');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
