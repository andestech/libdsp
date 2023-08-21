clear;
clc;
fftlen = 64; % n = 2^m;
outlen = 2*fftlen;
%-----------------------------------%
fid = fopen('golden_cfft_radix4_q15.bin', 'rb');
ofid = fopen('golden_rfft_q15.bin', 'wb');
%-----------------------------------%
src = fread(fid, 2*fftlen, 'short');
p = pi/(fftlen);
i = 1;
ic = 0.5;
ia =2*ic;
ib=2*ic;
src_i1 = 3;
src_i2 = 2*fftlen-1;
%-----------------------------------%
while(i < fftlen)
  coa = q2int_q15((0.5*(1-sin(ia*p))),1);
  coa1 = q2int_q15((0.5*(-cos(ia*p))),1);
  cob = q2int_q15((0.5*(1+sin(ib*p))),1);
  cob1 = q2int_q15((0.5*(cos(ib*p))),1);
  outr = src(src_i1)*coa;
  outr = outr - (src(src_i1+1)*coa1);
  outr = outr + (src(src_i2)*cob);
  outr = bitshift((outr+(src(src_i2+1)*cob1)),-15);

  outi = src(src_i2)*cob1;
  outi = outi - (src(src_i2+1)*cob);
  outi = outi + (src(src_i1+1)*coa);
  outi = outi + src(src_i1)*coa1;

  src_i1 = src_i1 + 2;
  src_i2 = src_i2 - 2;

  out(2*i+1) = (int16)(outr);
  out(2*i+2) = bitshift(outi,-15);
  out((4*fftlen)-(2*i)+1) = (int16)(outr);
  out((4*fftlen)-(2*i)+2) = -bitshift(outi,-15);

  ib = ib + ic*2;
  ia = ia + ic*2;
  i++;
end
out(2*fftlen+1) = src(1) - src(2);
out(2*fftlen+2) = src(1) + src(2);
out(1) = src(1) + src(2);
out(2) = src(1) - src(2);
%-----------------------------------%
fwrite(ofid, out, 'short');
fclose(fid);
fclose(ofid);
rifft_q15;
cifft_radix4_rfft_q15;
