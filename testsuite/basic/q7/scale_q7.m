clear;
clc;
scaleval = 4; % 0x8 in q_7 =1/(2^8), shift left 1 bits
shiftval = 2; % shift right 3 bits
%-------------------Read data-------------------------%
fid = fopen('input_q7.bin', 'rb');
Inputdata = fread(fid, 255, 'int8');
%-------------------convert q number------------------%

%-------------------Data output-----------------------%
output = bitshift(Inputdata,shiftval-scaleval);
for ii= 1:255
	if((Inputdata(ii) < 0) && (Inputdata(ii) != -256))
		output(ii) = output(ii)-1;
	end
end
golden = output;
fprintf('%d\n', golden);
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_scale_q7.bin', 'wb');
putdata = fwrite(ofid, golden, 'int8');
a=fclose(ofid);
%-------------------END-------------------------------%
