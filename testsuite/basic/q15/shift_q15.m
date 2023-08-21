clear;
clc;

%-------------------Read data-------------------------%
fid = fopen('input_q15-1.bin', 'rb');
ofid = fopen('golden_shift_q15.bin', 'wb');
Inputdata = fread(fid, 32761, 'short');
%-------------------Data output-----------------------%
output = bitshift(Inputdata, -3);
putdata = fwrite(ofid, output, 'short');
%-------------------Data output-----------------------%
output = bitshift(Inputdata, 3);
putdata = fwrite(ofid, output, 'short');
%--------------------------------------------------%
a=fclose(ofid);
%-------------------END-------------------------------%
