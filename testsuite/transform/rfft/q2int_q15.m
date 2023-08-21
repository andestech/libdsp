function out = q2int_q15(input, datasize)
out = zeros(datasize,1);
for nn = 1 : datasize
    if(input(nn) > 0)
        remain = input(nn);
		if(input(nn)>1)
		  remain = 1;
		end
    else
        remain = -input(nn);
		if(input(nn)<-1)
		  remain = 1;
		end
    end
    for n = 1 : 15
        if (remain-(1/2^n) >= 0)
            out(nn) = out(nn) + 2^(15-n);
            remain = remain - (1/2^n);
        end
    end
	if(input(nn)<0)
	  if(input(nn) <= -1)
		out(nn) = -2^15;
	  else
		out(nn) = -out(nn);
	  end
	end
end
