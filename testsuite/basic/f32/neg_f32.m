clear;
clc;

%-------------------Read data-------------------------%
fid = fopen('input_f32.bin', 'rb');
Inputdata = fread(fid, 65536, 'single');
a=fclose(fid);
%-------------------Data transform--------------------%
outputdata = -Inputdata;
fprintf('%f\n', outputdata);
%-------------------Data output-----------------------%
ofid = fopen('golden_negate_f32.bin', 'wb');
putdata = fwrite(ofid, outputdata, 'single');
a=fclose(ofid);
%-------------------END-------------------------------%
testfid = fopen('golden_negate_f32.bin', 'rb');
test = fread(testfid, 65536, 'uint32');
a=fclose(testfid);
