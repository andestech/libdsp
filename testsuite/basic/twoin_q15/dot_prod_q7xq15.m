clear;
clc;
size = 15000;
blocksize = 15;
%-------------------Read data-------------------------%
fid = fopen('input_q7_a.bin', 'rb');
fid2 = fopen('input_dot_q15_a.bin', 'rb');
ofid = fopen('golden_dot_prod_q7xq15.bin', 'wb');
InputA = fread(fid, size, 'char');
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
    putdata = fwrite(ofid, outputdata, 'int64');
end
%-------------------END-------------------------------%
a=fclose(ofid);
