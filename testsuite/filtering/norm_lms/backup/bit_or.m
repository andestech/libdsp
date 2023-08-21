function out = bit_or(x, y)

 out= 0;
        for i =  1 : 64
           tmpc = 0;
           tmpa = mod(x, 2);
           tmpb = mod(y, 2);

           if((tmpa!=0)||(tmpb!=0))
               tmpc = 1;
           end
             tmpc = bitshift(tmpc,  i-1);
             out = out + tmpc;
             x = bitshift(x, -1);
             y = bitshift(y, -1);
        end
