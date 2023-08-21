clear;
clc;
BLOCKSIZE = 63;
NUMSTAP = 63;
MU = 0.0500488281250000;  % MU = 1640
EPSILON = 0.000000119209289  % EPSILON
loopsize = 200;
statesize = BLOCKSIZE + NUMSTAP -1;
b = zeros(1, BLOCKSIZE);
b_n = zeros(1, BLOCKSIZE);
y = zeros(1, BLOCKSIZE);
x = zeros(1, BLOCKSIZE);
x0 = zeros(1, NUMSTAP-1);
E=0;
%--------------------------------%
fid = fopen('input_f32.bin', 'rb');
f2id = fopen('input_ref_f32.bin', 'rb');
ofid = fopen('golden_lms_norm_f32.bin', 'wb');
o2fid = fopen('golden_err_norm_f32.bin', 'wb');
%--------------------------------%
for loop = 1 : loopsize
  x = fread(fid, BLOCKSIZE, 'float');
  d = fread(f2id, BLOCKSIZE, 'float');
  state = copy(x0,x,BLOCKSIZE, NUMSTAP);
  %fprintf('%f\n', state);
  %fprintf('-------------\n', state);
  energy = E;
  for nn = 1 : BLOCKSIZE
	y_i =0;
    energy = energy + x(nn) * x(nn);
	for m =  1 : NUMSTAP
	  y_n = y_i + b(m) * state(m + nn - 1);
     % fprintf("pb %.3f state %.3f\n", b(m), state(m + nn - 1));
	  y_i=y_n;
	end
	y(nn) = y_i;
	tmpe = d(nn) - y_i;
    e(nn) = tmpe;
    w = (tmpe * MU) / (energy + EPSILON);
	for k = 1 : NUMSTAP
	  b_n(k) = b(k) + w * state(k + nn - 1);
	end
    w = state(nn);
    energy = energy - w * w;
	b = b_n;
  end
  x0 = state(BLOCKSIZE+1 :statesize);
  fwrite(ofid, y, 'float');
  %fprintf('%d out:', loop);fprintf('%.10f ',y);
  %fprintf('\n');
  fwrite(o2fid, e, 'float');
  %fprintf('%d err:', loop);fprintf('%.10f ',e);
  %fprintf('\n');
  E = energy;
end
%-----------------------------------%
fclose(fid);
fclose(ofid);
fclose(f2id);
fclose(o2fid);
