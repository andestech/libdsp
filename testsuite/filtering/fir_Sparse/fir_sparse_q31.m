clear;
clc;
BLOCKSIZE = 1023;
TAP = 16;
maxdelay = 5;
statesize = BLOCKSIZE+maxdelay-1;
y=zeros(1, BLOCKSIZE);
tapdelay = [3 3 3 3 2 2 2 2 0 0 0 0 1 1 1 1];
x= zeros(1, BLOCKSIZE);
x_i= zeros(1, TAP-1);
state = zeros(1, TAP);
%--------------------------------%
fid = fopen('input_q31.bin', 'rb');
ofid = fopen('golden_fir_sparse_q31.bin', 'wb');
b = [2^-5 2^-5 2^-5 2^-5 2^-4 2^-4 2^-4 2^-4 2^-6 2^-6 2^-6 2^-6 2^-2 2^-2 2^-2 2^-2];         % [coe[0], coe[1],...coe[15]]
%--------------------------------%
	x_d = fread(fid, BLOCKSIZE, 'int');
	x = int2q_q31(x_d, BLOCKSIZE);
	for n = 1: BLOCKSIZE
	y_i =0;
      state = copy(x_i, x(n), TAP);
	  for m = 1 : TAP
	    y_n = y_i + b(m) * state(tapdelay(m)+1);
	    y_i=y_n;
	  end
	  y(n) = y_i;
      x_i = state(1 : TAP-1);
	end
	y_o = q2int_q31(y, BLOCKSIZE);
	fprintf('%5d ', y_o);
	fprintf('\n');
    fwrite(ofid, y_o, 'int');
%------------------------------------%
fclose(fid);
fclose(ofid);
