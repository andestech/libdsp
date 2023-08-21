clear;
clc;
datasize = 256;
%-------------------Read data-------------------------%
fid = fopen('input_u8_a.bin', 'rb');
fid2 = fopen('input_u8_b.bin', 'rb');
InputA = fread(fid, datasize, 'uint8');
InputB = fread(fid2, datasize, 'uint8');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
outputdata = InputA - InputB;
fprintf('%d\n', outputdata);
%-------------------Data output-----------------------%
ofid = fopen('golden_sub_u8.bin', 'wb');
putdata = fwrite(ofid, outputdata, 'int8');
a=fclose(ofid);
%-------------------END-------------------------------%
