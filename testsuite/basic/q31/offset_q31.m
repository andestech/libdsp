clear;
clc;
val = zeros(32768,1);
offset = -22;
offsetval = 0;
%-------------------Read data-------------------------%
fid = fopen('input_q31.bin', 'rb');
Inputdata = fread(fid, 32768, 'int');
%-------------------convert q number------------------%
offsetval = int2q_q31(offset, 1);

for nn = 1 : 32768
    if (Inputdata(nn) == -(2^31))
        remain = (2^31)-1;
    elseif (Inputdata(nn) > 0)
        remain = Inputdata(nn);
    else
        remain = -Inputdata(nn);
    end    
    for n = 30 : -1 : 0
        if (floor(remain/2^n) == 1)
            val(nn) = val(nn) + (1 / 2^(31-n));
            remain = remain - 2^n;
        end
    end
    if ((Inputdata(nn) < 0) && (Inputdata(nn) ~= -2^31))
        val(nn) = -val(nn);
    elseif (Inputdata(nn) == -2^31)
        val(nn) = -1;
    end
end
%-------------------Data output-----------------------%
output = val + offsetval;
golden = output*2^31;
fprintf('%d\n', golden);
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_offset_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int');
a=fclose(ofid);
%-------------------END-------------------------------%

