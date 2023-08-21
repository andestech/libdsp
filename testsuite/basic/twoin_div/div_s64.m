clear;
clc;
datasize = 1000;
valA = zeros(datasize,1);
valB = zeros(datasize,1);

%-------------------Read data-------------------------%
pd = fopen('input_q63_u32.bin', 'rb');
py = fopen('input_q63.bin', 'rb');
ofid = fopen('golden_div_s64.bin', 'wb');
Inputy = fread(py, datasize, 'int64');
Inputd = fread(pd, datasize, 'uint32');
%-------------------Data transform--------------------%
    %fprintf('%lu\n', int64(Inputy));
    %fprintf('-------------\n');
    %fprintf('%lu\n', Inputd);
for nn = 1 : datasize
    if (Inputy(nn) == -2^63)
        remain = 2^63-1;
    elseif (Inputy(nn) > 0)
        remain = Inputy(nn);
    else
        remain = -Inputy(nn);
    end
    for n = 62 : -1 : 0
        if (floor(remain/2^n) == 1)
            valA(nn) = valA(nn) + (1 / 2^(63-n));
            remain = remain - 2^n;
        end
    end
    if (Inputy(nn) < 0)
        valA(nn) = -valA(nn);
    end
end

for nn = 1 : datasize
    remain = Inputd(nn);
    for n = 31 : -1 : 0
        if (floor(remain/2^n) == 1)
            valB(nn) = valB(nn) + (1 / 2^(32-n));
            remain = remain - 2^n;
        end
    end
    if (Inputd(nn) < 0)
        valB(nn) = -valB(nn);
    end
end
%-----------------------------------------------------%
    outputdata = valA ./ valB;
    golden = outputdata*2^31;
    %fprintf('-------------\n');
    %fprintf('%f\n', golden);
    putdata = fwrite(ofid, golden, 'int32');

%-------------------END-------------------------------%
a=fclose(ofid);
