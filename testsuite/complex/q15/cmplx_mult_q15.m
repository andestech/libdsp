clear;
clc;
datasize = 65536;
valA = zeros(datasize,1);
valB = zeros(datasize,1);
%-------------------Read data-------------------------%
fid = fopen('input_cmc_q15_a.bin', 'rb');
fid2 = fopen('input_cmc_q15_b.bin', 'rb');
valA = fread(fid, datasize, 'short');
valB = fread(fid2, datasize, 'short');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
inputa = int2q_q15(valA, datasize);
inputb = int2q_q15(valB, datasize);

%-----------------------------------------------------%
for p = 1:datasize/2
    A = inputa((2*p)-1) + i*inputa(2*p);
    B = inputb((2*p)-1) + i*inputb(2*p);
    C = A .* B;
    output(2*p-1) = real(C);
    output(2*p) = imag(C);
end

%-----------------------------------------------------%
golden = q2int_q15(output, datasize);
golden =  bitshift(golden,-2);

%-----------------------------------------------------%
%-------------------Data output-----------------------%
ofid = fopen('golden_cmplx_mult_q15.bin', 'wb');
putdata = fwrite(ofid, golden, 'short');
fclose(ofid);
%-------------------END-------------------------------%
