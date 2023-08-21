clear;
clc;
sample = 4;
datasize = 14998;
valA = zeros(datasize,1);
valB = zeros(datasize,1);
invA = zeros(datasize,1);
%-------------------Read data-------------------------%
fid = fopen('input_clx_q31_a.bin', 'rb');
fid2 = fopen('input_clx_q31_b.bin', 'rb');
InputA = fread(fid, datasize, 'int');
InputB = fread(fid2, datasize, 'int');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
valA = int2q_q31(InputA, datasize);
valB = int2q_q31(InputB, datasize);
%-----------------------------------------------------%
for pp = 1 : datasize/2
    outputdata(2*pp-1) = valA(2*pp-1) .* valB(2*pp-1)-valA(2*pp)*valB(2*pp);
    outputdata(2*pp) = valA(2*pp-1) .* valB(2*pp)+valA(2*pp) .* valB(2*pp-1);
end
%-----------------------------------------------------%
invA = q2int_q31(outputdata, datasize);
%-----------------------------------------------------%

golden = invA;
fprintf('%d\n', golden);
%-------------------Data output-----------------------%
ofid = fopen('golden_mult_cmplx_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int');
a=fclose(ofid);
%-------------------END-------------------------------%
