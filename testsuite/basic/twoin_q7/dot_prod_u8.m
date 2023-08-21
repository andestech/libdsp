clear;
clc;
blocksize = 15;
datasize = 255;
%-------------------Read data-------------------------%
fid = fopen('input_u8_a.bin', 'rb');
fid2 = fopen('input_u8_b.bin', 'rb');
ofid = fopen('golden_dot_prod_u8.bin', 'wb');
InputA = fread(fid, datasize, 'uint8');
InputB = fread(fid2, datasize, 'uint8');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
for m = 1 : blocksize : datasize
    for n = 1 : blocksize
        InputAvec(n) = InputA(n+m-1);
        InputBvec(n) = InputB(n+m-1);
    end
    outputdata = dot(InputAvec,InputBvec);
    fprintf('%f\n', outputdata);
    putdata = fwrite(ofid, outputdata, 'uint32');
end
%-------------------END-------------------------------%
a=fclose(ofid);
