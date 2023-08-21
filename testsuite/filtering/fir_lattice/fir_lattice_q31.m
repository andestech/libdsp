clear;
clc;
BLOCKSIZE = 1023;
STAGE =16;
y=zeros(1, BLOCKSIZE);
x= zeros(1, BLOCKSIZE);
g = zeros(1, STAGE);
k = [2^-6 2^-6 2^-6 2^-6 2^-6 2^-10  2^-6 2^-6 2^-10 2^-6 2^-6 2^-6 2^-6 2^-6 2^-6 2^-6];
f = zeros(1, STAGE);
f_i = zeros(1, STAGE);
g_i = zeros(1, STAGE);
%--------------------------------%
fid = fopen('input_q31.bin', 'rb');
ofid = fopen('golden_fir_lattice_q31.bin', 'wb');
%--------------------------------%
	x_d = fread(fid, BLOCKSIZE, 'int');
	x = int2q_q31(x_d, BLOCKSIZE);
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
	y_o = q2int_q31(y, BLOCKSIZE);
    fwrite(ofid, y_o, 'int');
%------------------------------------%
o2fid = fopen('golden_fir_lattice_state_q31.bin', 'wb');
g_q31 = q2int_q31(g_i, STAGE);
fwrite(o2fid, g_q31, 'int');
%------------------------------------%
fclose(fid);
fclose(ofid);
