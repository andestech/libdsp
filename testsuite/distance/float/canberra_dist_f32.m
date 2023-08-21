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
    out = 0;
    for m = 1 : SIZE
        diff = abs(src1(m) - src2(m));
        sum = abs(src1(m)) + abs(src2(m));
        if(src1(m) != 0.0)
            if(src2(m) != 0.0)
                out += (diff / sum);
            end
        end
    end
    golden(n) = out;
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_canberra_dist_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
