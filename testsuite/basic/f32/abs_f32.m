clear;
clc;

%-------------------Read data-------------------------%
fid = fopen('input_f32.bin', 'rb');
Inputdata = fread(fid, 65536, 'single');
a=fclose(fid);
%-------------------Data transform--------------------%
outputdata = abs(Inputdata);
fprintf('%f\n', outputdata);
%-------------------Data output-----------------------%
ofid = fopen('golden_abs_f32.bin', 'wb');
putdata = fwrite(ofid, outputdata, 'single');
a=fclose(ofid);
%-------------------END-------------------------------%
