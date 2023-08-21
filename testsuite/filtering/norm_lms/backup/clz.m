function out = clz(src)
     out = src;
     if(src !=0)
        out = log2_32b(src) + 1;
        out = 32 - out;
     endif
