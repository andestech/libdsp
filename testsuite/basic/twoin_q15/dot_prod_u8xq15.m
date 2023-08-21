clear;
clc;
size = 15000;
blocksize = 15;
%-------------------Read data-------------------------%
fid = fopen('input_u8.bin', 'rb');
fid2 = fopen('input_dot_q15_a.bin', 'rb');
ofid = fopen('golden_dot_prod_u8xq15.bin', 'wb');
InputA = fread(fid, size, 'uint8');
InputB = fread(fid2, size, 'short');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
for m = 1 : blocksize : size
    for n = 1 : blocksize
        InputAvec(n) = InputA(n+m-1);
        InputBvec(n) = InputB(n+m-1);
    end
    outputdata = dot(InputAvec,InputBvec);
    fprintf('%x\n', outputdata);
    putdata = fwrite(ofid, outputdata, 'int32');
end
%-------------------END-------------------------------%
a=fclose(ofid);
