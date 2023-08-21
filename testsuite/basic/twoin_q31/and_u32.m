clear;
clc;
datasize = 16383;

%-------------------Read data-------------------------%
fid = fopen('input_dot_a.bin', 'rb');
fid2 = fopen('input_dot_b.bin', 'rb');
InputA = fread(fid, datasize, 'uint32');
InputB = fread(fid2, datasize, 'uint32');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
for nn = 1 : datasize
    outputdata(nn) = bitand(InputA(nn), InputB(nn));
end
%-------------------Data output-----------------------%
ofid = fopen('golden_and_u32.bin', 'wb');
putdata = fwrite(ofid, outputdata, 'uint32');
a=fclose(ofid);
%-------------------END-------------------------------%
