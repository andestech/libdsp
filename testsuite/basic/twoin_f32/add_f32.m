clear;
clc;

%-------------------Read data-------------------------%
fid = fopen('input_f32_a.bin', 'rb');
fid2 = fopen('input_f32_b.bin', 'rb');
InputA = fread(fid, 65536, 'single');
InputB = fread(fid2, 65536, 'single');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
outputdata = InputA + InputB;
fprintf('%f\n', outputdata);
%-------------------Data output-----------------------%
ofid = fopen('golden_add_f32.bin', 'wb');
putdata = fwrite(ofid, outputdata, 'single');
a=fclose(ofid);
%-------------------END-------------------------------%
