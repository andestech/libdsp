clear;
clc;
BLOCKSIZE = 1024;
STAGE = 3;
CHANNEL = 2;
x= zeros(1, BLOCKSIZE * CHANNEL);
y_out= zeros(1, BLOCKSIZE * CHANNEL);
state = zeros(1, 4 * STAGE);
%--------------------------------%
fid = fopen('input_stereo_f32.bin', 'rb');
ofid = fopen('golden_biquad_cascade_stereo_df2T_f32.bin', 'wb');
b = [0.1 0.2 0.3 -0.4 -0.1 0.1 0.1 0.1 -0.1 -0.1 0.1 0.1 0.1 -0.1 -0.1];
%--------------------------------%
	x = fread(fid, BLOCKSIZE * CHANNEL, 'float');
	for n = 1: STAGE
       b0 = b(1+5*(n-1));
       b1 = b(2+5*(n-1));
       b2 = b(3+5*(n-1));
       a1 = b(4+5*(n-1));
       a2 = b(5+5*(n-1));

       d00 = state(1+4*(n-1));
       d10 = state(2+4*(n-1));
	   d01 = state(3+4*(n-1));
       d11 = state(4+4*(n-1));

       j = 1;
       for m = 1: BLOCKSIZE
            xn0 = x(j);
            xn1 = x(j+1);
            acc0 = b0 * xn0 + d00;
            acc1 = b0 * xn1 + d01;
            y_out(j) = acc0;
            y_out(j+1) = acc1;
            d00 = b1 * xn0 + a1 * acc0 + d10;
            d01 = b1 * xn1 + a1 * acc1 + d11;

            d10 = b2 * xn0 + a2 * acc0;
            d11 = b2 * xn1 + a2 * acc1;

            x(j) = y_out(j);
            x(j+1) = y_out(j+1);
            j = j + 2;
        end
        state(1+4*(n-1)) = d00;
        state(2+4*(n-1)) = d10;
	    state(3+4*(n-1)) = d01;
        state(4+4*(n-1)) = d11;
    end
    printf('%2.8f ', y_out);
    printf('\n');
    fwrite(ofid, y_out, 'float');
%------------------------------------%
o2fid = fopen('golden_biquad_cascade_stereo_df2T_state_f32.bin', 'wb');
fwrite(o2fid, state, 'float');
%------------------------------------%
fclose(fid);
fclose(ofid);
