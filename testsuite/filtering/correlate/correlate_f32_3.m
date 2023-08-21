clear;
clc;
SRCALEN = 64;
SRCBLEN = 3;
SRCCLEN = 2 * max(SRCALEN, SRCBLEN) - 1;
loop = 10;
%-------------------------------------------%
fid = fopen('input_f32_a.bin', 'rb');
f2id = fopen('input_f32_b_2.bin', 'rb');
ofid = fopen('golden_correlate_f32_3.bin', 'wb');

if(SRCALEN > SRCBLEN)
    zerocount = SRCALEN - SRCBLEN;
end
for n = 1 : loop
  A = fread(fid, SRCALEN, 'single');
  B = fread(f2id, SRCBLEN, 'single');
  BB = [B;zeros(zerocount,1)];
  C = xcorr(A, BB);  % Octave do not support xcorr func.
  fprintf('%2.8f ', C);
  fprintf('\n');
  fwrite(ofid, C, 'single');
end

%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);
