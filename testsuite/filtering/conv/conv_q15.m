clear;
clc;
SRCALEN = 12;
SRCBLEN = 5;
SRCCLEN = SRCALEN + SRCBLEN - 1;
loop = 1;
%-------------------------------------------%
fid = fopen('input_q15_a.bin', 'rb');
f2id = fopen('input_q15_b.bin', 'rb');
ofid = fopen('golden_conv_q15.bin', 'wb');

for n = 1 : loop
  A = fread(fid, SRCALEN, 'short');
  B = fread(f2id, SRCBLEN, 'short');
  C = conv(A, B);
  D = bitshift(C,-15);
  if(D >= 2^15-1)
	E = 2^15-1;
  elseif(D < -2^15)
    E = -2^15;
  else
	E = D;
  end
  fwrite(ofid, E, 'short');
end

%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);
