clear;
clc;
sample = 4;
datasize = 32768;
val = zeros(datasize,1);
inv = zeros(datasize/2,1);
%-------------------Read data-------------------------%
fid = fopen('input_mag_q31.bin', 'rb');
Input = fread(fid, datasize, 'int');
fclose(fid);
%-------------------Data transform--------------------%
%-----------------------------------------------------%
for pp = 1 : datasize/2
    real(pp) = (int64) (Input(2 * pp - 1)*Input(2 * pp - 1));
	imag(pp) = (int64) (Input(2 * pp) * Input(2 * pp));
	realshift(pp) = (int32)(bitshift(real(pp), -33));
	imagshift(pp) = (int32)(bitshift(imag(pp), -33));
	outputdata(pp) = realshift(pp) + imagshift(pp);
end
%-----------------------------------------------------%
%-----------------------------------------------------%

golden = outputdata;
fprintf('%d\n', golden);
%-------------------Data output-----------------------%
ofid = fopen('golden_mag_squared_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int');
a=fclose(ofid);
%-------------------END-------------------------------%
