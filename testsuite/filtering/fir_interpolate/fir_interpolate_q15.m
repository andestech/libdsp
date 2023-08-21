clear;
clc;
L =3;
BLOCKSIZE = 1024;
TAP = 15;
phaselength = TAP/L;
statesize = BLOCKSIZE + phaselength - 1;
datasize = BLOCKSIZE;
y=zeros(1, BLOCKSIZE);
x= zeros(1, BLOCKSIZE);
x_i= zeros(1, phaselength-1);
y_i=zeros(1,phaselength);
y_temp = zeros(1,phaselength);
y_out = zeros(1, L*BLOCKSIZE);
%--------------------------------%
fid = fopen('input_q15.bin', 'rb');
ofid = fopen('golden_fir_interpolate_q15.bin', 'wb');
b = [2^-4 2^-4 2^-4 2^-4 2^-4 2^-4 2^-4 2^-4 2^-4 2^-4 2^-4 2^-4 2^-4 2^-4 2^-4];         % [coe[0], coe[1],...coe[3]]
%--------------------------------%
	x_d = fread(fid, BLOCKSIZE, 'short');
	x = int2q_q15(x_d, BLOCKSIZE);
    state = copy(x_i, x, BLOCKSIZE, phaselength);
	for n = 1: BLOCKSIZE
	  for m = 1 : L
		y_i(m)=0;
		for k = 1 : phaselength
	     y_temp(m) = y_i(m) + b(L*(k-1)+m) * state(n + phaselength - k);
		 y_i(m)=y_temp(m);
		end
		y_out((n-1)*L+m)=y_i(m);
	  end
	end
    x_i = state(BLOCKSIZE+1 :statesize);
	y_o = q2int_q15(y_out, L*BLOCKSIZE);
	fprintf('%d ', y_o)
	fprintf('\n');
	fprintf('-------------\n');
    fwrite(ofid, y_o, 'short');
%------------------------------------%
state = copy(x_i, x, BLOCKSIZE, phaselength);
state_q15 = q2int_q15(state, statesize);
o2fid = fopen('golden_fir_interpolate_state_q15.bin', 'wb');
fwrite(o2fid, state_q15, 'short');
%------------------------------------%
fclose(fid);
fclose(ofid);
