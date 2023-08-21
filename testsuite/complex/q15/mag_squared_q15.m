clear;
clc;
sample = 4;
datasize = 65536;
val = zeros(datasize,1);
inv = zeros(datasize/2,1);
%-------------------Read data-------------------------%
fid = fopen('input_mag_q15.bin', 'rb');
Input = fread(fid, datasize, 'short');
fclose(fid);
%-------------------Data transform--------------------%
%-----------------------------------------------------%
for pp = 1 : datasize/2
    a = (int32)(Input(2*pp-1)*Input(2*pp-1));
	b = (int32)(Input(2*pp)*Input(2*pp));
	outputdata(pp) = (int64)(a + b);
	out(pp) = bitshift(outputdata(pp),-17);
end
%-----------------------------------------------------%
golden = out;
fprintf('%d\n', golden);
%-------------------Data output-----------------------%
ofid = fopen('golden_mag_squared_q15.bin', 'wb');
putdata = fwrite(ofid, golden, 'short');
a=fclose(ofid);
%-------------------END-------------------------------%
