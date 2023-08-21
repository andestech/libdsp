clear;
clc;
SRCALEN = 30;
SRCBLEN = 20;
SRCCLEN= SRCALEN+SRCBLEN-1;
STARTINDEX = 5;
SIZE = 17;
loop = 7;
%-------------------------------------------%
fid = fopen('input_q15_a.bin', 'rb');
f2id = fopen('input_q15_b.bin', 'rb');
ofid = fopen('golden_conv_partial_q15.bin', 'wb');

for n = 1 : loop
  %initialize
  for i = 1 : SRCCLEN
      A(i) = 0;
      B(i) = 0;
      C(i) = 0;
      D(i) = 0;
      E(i) = 0;
  end
  if(n == 2)
     SRCALEN = 14;
     SRCBLEN = 15;
     STARTINDEX = 5;
     SIZE = 10;
  elseif(n==3)
     SRCALEN = 20;
     SRCBLEN = 15;
     STARTINDEX = 5;
     SIZE = 20;
  elseif(n==4)
     SRCALEN = 4;
     SRCBLEN = 3;
     STARTINDEX = 0;
     SIZE = 5;
  elseif(n==5)
     SRCALEN = 4;
     SRCBLEN = 2;
     STARTINDEX = 2;
     SIZE = 2;
  elseif(n==6)
     SRCALEN = 20;
     SRCBLEN = 13;
     STARTINDEX = 10;
     SIZE = 20;
  elseif(n==7)
     SRCALEN = 20;
     SRCBLEN = 14;
     STARTINDEX = 10;
     SIZE = 20;
  endif
  A = fread(fid, SRCALEN, 'short');
  B = fread(f2id, SRCBLEN, 'short');
  for i = STARTINDEX : (STARTINDEX + SIZE -1)
      sum = 0.0;
      for j = 0 : i
         if(((i - j) < SRCBLEN)&&((j < SRCALEN)))
             sum = sum + A(j+1) * B(i-j+1);
         endif
      end
      C(i+1) = sum;
  end
  D = bitshift(C, -15);
  if(D >= 2^15-1)
	E = 2^15-1;
  elseif(D < -2^15)
    E = -2^15;
  else
	E = D;
  end

  printf('%d ', E);
  fwrite(ofid, E, 'short');
end
%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);
