clear;
clc;
SRCALEN = 64;
SRCBLEN = 3;
SRCCLEN = 2 * max(SRCALEN,SRCBLEN) - 1;
loop = 100;
%-------------------------------------------%
fid = fopen('input_q7_a.bin', 'rb');
f2id = fopen('input_q7_b_2.bin', 'rb');
ofid = fopen('golden_correlate_q7_3.bin', 'wb');
if(SRCALEN > SRCBLEN)
    zerocount = SRCALEN - SRCBLEN;
end

for n = 1 : loop
  A = fread(fid, SRCALEN, 'int8');
  B = fread(f2id, SRCBLEN, 'int8');
  C = [B;zeros(zerocount,1)];
  AA = int2q_q7(A, SRCALEN);
  BB = int2q_q7(C, SRCALEN);
  CC = xcorr(AA, BB);  % Octave do not support xcorr func.
  D =   q2int_q7(CC, SRCCLEN);
  fwrite(ofid, D, 'int8');

end

%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);
