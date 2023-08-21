clear;
clc;
datasize = 10000;
blocksize = 16;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_f64_a.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'uint64');
  [maximum(n),in(n)] = max(input);
  fprintf('%d %d\n',maximum(n), in(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_max_u64_max.bin', 'wb');
 o2fid = fopen('golden_max_u64_index.bin', 'wb');
 fwrite(ofid, maximum, 'uint64');
 fwrite(o2fid, in, 'uint32');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
