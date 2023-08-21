clear;
clc;
BLOCKSIZE = 1024;
TAP = 15;
M = 2;
datasize = BLOCKSIZE;
statesize = BLOCKSIZE + TAP-1;
y=zeros(1, BLOCKSIZE);
x_i= zeros(1, TAP-1);
%--------------------------------%
fid = fopen('input_dcmfir_fast_q31.bin', 'rb');
ofid = fopen('golden_dcmfir_fast_q31.bin', 'wb');
b = [2^-2 2^-2 2^-2 2^-6 2^-6 2^-6 2^-6 2^-4 2^-4 2^-4 2^-4 2^-5 2^-5 2^-5 2^-5];         % [coe[0], coe[1],...coe[15]]
%--------------------------------%
	x_d = fread(fid, BLOCKSIZE, 'int');
	x = int2q_q31(x_d, BLOCKSIZE);
	for n = 1: BLOCKSIZE
	y_i =0;
      state = copy(x_i, x, BLOCKSIZE, TAP);
	  for m = 1 : TAP
	    y_n = y_i + b(m) * state(n + TAP - m);
	    y_i=y_n;
	  end
	  y(n) = y_i;
	end
	y_o = q2int_q31(y, BLOCKSIZE);
    printf('%8d ', y_o);
    printf('\n');
    x_i = state(BLOCKSIZE+1 :statesize);
%------------------------------------%
    for mm = 1 : BLOCKSIZE/M
		out(mm) = y_o(mm*M-M+1);
	end
	printf('%8d ', out);
	printf('\n');
    fwrite(ofid, out, 'int');
%------------------------------------%
state = copy(x_i, x, BLOCKSIZE, TAP);
state_q31 = q2int_q31(state, statesize);
o2fid = fopen('golden_dcmfir_fast_state_q31.bin', 'wb');
fwrite(o2fid, state_q31, 'int');
%------------------------------------%
fclose(fid);
fclose(ofid);
