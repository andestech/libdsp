clear;
clc;
MATRIX_M = 100;
MATRIX_N = 100;
MATRIX_P = 100;
datasize = 10000;
asamp = MATRIX_M*MATRIX_N;
bsamp = MATRIX_N*MATRIX_P;
loop = datasize/asamp;

%-------------------Read data-------------------------%
fid = fopen('input_f64_a.bin', 'rb');
f2id = fopen('input_f64_a.bin', 'rb');
ofid = fopen('golden_mat_pow2_cache_f64.bin', 'wb');
%-------------------Data transform--------------------%
for m = 1: loop
  k=1;
  inputa = fread(fid, asamp, 'double');
  for n = 1 : MATRIX_M
	for nn = 1: MATRIX_N
	  A(n,nn) = inputa(k);
	  k=k+1;
	end
  end
  p=1;
  inputb = fread(f2id, bsamp, 'double');
  for n = 1 : MATRIX_N
	for nn = 1: MATRIX_P
	  B(n,nn) = inputb(p);
	  p=p+1;
	end
  end


C = (A * B);
fwrite(ofid, C', 'double');
end
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(f2id);
 fclose(ofid);
%-------------------END-------------------------------%
