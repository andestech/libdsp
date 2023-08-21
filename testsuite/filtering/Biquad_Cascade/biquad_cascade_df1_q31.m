clear;
clc;
BLOCKSIZE = 1023;
STAGE = 3;
postshift = 4;
y=zeros(1, STAGE);
x= zeros(1, BLOCKSIZE);
state = zeros(1, 4*STAGE);
%--------------------------------%
fid = fopen('input_q31.bin', 'rb');
ofid = fopen('golden_biquad_cascade_df1_q31.bin', 'wb');
b = 16*[2^-6 2^-6 2^-6 -2^-6 -2^-4 -2^-4 2^-6 2^-6 -2^-6 -2^-6 -2^-4 2^-4 2^-6 -2^-4 -2^-4];
%--------------------------------%
	x_d = fread(fid, BLOCKSIZE, 'int');
	x = int2q_q31(x_d, BLOCKSIZE);
	for n = 1: BLOCKSIZE
	  x_n = x(n);
	  for m = 1 : STAGE
	    y(m) = b(1+5*(m-1)) * x_n + b(2+5*(m-1))*state(1+4*(m-1)) + b(3+5*(m-1)) * state(2+4*(m-1))+ b(4+5*(m-1)) * state(3+4*(m-1))+  b(5+5*(m-1)) * state(4+4*(m-1));
		state(2+4*(m-1)) = state(1+4*(m-1));
		state(1+4*(m-1)) = x_n;
		state(4+4*(m-1)) = state(3+4*(m-1));
		state(3+4*(m-1)) = y(m);
		x_n = y(m);
	  end
	  y_out(n) = y(STAGE);
	end
	  yy = q2int_q31(y_out, BLOCKSIZE);
      printf('%5d ', yy);
      printf('\n');
      fwrite(ofid, yy, 'int');
%------------------------------------%
o2fid = fopen('golden_biquad_cascade_df1_state_q31.bin', 'wb');
state_q31 = q2int_q31(state, 4*STAGE);
fwrite(o2fid, state_q31, 'int');
%------------------------------------%
fclose(fid);
fclose(ofid);
fclose(o2fid);
