clear;
clc;
MATRIX_M = 49;
MATRIX_N = 49;
MATRIX_P = 49;
asamp = MATRIX_M*MATRIX_N;
bsamp = MATRIX_N*MATRIX_P;
loop = 1;

%-------------------Read data-------------------------%
fid = fopen('input_q31_fast_a.bin', 'rb');
f2id = fopen('input_q31_fast_b.bin', 'rb');
ofid = fopen('golden_mat_mul_fast_q31.bin', 'wb');
%-------------------Data transform--------------------%
for m = 1: loop
  k=1;
  inputa = fread(fid, asamp, 'int');
  for n = 1 : MATRIX_M
	for nn = 1: MATRIX_N
	  A(n,nn) = inputa(k);
	  k=k+1;
	end
  end
  p=1;
  inputb = fread(f2id, bsamp, 'int');
  for n = 1 : MATRIX_N
	for nn = 1: MATRIX_P
	  B(n,nn) = inputb(p);
	  p=p+1;
	end
end
C = (int64)(A * B);
D = (int32)(bitshift(C,-31));
fwrite(ofid, D', 'int');
end
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(f2id);
 fclose(ofid);
%-------------------END-------------------------------%
