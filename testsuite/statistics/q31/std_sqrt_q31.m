clear;
clc;
datasize = 10000/4;

%-------------------Read data-------------------------%
fid = fopen('golden_std_sqrt_q31.bin', 'rb');
Inputdata = fread(fid, datasize, 'int');
%-------------------convert q number------------------%
val = int2q_q31(Inputdata,datasize);
%-------------------Data output-----------------------%
output = sqrt(val);
golden = q2int_q31(output,datasize);
fprintf('%d\n', golden);
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_std_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int');
a=fclose(ofid);
%-------------------END-------------------------------%
