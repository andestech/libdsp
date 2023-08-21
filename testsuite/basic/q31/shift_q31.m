clear;
clc;
shift1 = -7;
shift2 = 7;
%-------------------Read data-------------------------%
fid = fopen('input_q31.bin', 'rb');
Inputdata = fread(fid, 16383, 'int');
%-------------------convert q number------------------%

%-------------------Data output-----------------------%
output = bitshift(Inputdata,shift1);
golden = output;
ofid = fopen('golden_shift_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int');
%-------------------Data output-----------------------%
output = bitshift(Inputdata,shift2);
golden = output;
putdata = fwrite(ofid, golden, 'int');
%--------------------------------------------------%
a=fclose(ofid);
%-------------------END-------------------------------%
