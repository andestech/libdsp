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
    tmp = src1 - src2;
    acc = sum(tmp .* tmp);
    out = sqrt(acc);
    golden(n) = out;
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_euclidean_dist_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
