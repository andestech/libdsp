clear;
clc;
MATRIX_ROW = 55;
MATRIX_COL = 43;
CLX_DIM = 2;
numsample = MATRIX_ROW *MATRIX_COL * CLX_DIM;

%-------------------Read data-------------------------%
fid = fopen('input_q31_a.bin', 'rb');
ofid = fopen('golden_cmat_trans_q31.bin', 'wb');
%-------------------Data transform--------------------%
k=1;
input = fread(fid, numsample, 'int');
for n = 1 : MATRIX_ROW
    for nn = 1: MATRIX_COL
        A(n,nn) = input(2 * k - 1) + input(2 * k) * i;
		k=k+1;
    end
end


C = A.';
C = C.';
for kk = 1 : (MATRIX_ROW * MATRIX_COL)
    out(2 * kk - 1) = real(C(kk));
    out(2 * kk) = imag(C(kk));
end
fwrite(ofid, out, 'int');
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(ofid);
%-------------------END-------------------------------%
