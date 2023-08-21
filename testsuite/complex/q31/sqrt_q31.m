clear;
clc;
datasize = 32768/2;
%-------------------Read data-------------------------%
fid = fopen('golden_mag_squared_q31.bin', 'rb');
Inputdata = fread(fid, datasize, 'int');
%-------------------convert q number------------------%
a = int2q_q31(Inputdata, datasize);
%-------------------Data output-----------------------%
output = sqrt(a);
golden = q2int_q31(output, datasize);
fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_mag_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int');
fprintf('%d\n',golden);
fclose(ofid);
%-------------------END-------------------------------%
