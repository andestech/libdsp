clear;
clc;
SRCALEN = 64;
SRCBLEN = 15;
SRCCLEN = SRCALEN + SRCBLEN - 1;
STARTINDEX = 5;
SIZE = 35;
loop = 100;
%-------------------------------------------%
fid = fopen('input_q7_a.bin', 'rb');
f2id = fopen('input_q7_b.bin', 'rb');
ofid = fopen('golden_conv_partial_q7.bin', 'wb');

for n = 1 : loop
  %initialize
    for i = 1 : SRCCLEN
        A(i) = 0;
        B(i) = 0;
        C(i) = 0;
    end
    if(n == 98)
        SRCALEN = 3;
        SRCBLEN = 3;
        SIZE = 2;
        STARTINDEX = 1;
    elseif(n == 99)
        SRCALEN = 14;
        SRCBLEN = 16;
        SIZE = 10;
        STARTINDEX = 5;
    elseif(n == 100)
        STARTINDEX = 14;
    endif
  A = fread(fid, SRCALEN, 'int8');
  B = fread(f2id, SRCBLEN, 'int8');
  for i = STARTINDEX : (STARTINDEX + SIZE -1)
      sum = 0.0;
      for j = 0 : i
         if(((i - j) < SRCBLEN)&&((j < SRCALEN)))
             sum = sum + A(j+1) * B(i-j+1);
         endif
      end
      C(i+1) = sum;
  end
  D = bitshift(C,-7);
  %saturation
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
