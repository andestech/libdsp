clear;
clc;
blocksize = 35;
datasize = 32760;

%-------------------Read data-------------------------%
fid = fopen('input_dot_a.bin', 'rb');
fid2 = fopen('input_dot_b.bin', 'rb');
ofid = fopen('golden_dot_prod_q31.bin', 'wb');
InputA = fread(fid, datasize, 'int');
InputB = fread(fid2, datasize, 'int');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%

%-----------------------------------------------------%
for m = 1 : blocksize : datasize
    for n = 1 : blocksize
        InputAvec(n) = InputA(n+m-1);
        InputBvec(n) = InputB(n+m-1);
    end
    outputdata = dot(InputAvec,InputBvec);
    golden = outputdata/2^14;
    fprintf('%f\n', golden);
    putdata = fwrite(ofid, golden, 'int64');
end
%-------------------END-------------------------------%
a=fclose(ofid);
