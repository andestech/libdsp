clear;
clc;
BLOCKSIZE = 16;
NUMSTAGE = 16; %c code + 1
loop = 64;
%--------------------------------%
%input signal must be scaled down by 2*log2(numStages) bits%
fid = fopen('input_q31.bin', 'rb');
ofid = fopen('golden_iir_q31.bin', 'wb');
g = zeros(1, NUMSTAGE);         % pState = g(n)
g_i = zeros(1, NUMSTAGE+BLOCKSIZE-1); % initpState = g_i
k = [0.0625 0.125 0.03125 0.015625 0.0078125 0.0625 0.125 0.03125 0.015625 0.0078125 0.0625 0.125 0.03125 0.015625 0.0078125]; % pkCoeffs = k(n)
v = [0.125 0.03125 0.015625 0.0078125 0.0625 0.125 0.03125 0.015625 0.0078125 0.0625 0.125 0.03125 0.015625 0.0078125 0.0625 0.125]; % pvCoeffs = v(n)
k = fliplr(k);
v = fliplr(v);
%--------------------------------%
for nn = 1 : loop
    for n = 1 : BLOCKSIZE
        x_n = fread(fid, 1, 'int');
        xq_n = int2q_q31(x_n, 1);
        yq(n) = 0;
        f(NUMSTAGE) = xq_n;
        j = NUMSTAGE;
        while (j > 1)
            f(j-1) = f(j) - k(j-1) * g_i(j-1);
            j = j - 1;
        end

        g(1) = f(1);
        for j = 1 : NUMSTAGE -1
            g(j + 1) = k(j) * f(j) + g_i(j);
        end

        for j = 1 : NUMSTAGE
            yq(n) = yq(n) + v(j) * g(j);
        end
   g_i = g;
  end
  y=q2int_q31(yq, BLOCKSIZE);
  printf('%16d ', y);
  printf('\n');
  fwrite(ofid, y, 'int');
end
%------------------------------------%
fclose(fid);
fclose(ofid);
