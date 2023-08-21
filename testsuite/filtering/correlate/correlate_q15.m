clear;
clc;
SRCALEN = 16;
SRCBLEN = 10;
SRCCLEN = 2 * max(SRCALEN,SRCBLEN) - 1;
loop = 1;
%-------------------------------------------%
fid = fopen('input_q15_a.bin', 'rb');
f2id = fopen('input_q15_b.bin', 'rb');
ofid = fopen('golden_correlate_q15.bin', 'wb');
if(SRCALEN > SRCBLEN)
    zerocount = SRCALEN - SRCBLEN;
end

for n = 1 : loop
  A = fread(fid, SRCALEN, 'short');
  B = fread(f2id, SRCBLEN, 'short');
  C = [B;zeros(zerocount,1)];
  AA = int2q_q15(A, SRCALEN);
  BB = int2q_q15(C, SRCALEN);
  CC = xcorr(AA, BB);  % Octave do not support xcorr func.
  D =   q2int_q15(CC, SRCCLEN);
  fwrite(ofid, D, 'short');

end

%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);
