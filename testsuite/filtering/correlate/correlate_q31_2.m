clear;
clc;
SRCALEN = 15;
SRCBLEN = 64;
SRCCLEN = 2 * max(SRCALEN,SRCBLEN) - 1;
A = zeros(1,SRCALEN);
B = zeros(1,SRCBLEN);
loop = 100;
%-------------------------------------------%
fid = fopen('input_q31_b.bin', 'rb');
f2id = fopen('input_q31_a.bin', 'rb');
ofid = fopen('golden_correlate_q31_2.bin', 'wb');
if(SRCALEN > SRCBLEN)
    zerocount = SRCALEN - SRCBLEN;
else
    zerocount = 0;
end

for n = 1 : loop
  A = fread(fid, SRCALEN, 'int');
  B = fread(f2id, SRCBLEN, 'int');
  C = [B;zeros(zerocount,1)];
  AA = int2q_q31(A, SRCALEN);
  BB = int2q_q31(C, SRCBLEN);
  CC = xcorr(AA, BB);  % Octave do not support xcorr func.
  D =   q2int_q31(CC, SRCCLEN);
  fwrite(ofid, D, 'int32');
end

%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);