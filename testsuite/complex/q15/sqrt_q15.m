clear;
clc;
datasize = 32768;
val = zeros(datasize,1);

%-------------------Read data-------------------------%
fid = fopen('golden_mag_squared_q15.bin', 'rb');
Inputdata = fread(fid, datasize, 'short');
%-------------------convert q number------------------%
val = int2q_q15(Inputdata,datasize);
%-------------------Data output-----------------------%
output = sqrt(val);
golden = q2int_q15(output,datasize);
fprintf('%d\n', golden);
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_mag_q15.bin', 'wb');
putdata = fwrite(ofid, golden, 'short');
a=fclose(ofid);
%-------------------END-------------------------------%