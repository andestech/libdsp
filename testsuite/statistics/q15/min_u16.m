clear;
clc;
blocksize = 255;
loop = 100;

%-------------------Read data-------------------------%
fid = fopen('input_q15.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'uint16');
  [minimum(n),in(n)] = min(input);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_min_u16_min.bin', 'wb');
 o2fid = fopen('golden_min_u16_index.bin', 'wb');
 fwrite(ofid, minimum, 'uint16');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
