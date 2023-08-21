clear;
clc;
val = zeros(32768,1);
%-------------------Read data-------------------------%
fid = fopen('input_sqrt_q31.bin', 'rb');
Inputdata = fread(fid, 32768, 'int');
%-------------------convert q number------------------%
for nn = 1 : 32768
    remain = Inputdata(nn);
    for n = 30 : -1 : 0
        if (floor(remain/2^n) == 1)
            val(nn) = val(nn) + (1 / 2^(31-n));
            remain = remain - 2^n;
        end       
    end
end
%-------------------Data output-----------------------%
output = sqrt(val);
golden = output*2^31;
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_sqrt_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int');
a=fclose(ofid);
%-------------------END-------------------------------%
