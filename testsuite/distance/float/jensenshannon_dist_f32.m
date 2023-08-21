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
    tmp = (src1 + src2)/2;
    tmp1 = sum(src1 .* log(src1 ./ tmp));
    tmp2 = sum(src2 .* log(src2 ./ tmp));
    accsum = tmp1 + tmp2;
    golden(n) = sqrt(accsum / 2);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_jensenshannon_dist_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
