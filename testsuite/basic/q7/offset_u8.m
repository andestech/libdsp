clear;
clc;

datasize = 258;
offset1 =-20;
offset2 = 30;
offset3 = -128
%-------------------Read data-------------------------%
fid = fopen('input_u8_a.bin', 'rb');
Inputdata = fread(fid, datasize, 'uint8');
%-------------------convert q number------------------%
for nn = 1 : datasize
    golden1(nn) = Inputdata(nn) + offset1;
    golden2(nn) = Inputdata(nn) + offset2;
    golden3(nn) = Inputdata(nn) + offset3;
end
%-------------------Data output-----------------------%
ofid = fopen('golden_offset_u8.bin', 'wb');
o2fid = fopen('golden_offset_u8_2.bin', 'wb');
o3fid = fopen('golden_offset_u8_3.bin', 'wb');
putdata1 = fwrite(ofid, golden1, 'uint8');
putdata2 = fwrite(o2fid, golden2, 'uint8');
putdata3 = fwrite(o3fid, golden3, 'uint8');

a=fclose(ofid);
%-------------------END-------------------------------%
