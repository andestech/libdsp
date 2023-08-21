clear;
clc;
MATRIX_ROW = 99;
MATRIX_COL = 99;
SCALEFRACT = 0x2000000;
SHIFT = 14;
numsample = MATRIX_ROW *MATRIX_COL;
datasize = 10000;
loop = datasize/numsample;

%-------------------Read data-------------------------%
fid = fopen('input_q31_a.bin', 'rb');
ofid = fopen('golden_mat_scale_q31.bin', 'wb');
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
B = bitshift(((int64)(A*SCALEFRACT)),-32);
C = bitshift(B, SHIFT+1);
d = bitshift(C, -(SHIFT+1));
if (B ~= d)
  C = bitshift(B,-31);
end
fwrite(ofid, C', 'int');
end
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(ofid);
%-------------------END-------------------------------%
