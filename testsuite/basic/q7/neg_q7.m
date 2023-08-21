clear;
clc;

datasize = 256;
val = zeros(datasize,1);
%-------------------Read data-------------------------%
fid = fopen('input_q7.bin', 'rb');
Inputdata = fread(fid, datasize, 'int8');
%-------------------convert q number------------------%
for nn = 1 : datasize
    if (Inputdata(nn) == -(2^7))
        remain = (2^7)-1;
    elseif (Inputdata(nn) > 0)
        remain = Inputdata(nn);
    else
        remain = -Inputdata(nn);
    end    
    for n = 6 : -1 : 0
        if (floor(remain/2^n) == 1)
            val(nn) = val(nn) + (1 / 2^(7-n));
            remain = remain - 2^n;
        end
    end
    if (Inputdata(nn) < 0)
        val(nn) = -val(nn);
    end
end
%-------------------Data output-----------------------%
output = -(val);
golden = output*2^7;
fprintf('%d\n', golden);
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_negate_q7.bin', 'wb');
putdata = fwrite(ofid, golden, 'int8');
a=fclose(ofid);
%-------------------END-------------------------------%
