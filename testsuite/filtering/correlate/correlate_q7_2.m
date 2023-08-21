clear;
clc;
SRCALEN = 15;
SRCBLEN = 64;
SRCCLEN = 2 * max(SRCALEN,SRCBLEN) - 1;
loop = 100;
%-------------------------------------------%
fid = fopen('input_q7_b.bin', 'rb');
f2id = fopen('input_q7_a.bin', 'rb');
ofid = fopen('golden_correlate_q7_2.bin', 'wb');
if(SRCALEN > SRCBLEN)
    zerocount = SRCALEN - SRCBLEN;
else
    zerocount = 0;
end

for n = 1 : loop
  A = fread(fid, SRCALEN, 'int8');
  B = fread(f2id, SRCBLEN, 'int8');
  C = [B;zeros(zerocount,1)];
  AA = int2q_q7(A, SRCALEN);
  BB = int2q_q7(C, SRCBLEN);
  CC = xcorr(AA, BB);  % Octave do not support xcorr func.
  D =   q2int_q7(CC, SRCCLEN);
  fwrite(ofid, D, 'int8');

end

%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);
