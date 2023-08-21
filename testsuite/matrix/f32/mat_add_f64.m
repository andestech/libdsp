clear;
clc;
MATRIX_ROW = 100;
MATRIX_COL = 100;
numsample = MATRIX_ROW *MATRIX_COL;
datasize = 10000;
loop = datasize/numsample;

%-------------------Read data-------------------------%
fid = fopen('input_f64_a.bin', 'rb');
f2id = fopen('input_f64_b.bin', 'rb');
ofid = fopen('golden_mat_add_f64.bin', 'wb');
%-------------------Data transform--------------------%
for m = 1: loop
  k=1;
inputa = fread(fid, numsample, 'double');
inputb = fread(f2id, numsample, 'double');
for n = 1 : MATRIX_ROW
    for nn = 1: MATRIX_COL
        A(n,nn) = inputa(k);
        B(n,nn) = inputb(k);
		k=k+1;
    end
end


C = A + B;
fwrite(ofid, C', 'double');
end
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(f2id);
 fclose(ofid);
%-------------------END-------------------------------%
