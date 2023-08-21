clear;
clc;
BLOCKSIZE = 1023;
STAGE = 3;
y=zeros(1, STAGE);
x= zeros(1, BLOCKSIZE);
state = zeros(1, 2*STAGE);
%--------------------------------%
fid = fopen('input_f32.bin', 'rb');
ofid = fopen('golden_biquad_cascade_df2T_f32.bin', 'wb');
b = [0.1 0.2 0.3 -0.4 -0.1 0.1 0.1 0.1 -0.1 -0.1 0.1 0.1 0.1 -0.1 -0.1];
%--------------------------------%
	x = fread(fid, BLOCKSIZE, 'float');
	for n = 1: BLOCKSIZE
	  x_n = x(n);
	  for m = 1 : STAGE
	    d0 = state(1+2*(m-1));
        d1 = state(2+2*(m-1));
        y(m) = b(1+5*(m-1)) * x_n + d0;
        d0 = ((b(2+5*(m-1)) * x_n) + (b(4+5*(m-1)) * y(m))) + d1;
        d1 = ((b(3+5*(m-1)) * x_n) + (b(5+5*(m-1)) * y(m)));
        state(1+2*(m-1)) = d0;
		state(2+2*(m-1)) = d1;
        x_n = y(m);
	  end
	  y_out(n) = y(STAGE);
	end
      printf('%2.8f ', y_out);
      printf('\n');
      fwrite(ofid, y_out, 'float');
%------------------------------------%
o2fid = fopen('golden_biquad_cascade_df2T_state_f32.bin', 'wb');
fwrite(o2fid, state, 'float');
%------------------------------------%
fclose(fid);
fclose(ofid);
