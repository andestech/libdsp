clear;
clc;
datasize = 256;
valA = zeros(datasize,1);
valB = zeros(datasize,1);
invA = zeros(datasize,1);

%-------------------Read data-------------------------%
fid = fopen('input_mult_q7_a.bin', 'rb');
fid2 = fopen('input_mult_q7_b.bin', 'rb');
InputA = fread(fid, datasize, 'int8');
InputB = fread(fid2, datasize, 'int8');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
for nn = 1 : datasize
    if (InputA(nn) == -2^7)
        remain = 127;
    elseif (InputA(nn) > 0)
        remain = InputA(nn);
    else
        remain = -InputA(nn);
    end
    for n = 6 : -1 : 0
        if (floor(remain/2^n) == 1)
            valA(nn) = valA(nn) + (1 / 2^(7-n));
            remain = remain - 2^n;
        end
    end
    if (InputA(nn) < 0)
        valA(nn) = -valA(nn);
    end
end
for nn = 1 : datasize
    if (InputB(nn) == -2^7)
        remain = 127;
    elseif (InputB(nn) > 0)
        remain = InputB(nn);
    else
        remain = -InputB(nn);
    end
    for n = 6 : -1 : 0
        if (floor(remain/2^n) == 1)
            valB(nn) = valB(nn) + (1 / 2^(7-n));
            remain = remain - 2^n;
        end
    end
    if (InputB(nn) < 0)
        valB(nn) = -valB(nn);
    end
end
%-----------------------------------------------------%
outputdata = valA .* valB;
%-----------------------------------------------------%

for nn = 1 : datasize
    if(outputdata(nn) == -128)
        remain = 127;
    elseif (outputdata(nn) > 0)
        remain = outputdata(nn);
    else
        remain = -outputdata(nn);
    end
    for n = 1 : 7
        if (remain-(1/2^n) >= 0)
            invA(nn) = invA(nn) + 2^(7-n);
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
ofid = fopen('golden_mult_q7.bin', 'wb');
putdata = fwrite(ofid, golden, 'int8');
a=fclose(ofid);
%-------------------END-------------------------------%
