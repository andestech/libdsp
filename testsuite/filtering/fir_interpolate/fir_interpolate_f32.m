clear;
clc;
L =4;
BLOCKSIZE = 1024;
TAP = 16;
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
fid = fopen('input_f32.bin', 'rb');
ofid = fopen('golden_fir_interpolate_f32.bin', 'wb');
b = [1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1]  % [coe[0], coe[1],...coe[15]]
%--------------------------------%
	x = fread(fid, BLOCKSIZE, 'float');
    state = copy(x_i, x, BLOCKSIZE, phaselength);
	fprintf('-------------\n');
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
	fprintf('%.5f ', y_out)
	fprintf('\n');
	fprintf('-------------\n');
    fwrite(ofid, y_out, 'float');
%------------------------------------%
state = copy(x_i, x, BLOCKSIZE, phaselength);
o2fid = fopen('golden_fir_interpolate_state_f32.bin', 'wb');
fwrite(o2fid, state, 'float');
%------------------------------------%
fclose(fid);
fclose(ofid);
