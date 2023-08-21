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
  [abs_maximum(n),in(n)] = max(abs(input)); %-abs-%
  fprintf('%f %d\n',abs_maximum(n), in(n));
end

 fclose(fid);
%-------------------Data output-----------------------%
 ofid = fopen('golden_absmax_f64_absmax.bin', 'wb');
 o2fid = fopen('golden_absmax_f64_index.bin', 'wb');
 fwrite(ofid, abs_maximum, 'double');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
