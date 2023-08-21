function fout = int2q_q31(input, datasize)

fout = zeros(datasize,1);
for nn = 1 : datasize
    if (input(nn) == -2^31)
        remain = 2^31-1;
    elseif (input(nn) > 0)
        remain = input(nn);
    else
        remain = -input(nn);
    end
    for n = 30 : -1 : 0
        if (floor(remain/2^n) == 1)
            fout(nn) = fout(nn) + (1 / 2^(31-n));
            remain = remain - 2^n;
        end
    end
    if (input(nn) < 0)
        fout(nn) = -fout(nn);
    end
end
