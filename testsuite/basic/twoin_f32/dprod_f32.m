clear;
clc;
blocksize = 8;
%-------------------Read data-------------------------%
fid = fopen('input_dot_f32_a.bin', 'rb');
fid2 = fopen('input_dot_f32_b.bin', 'rb');
ofid = fopen('golden_dot_prod_f32.bin', 'wb');
InputA = fread(fid, 65536, 'single');
InputB = fread(fid2, 65536, 'single');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
for m = 1 : blocksize : 800
    for n = 1 : blocksize
        InputAvec(n) = InputA(n+m-1);
        InputBvec(n) = InputB(n+m-1);
    end
    outputdata = dot(InputAvec,InputBvec);
    fprintf('%f\n', outputdata);
    putdata = fwrite(ofid, outputdata, 'single');
end
%-------------------END-------------------------------%
a=fclose(ofid);
