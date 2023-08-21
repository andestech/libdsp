clear;
clc;
datasize = 10000;
blocksize = 8;
loop = datasize/blocksize;
%-------------------Read input data-------------------------%
fid = fopen('input_f64_a.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'double');
  [abs_minimum(n),in(n)] = min(abs(input)); %-abs-%
  fprintf('%f %d\n',abs_minimum(n), in(n));
end

 fclose(fid);
%-------------------Data output-----------------------%
 ofid = fopen('golden_absmin_f64_absmin.bin', 'wb');
 o2fid = fopen('golden_absmin_f64_index.bin', 'wb');
 fwrite(ofid, abs_minimum, 'double');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
