clear;
clc;
datasize = 3000;

%-------------------Read data-------------------------%
fid = fopen('input_f64_a.bin', 'rb');
%------------------- log--------------------%

input = fread(fid, datasize, 'double');
golden = log(input);

%-------------------log end -----------------------%
 ofid = fopen('golden_log_f64.bin', 'wb');
 fwrite(ofid, golden, 'double');
 fclose(ofid);
%-------------------END-------------------------------%
