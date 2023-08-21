clear;
clc;
sample = 4;
datasize = 32768;
valA = zeros(datasize,1);
valB = zeros(datasize/2,1);
outputdata = zeros(datasize,1);
invA = zeros(datasize,1);
%-------------------Read data-------------------------%
fid = fopen('input_mult_q31_a.bin', 'rb');
fid2 = fopen('input_mult_q31_b.bin', 'rb');
InputA = fread(fid, datasize, 'int');
InputB = fread(fid2, datasize/2, 'int');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
valA = int2q_q31(InputA, datasize);
valB = int2q_q31(InputB, datasize/2);
%-----------------------------------------------------%
for pp = 1 : datasize/2
    outputdata(2*pp-1) = valA(2*pp-1) .* valB(pp);
    outputdata(2*pp) = valA(2*pp) .* valB(pp);
end
%-----------------------------------------------------%
invA = q2int_q31(outputdata, datasize);
%-----------------------------------------------------%

golden = invA;
fprintf('%d\n', golden);
%-------------------Data output-----------------------%
ofid = fopen('golden_mult_real_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int');
a=fclose(ofid);
%-------------------END-------------------------------%
