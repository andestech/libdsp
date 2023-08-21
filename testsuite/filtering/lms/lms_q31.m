clear;
clc;
BLOCKSIZE = 16;
NUMSTAP = 15;
MU = 2^-6;
loopsize =250;
numdata =1;
statesize = BLOCKSIZE + NUMSTAP -1
scale = 16;   %postshift=2 2^2;
b = zeros(1, BLOCKSIZE);
b_n = zeros(1, BLOCKSIZE);
b_s = zeros(1, BLOCKSIZE);
y_n =0;
y = zeros(1, BLOCKSIZE);
x = zeros(1, BLOCKSIZE);
x0 = zeros(1, NUMSTAP-1);
%--------------------------------%
fid = fopen('input_q31.bin', 'rb');
f2id = fopen('input_ref_q31.bin', 'rb');
ofid = fopen('golden_lms_q31.bin', 'wb');
o2fid = fopen('golden_err_q31.bin', 'wb');
%--------------------------------%
x_d = fread(fid, BLOCKSIZE, 'int'); %in
d_d = fread(f2id, BLOCKSIZE, 'int'); %d(n)
x = int2q_q31(x_d, BLOCKSIZE);
d = int2q_q31(d_d, BLOCKSIZE);
%---------------------------------%
for loop = 1 : loopsize
  state = copy(x0,x,BLOCKSIZE, NUMSTAP);
  fprintf('%f\n', state);
  fprintf('-------------\n', state);
  for nn = 1 : BLOCKSIZE
	y_i =0;
	for m =  1 : NUMSTAP
	  y_n = y_i + scale*b(m) * state(nn + NUMSTAP - m);
	  y_i=y_n;
	end
	e(nn) = d(nn) - y_i;
	for k = 1 : NUMSTAP
	  b_n(k) = b(k) + e(nn) * MU * state(nn + NUMSTAP - k);
	end
	b = b_n;
	y(nn) = y_i;
  end
  x0 = state(BLOCKSIZE+1 :statesize);
  yy = q2int_q31(y, BLOCKSIZE);
  ee = q2int_q31(e, BLOCKSIZE);
  fwrite(ofid, yy, 'int');
  fprintf('%d out:', loop);fprintf('%d ',yy);
  fprintf('\n');
  fwrite(o2fid, ee, 'int');
  fprintf('%d err:', loop);fprintf('%d ',ee);
  fprintf('\n');
end
%-----------------------------------%
fclose(fid);
fclose(ofid);
fclose(f2id);
fclose(o2fid);
