clear;
clc;
BLOCKSIZE = 12;
NUMSTAP = 5;
MU = 2^-5;
loopsize =250;
numdata =1;
statesize = BLOCKSIZE + NUMSTAP -1
scale = 4;   %postshift=2 2^2;
b = zeros(1, BLOCKSIZE);
b_n = zeros(1, BLOCKSIZE);
b_s = zeros(1, BLOCKSIZE);
y_n =0;
y = zeros(1, BLOCKSIZE);
x = zeros(1, BLOCKSIZE);
x0 = zeros(1, NUMSTAP-1);
%--------------------------------%
fid = fopen('input_q15.bin', 'rb');
f2id = fopen('input_ref_q15.bin', 'rb');
ofid = fopen('golden_lms_q15.bin', 'wb');
o2fid = fopen('golden_err_q15.bin', 'wb');
%--------------------------------%
x_d = fread(fid, BLOCKSIZE, 'short'); %in
d_d = fread(f2id, BLOCKSIZE, 'short'); %d(n)
x = int2q_q15(x_d, BLOCKSIZE);
d = int2q_q15(d_d, BLOCKSIZE);
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
  yy = q2int_q15(y, BLOCKSIZE);
  ee = q2int_q15(e, BLOCKSIZE);
  fwrite(ofid, yy, 'short');
  fprintf('%d out:', loop);fprintf('%d ',yy);
  fprintf('\n');
  fwrite(o2fid, ee, 'short');
  fprintf('%d err:', loop);fprintf('%d ',ee);
  fprintf('\n');
end
%-----------------------------------%
fclose(fid);
fclose(ofid);
fclose(f2id);
fclose(o2fid);
