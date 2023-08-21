clear;
clc;
blocksize = 255;
loop = 100;

%-------------------Read data-------------------------%
fid = fopen('input_q15.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'uint16');
  [maximum(n),in(n)] = max(input);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_max_u16_max.bin', 'wb');
 o2fid = fopen('golden_max_u16_index.bin', 'wb');
 fwrite(ofid, maximum, 'uint16');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
