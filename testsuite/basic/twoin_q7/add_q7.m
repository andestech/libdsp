clear;
clc;
datasize = 256;
%-------------------Read data-------------------------%
fid = fopen('input_q7_a.bin', 'rb');
fid2 = fopen('input_q7_b.bin', 'rb');
InputA = fread(fid, datasize, 'int8');
InputB = fread(fid2, datasize, 'int8');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
outputdata = InputA + InputB;
fprintf('%d\n', outputdata);
%-------------------Data output-----------------------%
ofid = fopen('golden_add_q7.bin', 'wb');
putdata = fwrite(ofid, outputdata, 'int8');
a=fclose(ofid);
%-------------------END-------------------------------%
