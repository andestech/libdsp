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
  [minimum(n),in(n)] = min(input);
  fprintf('%d %d\n',minimum(n), in(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_min_u64_min.bin', 'wb');
 o2fid = fopen('golden_min_u64_index.bin', 'wb');
 fwrite(ofid, minimum, 'uint64');
 fwrite(o2fid, in, 'uint32');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
