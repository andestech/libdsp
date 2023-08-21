clear;
clc;
val = zeros(65536,1);
%-------------------Read data-------------------------%
fid = fopen('input_q15-1.bin', 'rb');
Inputdata = fread(fid, 65536, 'short');
%-------------------convert q number------------------%
val = int2q_q15(Inputdata, 65536);
%-------------------Data output-----------------------%
output = abs(val);
golden = q2int_q15(output, 65536);
printf('%d\n', golden);
%-----------------------------------------------------%
ofid = fopen('golden_abs_q15.bin', 'wb');
putdata = fwrite(ofid, golden, 'short');
a=fclose(fid);
a=fclose(ofid);
%-------------------END-------------------------------%
