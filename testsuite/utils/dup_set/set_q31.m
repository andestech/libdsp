clear;
clc;
blocksize = 35;
datasize = 10;
golden = zeros(blocksize*datasize,1);
%-------------------Read data-------------------------%
fid = fopen('input_q31_1.bin', 'rb');
%-------------------convert q number------------------%
k = 1;
for nn = 1 : datasize
    input = fread(fid, 1, 'int');
    for mm = 1 : blocksize
        golden(k) = input;
        k = k+1;
    end
end
%-------------------Data output-----------------------%
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_set_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int');
a=fclose(ofid);
%-------------------END-------------------------------%
