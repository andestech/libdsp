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
  [minimum(n),in(n)] = min(input);
  intq = int2q_q7(input,blocksize);
  %fprintf('%d\n',input);
  %fprintf('%f\n',intq);
  maxq = int2q_q7(minimum(n),1);
  %fprintf('%d %d %f\n',maximum(n), in(n),maxq);
  %fprintf('-----------------\n');
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_min_q7_min.bin', 'wb');
 o2fid = fopen('golden_min_q7_index.bin', 'wb');
 fwrite(ofid, minimum, 'int8');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
