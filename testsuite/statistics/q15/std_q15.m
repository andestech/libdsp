clear;
clc;
datasize = 30000;
blockSize = 15;
loop = datasize/blockSize;

%-------------------Read data-------------------------%
fid = fopen('input_sat_q15.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
input = fread(fid, blockSize, 'short');
square = input.*input;
sos = (sum(square));
ts = (sum(input));
meansqr = (int32)(sos /(blockSize - 1));
sqrmean = (int32)((ts .* ts) /(blockSize.*(blockSize-1)));
tmp1 = bitshift((meansqr - sqrmean), -15);
if(tmp1 >= 2^15-1)
    out = 2^15-1;
elseif(tmp1 < -2^15)
    out = -2^15;
else
    out = tmp1;
end
golden(n) = out;
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_std_sqrt_q15.bin', 'wb');
 fwrite(ofid, golden, 'int16');
 fclose(ofid);
%-------------------END-------------------------------%