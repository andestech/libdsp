clear;
clc;
datasize = 1100;
blocksize = 11;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_u8_a.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'uint8');
  [maximum(n),in(n)] = max(input);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_max_u8_max.bin', 'wb');
 o2fid = fopen('golden_max_u8_index.bin', 'wb');
 fwrite(ofid, maximum, 'uint8');
 fwrite(o2fid, in, 'uint32');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
