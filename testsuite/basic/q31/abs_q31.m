clear;
clc;
val = zeros(32768,1);
%-------------------Read data-------------------------%
fid = fopen('input_q31.bin', 'rb');
Inputdata = fread(fid, 32768, 'int');
%-------------------convert q number------------------%
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
    if (Inputdata(nn) < 0)
        val(nn) = -val(nn);
    end
end
%-------------------Data output-----------------------%
output = abs(val);
golden = output*2^31;
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_abs_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int');
a=fclose(ofid);
%-------------------END-------------------------------%
testfid = fopen('golden_abs_q31.bin', 'rb');
test = fread(testfid, 32768, 'int');
fprintf('%d\n' , test);
a=fclose(testfid);
