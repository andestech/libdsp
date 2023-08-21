clear;
clc;
blocksize = 34;
datasize = 100;
golden = zeros(blocksize*datasize,1);
%-------------------Read data-------------------------%
fid = fopen('input_q15_1.bin', 'rb');
%-------------------convert q number------------------%
k = 1;
for nn = 1 : datasize
    input = fread(fid, 1, 'short');
    for mm = 1 : blocksize
        golden(k) = input;
        k = k+1;
    end
end
%-------------------Data output-----------------------%
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_set_q15.bin', 'wb');
putdata = fwrite(ofid, golden, 'short');
a=fclose(ofid);
%-------------------END-------------------------------%
