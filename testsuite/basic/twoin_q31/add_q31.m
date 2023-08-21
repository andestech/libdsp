clear;
clc;


inputsize = 32768;
%-------------------Read data-------------------------%
fid = fopen('input_q31_a.bin', 'rb');
fid2 = fopen('input_q31_b.bin', 'rb');
InputA = fread(fid, inputsize, 'int');
InputB = fread(fid2, inputsize, 'int');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
outputdata = InputA + InputB;
fprintf('%d\n', outputdata);
%-------------------Data output-----------------------%
ofid = fopen('golden_add_q31.bin', 'wb');
putdata = fwrite(ofid, outputdata, 'int');
a=fclose(ofid);
%-------------------END-------------------------------%
