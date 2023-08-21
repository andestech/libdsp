clear;
clc;

%-------------------Read data-------------------------%
fid = fopen('input_q15_a.bin', 'rb');
fid2 = fopen('input_q15_b.bin', 'rb');
InputA = fread(fid, 65536, 'short');
InputB = fread(fid2, 65536, 'short');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
outputdata = InputA + InputB;
fprintf('%d\n', outputdata);
%-------------------Data output-----------------------%
ofid = fopen('golden_add_q15.bin', 'wb');
putdata = fwrite(ofid, outputdata, 'short');
a=fclose(ofid);
%-------------------END-------------------------------%
