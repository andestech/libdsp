clear;
clc;
MATRIX_M = 99;
MATRIX_N = 87;
MATRIX_P = 1;
asamp = MATRIX_M*MATRIX_N;
bsamp = MATRIX_N*MATRIX_P;
loop = 1;

%-------------------Read data-------------------------%
fid = fopen('input_f32_testa.bin', 'rb');
f2id = fopen('input_f32_test.bin', 'rb');
ofid = fopen('golden_mat_mul_mxv_f32.bin', 'wb');
%-------------------Data transform--------------------%
for m = 1: loop
  k=1;
  inputa = fread(fid, asamp, 'single');
  for n = 1 : MATRIX_M
	for nn = 1: MATRIX_N
	  A(n,nn) = inputa(k);
	  k=k+1;
	end
  end
  p=1;
  inputb = fread(f2id, bsamp, 'single');
  for n = 1 : MATRIX_N
	for nn = 1: MATRIX_P
	  B(n,nn) = inputb(p);
	  p=p+1;
	end
  end

C = (A * B);
fwrite(ofid, C', 'single');
end
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(f2id);
 fclose(ofid);
%-------------------END-------------------------------%
