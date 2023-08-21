clear;
clc;
SRCALEN = 64;
SRCBLEN = 3;
SRCCLEN = 2 * max(SRCALEN,SRCBLEN) - 1;
A = zeros(1,SRCALEN);
B = zeros(1,SRCBLEN);
loop = 100;
%-------------------------------------------%
fid = fopen('input_q31_a.bin', 'rb');
f2id = fopen('input_q31_b_2.bin', 'rb');
ofid = fopen('golden_correlate_q31_3.bin', 'wb');
if(SRCALEN > SRCBLEN)
    zerocount = SRCALEN - SRCBLEN;
end

for n = 1 : loop
  A = fread(fid, SRCALEN, 'int');
  B = fread(f2id, SRCBLEN, 'int');
  C = [B;zeros(zerocount,1)];
  AA = int2q_q31(A, SRCALEN);
  BB = int2q_q31(C, SRCBLEN);
  CC = xcorr(AA, BB);  % Octave do not support xcorr func.
  D =   q2int_q31(CC, SRCCLEN);
  fwrite(ofid, D, 'int');
end

%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);

