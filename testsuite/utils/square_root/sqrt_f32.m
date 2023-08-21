clear;
clc;
val = zeros(32768,1);
%-------------------Read data-------------------------%
fid = fopen('input_sqrt_f32.bin', 'rb');
Inputdata = fread(fid, 32768, 'single');
%-------------------Data output-----------------------%
output = sqrt(Inputdata);
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_sqrt_f32.bin', 'wb');
putdata = fwrite(ofid, output, 'single');
a=fclose(ofid);
%-------------------END-------------------------------%
