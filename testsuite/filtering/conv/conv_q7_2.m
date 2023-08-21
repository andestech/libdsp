clear;
clc;
SRCALEN = 3;
SRCBLEN = 16;
SRCCLEN = SRCALEN + SRCBLEN - 1;
loop = 100;
%-------------------------------------------%
fid = fopen('input_q7_a_2.bin', 'rb');
f2id = fopen('input_q7_b.bin', 'rb');
ofid = fopen('golden_conv_q7_2.bin', 'wb');

for n = 1 : loop
  A = fread(fid, SRCALEN, 'int8');
  B = fread(f2id, SRCBLEN, 'int8');
  C = conv(A, B);
  D = bitshift(C,-7);
  if(D >= 2^7-1)
	E = 2^7-1;
  elseif(D < -2^7)
    E = -2^7;
  else
	E = D;
  end
  fwrite(ofid, E, 'int8');
end

%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);
