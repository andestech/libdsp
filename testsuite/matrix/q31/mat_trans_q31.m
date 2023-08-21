clear;
clc;
MATRIX_ROW = 99;
MATRIX_COL = 99;
numsample = MATRIX_ROW *MATRIX_COL;
datasize = 10000;
loop = datasize/numsample;

%-------------------Read data-------------------------%
fid = fopen('input_q31_a.bin', 'rb');
ofid = fopen('golden_mat_trans_q31.bin', 'wb');
%-------------------Data transform--------------------%
for m = 1: loop
  k=1;
input = fread(fid, numsample, 'int');
for n = 1 : MATRIX_ROW
    for nn = 1: MATRIX_COL
        A(n,nn) = input(k);
		k=k+1;
    end
end


C = A';
fwrite(ofid, C', 'int');
end
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(ofid);
%-------------------END-------------------------------%
