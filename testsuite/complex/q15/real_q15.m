clear;
clc;
sample = 4;
datasize = 32768;
valA = zeros(datasize,1);
valB = zeros(datasize/2,1);
invA = zeros(datasize,1);
%-------------------Read data-------------------------%
fid = fopen('input_real_q15_a.bin', 'rb');
fid2 = fopen('input_real_q15_b.bin', 'rb');
InputA = fread(fid, datasize, 'short');
InputB = fread(fid2, datasize/2, 'short');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
for nn = 1 : datasize
    if (InputA(nn) == -32768)
        remain = 32767;
    elseif (InputA(nn) > 0)
        remain = InputA(nn);
    else
        remain = -InputA(nn);
    end
    for n = 14 : -1 : 0
        if (floor(remain/2^n) == 1)
            valA(nn) = valA(nn) + (1 / 2^(15-n));
            remain = remain - 2^n;
        end
    end
    if (InputA(nn) < 0)
        valA(nn) = -valA(nn);
    end
end
for nn = 1 : datasize/2
    if (InputB(nn) == -32768)
        remain = 32767;
    elseif (InputB(nn) > 0)
        remain = InputB(nn);
    else
        remain = -InputB(nn);
    end
    for n = 14 : -1 : 0
        if (floor(remain/2^n) == 1)
            valB(nn) = valB(nn) + (1 / 2^(15-n));
            remain = remain - 2^n;
        end
    end
    if (InputB(nn) < 0)
        valB(nn) = -valB(nn);
    end
end
%-----------------------------------------------------%
for pp = 1 : datasize/2
    outputdata(2*pp-1) = valA(2*pp-1) .* valB(pp);
    outputdata(2*pp) = valA(2*pp) .* valB(pp);
end
%-----------------------------------------------------%

for nn = 1 : datasize
    if(outputdata(nn) == -32768)
        remain = 32767;
    elseif (outputdata(nn) > 0)
        remain = outputdata(nn);
    else
        remain = -outputdata(nn);
    end
    for n = 1 : 15
        if (remain-(1/2^n) >= 0)
            invA(nn) = invA(nn) + 2^(15-n);
            remain = remain - (1/2^n);
        end
    end
    if(outputdata(nn) < 0)
        invA(nn) = -invA(nn)-1;
    end
end

%-----------------------------------------------------%

golden = invA;
fprintf('%d\n', golden);
%-------------------Data output-----------------------%
ofid = fopen('golden_real_q15.bin', 'wb');
putdata = fwrite(ofid, golden, 'short');
a=fclose(ofid);
%-------------------END-------------------------------%