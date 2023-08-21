clear;
clc;
SIZE=100;
datasize = 10000;
ORD = 2;
loop = datasize/SIZE;
golden=zeros(1, loop);

%-------------------Read data-------------------------%
fid1 = fopen('input_f32.bin', 'rb');
fid2 = fopen('input_f32_2.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
    src1 = fread(fid1, SIZE, 'float');
    src2 = fread(fid2, SIZE, 'float');
    tmp = abs(src1 - src2);
    accsum = sum(power(tmp , ORD));

    out = power(accsum, (1.0 / ORD));
    golden(n) = out;
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_minkowski_dist_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
