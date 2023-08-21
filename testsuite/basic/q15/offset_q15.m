clear;
clc;
val = zeros(65536,1);
offset = -5;
offsetval = 0;
%-------------------Read data-------------------------%
fid = fopen('input_q15-1.bin', 'rb');
Inputdata = fread(fid, 65536, 'short');
%-------------------convert q number------------------%
offsetval = int2q_q15(offset, 1);
for nn = 1 : 65536
    if (Inputdata(nn) == -32768)
        remain = 32767;
    elseif (Inputdata(nn) > 0)
        remain = Inputdata(nn);
    else
        remain = -Inputdata(nn);
    end    
    for n = 14 : -1 : 0
        if (floor(remain/2^n) == 1)
            val(nn) = val(nn) + (1 / 2^(15-n));
            remain = remain - 2^n;
        end
    end
    if ((Inputdata(nn) < 0) && (Inputdata(nn) ~= -32768))
        val(nn) = -val(nn);
    elseif (Inputdata(nn) == -32768)
        val(nn) = -1;        
    end
end

%-------------------Data output-----------------------%
output = val + offsetval;
golden = output*32768;
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_offset_q15.bin', 'wb');
putdata = fwrite(ofid, golden, 'short');
a=fclose(ofid);
%-------------------END-------------------------------%
