clear;
clc;
MATRIX_M = 50;
MATRIX_N = 50;
MATRIX_P = 50;
format long
asamp = MATRIX_M*MATRIX_N * 2;
bsamp = MATRIX_N*MATRIX_P * 2;
osamp = MATRIX_M*MATRIX_P * 2;

%-------------------Read data-------------------------%
fid = fopen('input_f32_testa.bin', 'rb');
f2id = fopen('input_f32_test.bin', 'rb');
ofid = fopen('golden_cmat_mul_f32.bin', 'wb');
%-------------------Data transform--------------------%
  k=1;
  inputa = fread(fid, asamp, 'single');
  for n = 1 : MATRIX_M
	for nn = 1: MATRIX_N
	  A(n,nn) = inputa(2*k-1) + inputa(2*k)*i;
	  k=k+1;
	end
  end
  p=1;
  inputb = fread(f2id, bsamp, 'single');
  for n = 1 : MATRIX_N
	for nn = 1: MATRIX_P
	  B(n,nn) = inputb(2*p-1) + inputb(2*p)*i;
	  p=p+1;
	end
  end
  C = (A*B)

    p = 1;
for n = 1:MATRIX_M
    for nn = 1:MATRIX_P
        output(2*p-1) = real(C(n,nn));
        output(2*p) = imag(C(n,nn));
        p=p+1;
    end
end

fwrite(ofid, output, 'single');
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(f2id);
 fclose(ofid);
%-------------------END-------------------------------%
