clear;
clc;
BLOCKSIZE = 1024;
TAP = 15;
M =2;
statesize = BLOCKSIZE+TAP-1;
datasize = BLOCKSIZE;
y=zeros(1, BLOCKSIZE);
x= zeros(1, BLOCKSIZE);
x_i= zeros(1, TAP-1);
in = zeros(1, BLOCKSIZE);
out=zeros(1, BLOCKSIZE/M);
%--------------------------------%
fid = fopen('input_f32.bin', 'rb');
ofid = fopen('golden_fir_decimate_f32.bin', 'wb');
b = [0.4 0.4 0.4 0.3 0.3 0.3 0.3 0.2 0.2 0.2 0.2 0.1 0.1 0.1 0.1];% [coe[0], coe[1],...coe[15]]
%--------------------------------%
	x = fread(fid, BLOCKSIZE, 'float');
	for n = 1: BLOCKSIZE
	y_i =0;
      state = copy(x_i, x, BLOCKSIZE, TAP);
	  for m = 1 : TAP
	    y_n = y_i + b(m) * state(n + TAP - m);
	    y_i=y_n;
	  end
	  y(n) = y_i;
	end
%      printf('%2.8f ', y);
%      printf('\n');
    x_i = state(BLOCKSIZE+1 :statesize);
%------------------------------------%
    for mm = 1 : BLOCKSIZE/M
		out(mm) = y(mm*M-M+1);
	end
%	printf('%2.8f ', out);
%	printf('\n');
	fwrite(ofid, out, 'float');
%------------------------------------%
state = copy(x_i, x, BLOCKSIZE, TAP);
o2fid = fopen('golden_fir_decimate_state_f32.bin', 'wb');
fwrite(o2fid, state, 'float');
%------------------------------------%
fclose(fid);
fclose(ofid);
