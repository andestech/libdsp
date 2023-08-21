clear;
clc;
BLOCKSIZE = 1024;
TAP = 16;
maxdelay = 3;
statesize = BLOCKSIZE+maxdelay-1;
y=zeros(1, BLOCKSIZE);
tapdelay = [3 3 3 3 2 2 2 2 0 0 0 0 1 1 1 1];
x= zeros(1, BLOCKSIZE);
x_i= zeros(1, TAP-1);
state = zeros(1, TAP);
%--------------------------------%
fid = fopen('input_f32.bin', 'rb');
ofid = fopen('golden_fir_sparse_f32.bin', 'wb');
b = [0.1 0.1 0.1 0.1 0.2 0.2 0.2 0.2 0.3 0.3 0.3 0.3 0.4 0.4 0.4 0.4];         % [coe[0], coe[2],...coe[3]]
%--------------------------------%
	x = fread(fid, BLOCKSIZE, 'float');
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
	fprintf('%.4f ', y);
	fprintf('\n');
    fwrite(ofid, y, 'float');
%------------------------------------%
fclose(fid);
fclose(ofid);
