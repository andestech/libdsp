clear;
clc;
MATRIX_M = 49;
MATRIX_N = 37;
MATRIX_P = 1;
asamp = MATRIX_M*MATRIX_N;
bsamp = MATRIX_N*MATRIX_P;
loop = 1;

%-------------------Read data-------------------------%
fid = fopen('input_q15_fast_a.bin', 'rb');
f2id = fopen('input_q15_fast_b.bin', 'rb');
ofid = fopen('golden_mat_mul_mxv_q15.bin', 'wb');
%-------------------Data transform--------------------%
for m = 1: loop
  k=1;
  inputa = fread(fid, asamp, 'short');
  for n = 1 : MATRIX_M
	for nn = 1: MATRIX_N
	  A(n,nn) = inputa(k);
	  k=k+1;
	end
  end
  p=1;
  inputb = fread(f2id, bsamp, 'short');
  for n = 1 : MATRIX_N
	for nn = 1: MATRIX_P
	  B(n,nn) = inputb(p);
	  p=p+1;
	end
  end
C = (int64)(A * B);
D = bitshift(C,-15);
   for n = 1:MATRIX_M
	 for nn = 1:MATRIX_P
	   if(D(n,nn)>2^15-1)
		 D(n,nn)=2^15-1;
	   elseif(D(n,nn)<=(-2^15))
		 D(n,nn)= (-2^15);
	   else
		   D(n,nn)=D(n,nn);
	   end
	  end
	end

fwrite(ofid, D', 'short');
printf('%d %d\n',m, D);
end
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(f2id);
 fclose(ofid);
%-------------------END-------------------------------%
