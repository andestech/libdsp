clear;
clc;
MATRIX_M = 47;
MATRIX_N = 37;
MATRIX_P = 33;
format long
asamp = MATRIX_M*MATRIX_N * 2;
bsamp = MATRIX_N*MATRIX_P * 2;
osamp = MATRIX_M*MATRIX_P * 2;

%-------------------Read data-------------------------%
fid = fopen('input_q15_a.bin', 'rb');
f2id = fopen('input_q15_b.bin', 'rb');
ofid = fopen('golden_cmat_mul_q15.bin', 'wb');
%-------------------Data transform--------------------%
  k=1;
  inputa = fread(fid, asamp, 'short');
  for n = 1 : MATRIX_M
	for nn = 1: MATRIX_N
	  A(n,nn) = inputa(2*k-1) + inputa(2*k)*i;
	  k=k+1;
	end
  end
  p=1;
  inputb = fread(f2id, bsamp, 'short');
  for n = 1 : MATRIX_N
	for nn = 1: MATRIX_P
	  B(n,nn) = inputb(2*p-1) + inputb(2*p)*i;
	  p=p+1;
	end
  end
  A = A/32768;
  B = B/32768;
  C = (A*B);
  D = floor(C * 32768);

   for n = 1:MATRIX_M
	 for nn = 1:MATRIX_P
	   if(real(D(n,nn))>2^15-1)
            D(n,nn) = (2^15-1)+imag(D(n,nn))*i;
	   elseif(imag(D(n,nn))>2^15-1)
            D(n,nn) = real(D(n,nn)) + (2^15-1) * i;
	   elseif(real(D(n,nn))<=(-2^15))
            D(n,nn) = (-2^15)+imag(D(n,nn))*i;
	   elseif(imag(D(n,nn))<=(-2^15))
            D(n,nn) = real(D(n,nn)) + (-2^15) *i;
	   else
		   D(n,nn)=D(n,nn);
	   end
	  end
	end
    p = 1;
for n = 1:MATRIX_M
    for nn = 1:MATRIX_P
        output(2*p-1) = real(D(n,nn));
        output(2*p) = imag(D(n,nn));
        p=p+1;
    end
end

fwrite(ofid, output, 'short');
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(f2id);
 fclose(ofid);
%-------------------END-------------------------------%
