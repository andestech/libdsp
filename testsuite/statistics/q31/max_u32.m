clear;
clc;
blocksize = 255;
loop = 100;

%-------------------Read data-------------------------%
fid = fopen('input_q31.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'uint32');
  [maximum(n),in(n)] = max(input);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_max_u32_max.bin', 'wb');
 o2fid = fopen('golden_max_u32_index.bin', 'wb');
 fwrite(ofid, maximum, 'uint32');
 fwrite(o2fid, in, 'uint32');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
