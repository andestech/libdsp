clear;
clc;

%-------------------Read data-------------------------%
fid = fopen('input_q31_a.bin', 'rb');
fid2 = fopen('input_q31_b.bin', 'rb');
InputA = fread(fid, 32768, 'int');
InputB = fread(fid2, 32768, 'int');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
outputdata = InputA - InputB;
fprintf('%d\n', outputdata);
%-------------------Data output-----------------------%
ofid = fopen('golden_sub_q31.bin', 'wb');
putdata = fwrite(ofid, outputdata, 'int');
a=fclose(ofid);
%-------------------END-------------------------------%
