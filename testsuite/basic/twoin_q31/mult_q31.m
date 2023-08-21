clear;
clc;
blocksize = 4;
datasize = 32768;
valA = zeros(datasize,1);
valB = zeros(datasize,1);

%-------------------Read data-------------------------%
fid = fopen('input_mult_a.bin', 'rb');
fid2 = fopen('input_mult_b.bin', 'rb');
ofid = fopen('golden_mult_q31.bin', 'wb');
InputA = fread(fid, datasize, 'int');
InputB = fread(fid2, datasize, 'int');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
for nn = 1 : datasize
    if (InputA(nn) == -2^31)
        remain = 2^31-1;
    elseif (InputA(nn) > 0)
        remain = InputA(nn);
    else
        remain = -InputA(nn);
    end
    for n = 30 : -1 : 0
        if (floor(remain/2^n) == 1)
            valA(nn) = valA(nn) + (1 / 2^(31-n));
            remain = remain - 2^n;
        end
    end
    if (InputA(nn) < 0)
        valA(nn) = -valA(nn);
    end
end
for nn = 1 : datasize
    if (InputB(nn) == -2^31)
        remain = 2^31-1;
    elseif (InputB(nn) > 0)
        remain = InputB(nn);
    else
        remain = -InputB(nn);
    end
    for n = 30 : -1 : 0
        if (floor(remain/2^n) == 1)
            valB(nn) = valB(nn) + (1 / 2^(31-n));
            remain = remain - 2^n;
        end
    end
    if (InputB(nn) < 0)
        valB(nn) = -valB(nn);
    end
end
%-----------------------------------------------------%
    outputdata = valA .* valB;
    golden = outputdata*2^31;
    fprintf('%f\n', golden);
    putdata = fwrite(ofid, golden, 'int32');

%-------------------END-------------------------------%
a=fclose(ofid);
