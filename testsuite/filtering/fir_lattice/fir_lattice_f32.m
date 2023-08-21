clear;
clc;
BLOCKSIZE = 1024;
STAGE = 16;
y=zeros(1, BLOCKSIZE);
x= zeros(1, BLOCKSIZE);
g = zeros(1, STAGE);
k = [0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.6];
f = zeros(1, STAGE);
f_i = zeros(1, STAGE);
g_i = zeros(1, STAGE);
%--------------------------------%
fid = fopen('input_f32.bin', 'rb');
ofid = fopen('golden_fir_lattice_f32.bin', 'wb');
%--------------------------------%
	x = fread(fid, BLOCKSIZE, 'float');
	for n = 1 : BLOCKSIZE
	  g(1)=x(n);
	  f(1)=x(n);
	  for m = 2 : STAGE+1
		f(m) = f(m-1)+k(m-1) *g_i(m-1);
		g(m) = k(m-1)*f(m-1)+g_i(m-1);
      end
	  y(n) =f(m);
	  f_i = f;
	  g_i = g;
	end
    fwrite(ofid, y, 'float');
%------------------------------------%
o2fid = fopen('golden_fir_lattice_state_f32.bin', 'wb');
fwrite(o2fid, g_i, 'float');
%------------------------------------%
fclose(fid);
fclose(ofid);
