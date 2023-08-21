clear;
clc;
scaleval = 0x41000; % 0x40 in q_15 =1/(2^9), shift right 9 bits
shiftval = -5; % shift right 13 bits
scaleval_2 = 0x41000; % 0x40 in q_15 =1/(2^9), shift right 9 bits
shiftval_2 = 5; % shift right 13 bits



%-------------------Read data-------------------------%
fid = fopen('input_q31.bin', 'rb');
Inputdata = fread(fid, 32767, 'int');

%-------------------Data output-----------------------%
tmp1 = Inputdata * scaleval;
tmp2 = Inputdata * scaleval_2;
output = bitshift(tmp1,shiftval - 31);
output2 = bitshift(tmp2,shiftval_2 - 31);
golden = output;
golden2 = output2;
fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_scale_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int');
putdata = fwrite(ofid, golden2, 'int');
%-------------------Special case-----------------------%
in = 0x7ffffff0; scaleval_3 = 0x7ffffff0; shiftval_3 = 5;
output = bitshift(in * scaleval_3, shiftval_3 - 31);
putdata = fwrite(ofid, output, 'int32');
putdata = fwrite(ofid, output, 'int32');
putdata = fwrite(ofid, output, 'int32');

in = 0x8f000000; scaleval_3 = 0x70000000; shiftval_3 = 5;
output = 0x80000000;
putdata = fwrite(ofid, output, 'uint32');
putdata = fwrite(ofid, output, 'uint32');
putdata = fwrite(ofid, output, 'uint32');
%-------------------END-------------------------------%
fclose(ofid);
