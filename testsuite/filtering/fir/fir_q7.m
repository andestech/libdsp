clear;
clc;
BLOCKSIZE = 1023;
TAP = 15;
datasize = BLOCKSIZE;
statesize = BLOCKSIZE + TAP-1;
y=zeros(1, BLOCKSIZE);
x_i= zeros(1, TAP-1);
%--------------------------------%
fid = fopen('input_q7.bin', 'rb');
ofid = fopen('golden_fir_q7.bin', 'wb');
b = [2^-5 2^-5 2^-5 2^-5 2^-5 2^-5 2^-5 2^-5 2^-5 2^-5 2^-5 2^-5 2^-5 2^-5 2^-5];         % [coe[0], coe[1],...coe[15]]
%--------------------------------%
	x_d = fread(fid, BLOCKSIZE, 'int8');
	x = int2q_q7(x_d, BLOCKSIZE);
	for n = 1: BLOCKSIZE
	y_i =0;
      state = copy(x_i, x, BLOCKSIZE, TAP);
	  for m = 1 : TAP
	    y_n = y_i + b(m) * state(n + TAP - m);
	    y_i=y_n;
	  end
	  y(n) = y_i;
	end
	y_o = q2int_q7(y, BLOCKSIZE);
    x_i = state(BLOCKSIZE+1 :statesize);
    fwrite(ofid, y_o, 'int8');
%------------------------------------%
state = copy(x_i, x, BLOCKSIZE, TAP);
state_q7 = q2int_q7(state, statesize);
o2fid = fopen('golden_fir_state_q7.bin', 'wb');
fwrite(o2fid, state_q7, 'int8');
%------------------------------------%
fclose(fid);
fclose(ofid);
