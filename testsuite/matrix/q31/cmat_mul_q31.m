clear;
clc;
MATRIX_M = 18;
MATRIX_N = 38;
MATRIX_P = 21;
format long
asamp = MATRIX_M*MATRIX_N * 2;
bsamp = MATRIX_N*MATRIX_P * 2;
osamp = MATRIX_M*MATRIX_P * 2;

%-------------------Read data-------------------------%
fid = fopen('input_q31_a_1.bin', 'rb');
f2id = fopen('input_q31_b_1.bin', 'rb');
ofid = fopen('golden_cmat_mul_q31.bin', 'wb');
%-------------------Data transform--------------------%
  k=1;
  inputa = fread(fid, asamp, 'int');
  for n = 1 : MATRIX_M
	for nn = 1: MATRIX_N
	  A(n,nn) = inputa(2*k-1) + inputa(2*k)*i;
	  k=k+1;
	end
  end
  p=1;
  inputb = fread(f2id, bsamp, 'int');
  for n = 1 : MATRIX_N
	for nn = 1: MATRIX_P
	  B(n,nn) = inputb(2*p-1) + inputb(2*p)*i;
	  p=p+1;
	end
  end
  A = A/(2^31);
  B = B/(2^31);
  C = (A*B);
  D = floor(C * (2^31));

   for n = 1:MATRIX_M
	 for nn = 1:MATRIX_P
	   if(real(D(n,nn))>2^32-1)
            D(n,nn) = (2^32-1)+imag(D(n,nn))*i;
	   elseif(imag(D(n,nn))>2^32-1)
            D(n,nn) = real(D(n,nn)) + (2^32-1) * i;
	   elseif(real(D(n,nn))<=(-2^32))
            D(n,nn) = (-2^32)+imag(D(n,nn))*i;
	   elseif(imag(D(n,nn))<=(-2^32))
            D(n,nn) = real(D(n,nn)) + (-2^32) *i;
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

fwrite(ofid, output, 'int');
%-------------------Data output-----------------------%
 fclose(fid);
 fclose(f2id);
 fclose(ofid);
%-------------------END-------------------------------%
