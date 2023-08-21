clear;
clc;
val = zeros(32768,1);
%-------------------Read data-------------------------%
fid = fopen('input_sqrt_q15.bin', 'rb');
Inputdata = fread(fid, 32768, 'short');
%-------------------convert q number------------------%
for nn = 1 : 32768
    remain = Inputdata(nn);
    for n = 14 : -1 : 0
        if (floor(remain/2^n) == 1)
            val(nn) = val(nn) + (1 / 2^(15-n));
            remain = remain - 2^n;
        end
    end
end
%-------------------Data output-----------------------%
output = sqrt(val);
golden = output*32768;
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_sqrt_q15.bin', 'wb');
putdata = fwrite(ofid, golden, 'short');
a=fclose(ofid);
%-------------------END-------------------------------%
