clear;
clc;
blocksize = 255;
loop = 100;

%-------------------Read data-------------------------%
fid = fopen('input_q31.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'uint32');
  [minimum(n),in(n)] = min(input);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_min_u32_min.bin', 'wb');
 o2fid = fopen('golden_min_u32_index.bin', 'wb');
 fwrite(ofid, minimum, 'uint32');
 fwrite(o2fid, in, 'uint32');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
