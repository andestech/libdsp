clear;
clc;
%-------------------Read data-------------------------%
fid = fopen('input_u8_a.bin', 'rb');
ofid = fopen('golden_shift_u8.bin', 'wb');
Inputdata = fread(fid, 255, 'uint8');
%-------------------convert q number------------------%

%-------------------Data output-----------------------%
output = bitshift(Inputdata, -3);
putdata = fwrite(ofid, output, 'uint8');
output = bitshift(Inputdata, 3);
putdata = fwrite(ofid, output, 'uint8');
a=fclose(ofid);
%-------------------END-------------------------------%
