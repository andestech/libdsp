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
  [minimum(n),in(n)] = min(input);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_min_u8_min.bin', 'wb');
 o2fid = fopen('golden_min_u8_index.bin', 'wb');
 fwrite(ofid, minimum, 'uint8');
 fwrite(o2fid, in, 'uint32');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
