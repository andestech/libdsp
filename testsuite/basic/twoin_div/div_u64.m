clear;
clc;
datasize = 1000;
valA = zeros(datasize,1);
valB = zeros(datasize,1);

%-------------------Read data-------------------------%
pd = fopen('input_u64_u32.bin', 'rb');
py = fopen('input_u64.bin', 'rb');
ofid = fopen('golden_div_u64.bin', 'wb');
Inputy = fread(py, datasize, 'uint64');
Inputd = fread(pd, datasize, 'uint32');
%-------------------Data transform--------------------%
    %fprintf('%lu\n', int64(Inputy));
    %fprintf('-------------\n');
    %fprintf('%lu\n', Inputd);
for nn = 1 : datasize
    remain = Inputy(nn);
    for n = 64 : -1 : 0
        if (floor(remain/2^n) == 1)
            valA(nn) = valA(nn) + (1 / 2^(64-n));
            remain = remain - 2^n;
        end
    end
end

for nn = 1 : datasize
    remain = Inputd(nn);
    for n = 32 : -1 : 0
        if (floor(remain/2^n) == 1)
            valB(nn) = valB(nn) + (1 / 2^(32-n));
            remain = remain - 2^n;
        end
    end
end
%-----------------------------------------------------%
    outputdata = valA ./ valB;
    golden = outputdata*2^32;
    %fprintf('-------------\n');
    %fprintf('%f\n', golden);
    putdata = fwrite(ofid, golden, 'int32');

%-------------------END-------------------------------%
a=fclose(ofid);
