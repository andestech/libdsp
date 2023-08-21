clear;
clc;
datasize = 10000;
blocksize = 8;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_f32.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'float');
  [maximum(n),in(n)] = min(input);
  fprintf('%f %d\n',maximum(n), in(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_min_f32_min.bin', 'wb');
 o2fid = fopen('golden_min_f32_index.bin', 'wb');
 fwrite(ofid, maximum, 'float');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
