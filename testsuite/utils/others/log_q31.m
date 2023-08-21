clear;
clc;

%------------------- log q15-------------------------%
count = 1;
for n = 1 : 4194304 : 4294967296
    n
    inf32 = n / 2147483648;
    f32out = log(inf32);
    gof32(count) = f32out / 32;
    count = count+1
end

%-------------------log end -----------------------%
 ofid = fopen('golden_log_q31_in_f32.bin', 'wb');
 fwrite(ofid, gof32, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
