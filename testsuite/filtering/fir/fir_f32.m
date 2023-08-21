clear;
clc;
BLOCKSIZE = 1024;
TAP = 16;
statesize = BLOCKSIZE+TAP-1;
datasize = BLOCKSIZE;
y=zeros(1, BLOCKSIZE);
x= zeros(1, BLOCKSIZE);
x_i= zeros(1, TAP-1);
%--------------------------------%
fid = fopen('input_f32.bin', 'rb');
ofid = fopen('golden_fir_f32.bin', 'wb');
b = [0.4 0.4 0.4 0.4  0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4 0.4];% [coe[0], coe[1],...coe[15]]
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
%      printf('%2.8f ', y_i);
%      printf('\n');
end
x_i = state(BLOCKSIZE+1 :statesize);
fwrite(ofid, y, 'float');
%------------------------------------%
state = copy(x_i, x, BLOCKSIZE, TAP);
o2fid = fopen('golden_fir_state_f32.bin', 'wb');
fwrite(o2fid, state, 'float');
%------------------------------------%
fclose(fid);
fclose(ofid);
