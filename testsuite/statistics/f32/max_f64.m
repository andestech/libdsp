clear;
clc;
datasize = 10000;
blocksize = 16;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_f64_a.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'double');
  [maximum(n),in(n)] = max(input);
  fprintf('%f %d\n',maximum(n), in(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_max_f64_max.bin', 'wb');
 o2fid = fopen('golden_max_f64_index.bin', 'wb');
 fwrite(ofid, maximum, 'double');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
