clear;
clc;
%-------------------Read data-------------------------%
fid = fopen('input_q7.bin', 'rb');
ofid = fopen('golden_shift_q7.bin', 'wb');
Inputdata = fread(fid, 255, 'int8');
%-------------------convert q number------------------%

%-------------------Data output-----------------------%
output = bitshift(Inputdata, -3);
putdata = fwrite(ofid, output, 'int8');
output = bitshift(Inputdata, 3);
putdata = fwrite(ofid, output, 'int8');
a=fclose(ofid);
%-------------------END-------------------------------%
