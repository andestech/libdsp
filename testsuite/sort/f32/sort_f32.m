clear;
clc;

size=256;

%-------------------Read data-------------------------%
fid = fopen('input_f32.bin', 'rb');
Inputdata = fread(fid, size, 'single');
a=fclose(fid);
%-------------------Data transform--------------------%
ascend_out = sort(Inputdata, 'ascend')
descend_out = sort(Inputdata, 'descend')
%-------------------Data output-----------------------%
ofid1 = fopen('golden_sort_ascend_f32.bin', 'wb');
ofid2 = fopen('golden_sort_descend_f32.bin', 'wb');
outdata1 = fwrite(ofid1, ascend_out, 'single');
outdata2 = fwrite(ofid2, descend_out, 'single');
a=fclose(ofid1);
a=fclose(ofid2);
%-------------------END-------------------------------%
