clear;
clc;
MATRIX_ROW = 99;
MATRIX_COL = 99;
SCALEFRACT = 100;
SHIFT = 4;
numsample = MATRIX_ROW *MATRIX_COL;
datasize = 10000;
loop = datasize/numsample;

%-------------------Read data-------------------------%
fid = fopen('input_q15_a.bin', 'rb');
ofid = fopen('golden_mat_scale_q15.bin', 'wb');
%-------------------Data transform--------------------%
for m = 1: loop
  k=1;
input = fread(fid, numsample, 'short');
for n = 1 : MATRIX_ROW
    for nn = 1: MATRIX_COL
        A(n,nn) = input(k);
		k=k+1;
    end
end
B = (int32)(A*SCALEFRACT);
C = bitshift(B,SHIFT-15);
fwrite(ofid, C', 'short');
end
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(ofid);
%-------------------END-------------------------------%
