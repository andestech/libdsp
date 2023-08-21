clear;
clc;
scaleval = 13; % 0x40 in q_15 =1/(2^9), shift right 9 bits
shiftval = 4; % shift right 13 bits
%-------------------Read data-------------------------%
fid = fopen('input_q15-1.bin', 'rb');
Inputdata = fread(fid, 65536, 'short');
%-------------------Data output-----------------------%
output = bitshift(Inputdata,shiftval-scaleval);
for ii= 1:65536
	if((Inputdata(ii) < 0) && (Inputdata(ii) != -32768))
		output(ii) = output(ii)-1;
	end
end
golden = output;
fprintf('%d\n', golden);
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_scale_q15.bin', 'wb');
putdata = fwrite(ofid, golden, 'short');
a=fclose(ofid);
%-------------------END-------------------------------%
