clear;
clc;
MATRIX_M = 100;
MATRIX_N = 100;
MATRIX_P = 100;
asamp = MATRIX_M*MATRIX_N;
bsamp = MATRIX_N*MATRIX_P;
loop = 1;

%-------------------Read data-------------------------%
fid = fopen('input_q7_a.bin', 'rb');
f2id = fopen('input_q7_b.bin', 'rb');
ofid = fopen('golden_mat_mult_q7.bin', 'wb');
%-------------------Data transform--------------------%
for m = 1: loop
  k=1;
  inputa = fread(fid, asamp, 'char');
  for n = 1 : MATRIX_M
	for nn = 1: MATRIX_N
	  A(n,nn) = inputa(k);
	  k=k+1;
	end
  end
  p=1;
  inputb = fread(f2id, bsamp, 'char');
  for n = 1 : MATRIX_N
	for nn = 1: MATRIX_P
	  B(n,nn) = inputb(p);
	  p=p+1;
	end
  end
C = (int16)(A * B);
D = bitshift(C,-7);
   for n = 1:MATRIX_M
	 for nn = 1:MATRIX_P
	   if(D(n,nn)>2^8-1)
		 D(n,nn)=2^8-1;
	   elseif(D(n,nn)<=(-2^8))
		 D(n,nn)= (-2^8);
	   else
		   D(n,nn)=D(n,nn);
	   end
	  end
	end

fwrite(ofid, D', 'char');
printf('%d %d\n',m, D);
end
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(f2id);
 fclose(ofid);
%-------------------END-------------------------------%
