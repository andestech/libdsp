clear;
clc;

%------------------- log q15-------------------------%
count = 1;
for n = 1 : 8 : 65536
    n
    inf32 = n / 32768;
    f32out = log(inf32);
    gof32(count) = f32out / 16;
    count = count+1
end

%-------------------log end -----------------------%
 ofid = fopen('golden_log_q15_in_f32.bin', 'wb');
 fwrite(ofid, gof32, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
