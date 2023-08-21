clear;
clc;
SIZE=100;
datasize = 10000;
loop = datasize/SIZE;
golden=zeros(1, loop);

%-------------------Read data-------------------------%
fid1 = fopen('input_f32.bin', 'rb');
fid2 = fopen('input_f32_2.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
    src1 = fread(fid1, SIZE, 'float');
    src2 = fread(fid2, SIZE, 'float');
    mean1 = mean(src1);
    mean2 = mean(src2);
    src1 = src1 - mean1;
    src2 = src2 - mean2;
    pwr1 = sum(src1 .* src1);
    pwr2 = sum(src2 .* src2);
    dotprod = dot(src1, src2);
    dotprod = dotprod / SIZE;
    pwr1 = pwr1 / SIZE;
    pwr2 = pwr2 / SIZE;
    sq = sqrt(pwr1 .* pwr2);
    golden(n) = 1.0 - dotprod / sq;
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_corr_dist_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
