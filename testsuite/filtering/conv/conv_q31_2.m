clear;
clc;
SRCALEN = 15;
SRCBLEN = 64;
SRCCLEN = SRCALEN + SRCBLEN - 1;
loop = 100;
%-------------------------------------------%
fid = fopen('input_q31_b.bin', 'rb');
f2id = fopen('input_q31_a.bin', 'rb');
ofid = fopen('golden_conv_q31_2.bin', 'wb');

for n = 1 : loop
  A = fread(fid, SRCALEN, 'int');
  B = fread(f2id, SRCBLEN, 'int');
  C = conv(A, B);
  D = bitshift(C,-31);
  fwrite(ofid, D, 'int');
end

%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);
