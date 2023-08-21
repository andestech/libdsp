clear;
clc;
fftlen = 64; % n = 2^m;
outlen = 2*fftlen;
%-----------------------------------%
fid = fopen('golden_rfft_q15.bin', 'rb');
ofid = fopen('golden_rifft_ncff_q15.bin', 'wb');
%-----------------------------------%
src = fread(fid, 2*fftlen+2, 'short');
p = pi/(fftlen);
i = fftlen;
ic = 0.5;
ia = 0;
ib = 0;
src_i1 = 1;
src_i2 = 2*fftlen+1;
out_i = 1;
%-----------------------------------%
while(i > 0)
  coa = round((0.5*(1-sin(ia*p)))*32768);
  coa1 = round((0.5*(-cos(ia*p)))*32768);
  cob = round((0.5*(1+sin(ib*p)))*32768);
  cob1 = round((0.5*(cos(ib*p)))*32768);
  outr = src(src_i2)*cob;
  outr = outr - (src(src_i2+1)*cob1);
  outr = outr + (src(src_i1)*coa);
  outr = bitshift((outr+(src(src_i1+1)*coa1)),-15);

  outi = src(src_i1+1)*coa;
  outi = outi - (src(src_i1)*coa1);
  outi = outi - (src(src_i2)*cob1);
  outi = outi - (src(src_i2+1)*cob);

  src_i1 = src_i1 + 2;
  src_i2 = src_i2 - 2;

  out(out_i) = (int16)(outr);
  out_i = out_i + 1;
  out(out_i) = bitshift(outi,-15);
  out_i = out_i + 1;

  ib = ib + ic*2;
  ia = ia + ic*2;
  i--;
end
%-----------------------------------%
fwrite(ofid, out, 'short');
fclose(fid);
fclose(ofid);
cifft_radix4_rfft_q15;
