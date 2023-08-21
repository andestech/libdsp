clear;
clc;
scaleval = 4; % 0x8 in q_7 =1/(2^8), shift left 1 bits
shiftval = 5; % shift right 3 bits
%-------------------Read data-------------------------%
fid = fopen('input_u8_a.bin', 'rb');
Inputdata = fread(fid, 255, 'uint8');
%-------------------convert q number------------------%

%-------------------Data output-----------------------%
output = bitshift(Inputdata,shiftval-scaleval);
golden = output;
fprintf('%d\n', golden);
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_scale_u8.bin', 'wb');
putdata = fwrite(ofid, golden, 'uint8');
a=fclose(ofid);
%-------------------END-------------------------------%
