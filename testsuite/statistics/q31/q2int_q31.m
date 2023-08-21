function out = q2int_q31(input, datasize)
out = zeros(datasize,1);
for nn = 1 : datasize
    if(input(nn) == -2^31)
        remain = 2^31-1
    elseif (input(nn) > 0)
        remain = input(nn);
    else
        remain = -input(nn);
    end
    for n = 1 : 31
        if (remain-(1/2^n) >= 0)
            out(nn) = out(nn) + 2^(31-n);
            remain = remain - (1/2^n);
        end
    end
    if(input(nn) < 0)
        out(nn) = -out(nn);
    end
end
