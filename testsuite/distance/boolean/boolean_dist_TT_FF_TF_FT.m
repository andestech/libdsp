function [tt, ff, tf, ft] = boolean_dist_TT_FF_TF_FT(src1, src2, numofbool)

tmptt=0;
tmpff=0;
tmptf=0;
tmpft=0;
idx=1;
for n = numofbool : -32: 32
    tmp1 = src1(idx);
    tmp2 = src2(idx);
    shift = 0;
    for m = 1 : 1 : 32
        tmp3 = bitand(tmp1, 1);
        tmp4 = bitand(tmp2, 1);
        tmp1 = bitshift(tmp1, -1);
        tmp2 = bitshift(tmp2, -1);
        tmptt = tmptt + and(tmp3, tmp4);
        tmpff = tmpff + and(xor(1,tmp3), xor(1,tmp4));
        tmptf = tmptf + and(tmp3,xor(1,tmp4));
        tmpft = tmpft + and(xor(1,tmp3), tmp4);
    end
    idx = idx+1;
end
res = n - 32;
shf = 32 - res;


tmp1 = src1(idx);
tmp2 = src2(idx);
tmp1 = bitshift(tmp1, -shf);
tmp2 = bitshift(tmp2, -shf);
idx = idx+1;
for n = res : -1: 1
        tmp3 = bitand(tmp1, 1);
        tmp4 = bitand(tmp2, 1);
        tmp1 = bitshift(tmp1, -1);
        tmp2 = bitshift(tmp2, -1);
        tmptt = tmptt + and(tmp3, tmp4);
        tmpff = tmpff + and(xor(1,tmp3), xor(1,tmp4));
        tmptf = tmptf + and(tmp3,xor(1,tmp4));
        tmpft = tmpft + and(xor(1,tmp3), tmp4);
end
tt = tmptt;
ff = tmpff;
tf = tmptf;
ft = tmpft;
endfunction
