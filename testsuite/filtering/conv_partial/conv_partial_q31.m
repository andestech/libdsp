clear;
clc;
SRCALEN = 64;
SRCBLEN = 15;
SRCCLEN= SRCALEN+SRCBLEN-1;
STARTINDEX = 5;
SIZE = 35;
loop = 100;
%-------------------------------------------%
fid = fopen('input_q31_a.bin', 'rb');
f2id = fopen('input_q31_b.bin', 'rb');
ofid = fopen('golden_conv_partial_q31.bin', 'wb');

for n = 1 : loop
  %initialize
  for i = 1 : SRCCLEN
      A(i) = 0;
      B(i) = 0;
      C(i) = 0;
  end
  %add testcases to improve code coverage 100%.
  if(n == 97)
     SRCBLEN = 14;
  elseif(n == 98)
     SRCALEN = 4;
     SRCBLEN = 3;
     STARTINDEX = 0;
     SIZE = 5;
  elseif(n == 99)
     SRCALEN = 14;
     SRCBLEN = 15;
     STARTINDEX = 5;
     SIZE = 10;
  elseif(n == 100)
     STARTINDEX = 14;
  endif
  A = fread(fid, SRCALEN, 'int');
  B = fread(f2id, SRCBLEN, 'int');
  for i = STARTINDEX : (STARTINDEX + SIZE -1)
      sum = 0.0;
      for j = 0 : i
         if(((i - j) < SRCBLEN)&&((j < SRCALEN)))
             sum = sum + A(j+1) * B(i-j+1);
         endif
      end
      C(i+1) = sum;
  end
  C = bitshift(C, -31);
if(n == 100)
  printf('%2.8f ', C);
endif
%  printf('\n');
  fwrite(ofid, C, 'int');
end
%--------------------------------------------%

fclose(fid);
fclose(f2id);
fclose(ofid);
