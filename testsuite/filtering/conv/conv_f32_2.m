clear;
clc;
SRCALEN = 16;
SRCBLEN = 64;
SRCCLEN = SRCALEN + SRCBLEN - 1;
loop = 10;
%-------------------------------------------%
fid = fopen('input_f32_b.bin', 'rb');
f2id = fopen('input_f32_a.bin', 'rb');
ofid = fopen('golden_conv_f32_2.bin', 'wb');

for n = 1 : loop
  A = fread(fid, SRCALEN, 'single');
  B = fread(f2id, SRCBLEN, 'single');
  C = conv(A, B);
  printf('%2.8f ', C);
  printf('\n');
  fwrite(ofid, C, 'single');
end

%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);
