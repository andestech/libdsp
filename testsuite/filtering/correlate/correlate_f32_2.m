clear;
clc;
SRCALEN = 15;
SRCBLEN = 64;
SRCCLEN = 2 * max(SRCALEN, SRCBLEN) - 1;
loop = 10;
%-------------------------------------------%
fid = fopen('input_f32_b.bin', 'rb');
f2id = fopen('input_f32_a.bin', 'rb');
ofid = fopen('golden_correlate_f32_2.bin', 'wb');

for n = 1 : loop
  A = fread(fid, SRCALEN, 'single');
  B = fread(f2id, SRCBLEN, 'single');
  C = xcorr(A, B);  % Octave do not support xcorr func.
  fprintf('%2.8f ', C);
  fprintf('\n');
  fwrite(ofid, C, 'single');
end

%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);
