clear;
clc;
BLOCKSIZE = 16;
NUMSTAP = 16;
MU = 0.5;
loopsize = 64;
statesize = BLOCKSIZE + NUMSTAP -1;
b = zeros(1, BLOCKSIZE);
b_n = zeros(1, BLOCKSIZE);
y = zeros(1, BLOCKSIZE);
x = zeros(1, BLOCKSIZE);
x0 = zeros(1, NUMSTAP-1);
%--------------------------------%
fid = fopen('input_f32.bin', 'rb');
f2id = fopen('input_ref_f32.bin', 'rb');
ofid = fopen('golden_lms_norm_f32.bin', 'wb');
o2fid = fopen('golden_err_norm_f32.bin', 'wb');
%--------------------------------%
x = fread(fid, BLOCKSIZE, 'float');
d = fread(f2id, BLOCKSIZE, 'float');
%------------------------------------%
for loop = 1 : loopsize
  state = copy(x0,x,BLOCKSIZE, NUMSTAP);
  fprintf('%f\n', state);
  fprintf('-------------\n', state);
  for nn = 1 : BLOCKSIZE
	y_i =0;
	E=0;
	for m =  1 : NUMSTAP
	  y_n = y_i + b(m) * state(nn + NUMSTAP - m);
	  y_i=y_n;
	end
	e(nn) = d(nn) - y_i;
    for l = 1 : NUMSTAP
	  E_n = E + state(nn + NUMSTAP - l)*state(nn + NUMSTAP - l);
	  E=E_n;
    end
	for k = 1 : NUMSTAP
	  b_n(k) = b(k) + e(nn) * (MU / E) * state(nn + NUMSTAP - k);
	end
	b = b_n;
	y(nn) = y_i;
  end
  x0 = state(BLOCKSIZE+1 :statesize);
  fwrite(ofid, y, 'float');
  fprintf('%d out:', loop);fprintf('%.10f ',y);
  fprintf('\n');
  fwrite(o2fid, e, 'float');
  fprintf('%d err:', loop);fprintf('%.10f ',e);
  fprintf('\n');
end
%-----------------------------------%
fclose(fid);
fclose(ofid);
fclose(f2id);
fclose(o2fid);
