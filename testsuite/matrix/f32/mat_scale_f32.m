clear;
clc;
MATRIX_ROW = 100;
MATRIX_COL = 100;
SCALE = 5;
numsample = MATRIX_ROW *MATRIX_COL;
datasize = 10000;
loop = datasize/numsample;

%-------------------Read data-------------------------%
fid = fopen('input_f32_a.bin', 'rb');
ofid = fopen('golden_mat_scale_f32.bin', 'wb');
%-------------------Data transform--------------------%
for m = 1: loop
  k=1;
input = fread(fid, numsample, 'single');
for n = 1 : MATRIX_ROW
    for nn = 1: MATRIX_COL
        A(n,nn) = input(k);
		k=k+1;
    end
end


C = A.*SCALE;
fwrite(ofid, C', 'single');
end
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(ofid);
%-------------------END-------------------------------%