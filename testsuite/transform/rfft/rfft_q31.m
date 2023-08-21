clear;
clc;
fftlen = 64; % n = 2^m;
outlen = 2*fftlen;
%-----------------------------------%
fid = fopen('golden_cfft_radix4_q31.bin', 'rb');
ofid = fopen('golden_rfft_q31.bin', 'wb');
%-----------------------------------%
src = fread(fid, 2*fftlen, 'int');
p = pi/(fftlen);
i = fftlen-1;
ic = 0.5;
ia = 2*ic;
ib = 2*ic;
src_i1 = 3;
src_i2 = 2*fftlen;
out_i1 = 3;
out_i2 = 4*fftlen;
%-----------------------------------%
while(i > 0)
  coa1 = q2int_q31((0.5*(1-sin(ia*p))),1);
  coa2 = q2int_q31((0.5*(-cos(ia*p))),1);
  outr = bitshift((int64)(src(src_i1)*coa1),-32);
  outi = bitshift((int64)(src(src_i1)*coa2),-32);
  src_i1=src_i1+1;
  outr = bitshift(bitshift((int64)(outr),32)+(int64)(src(src_i1)*(-coa2)),-32);
  outi = bitshift(bitshift((int64)(outi),32)+(int64)(src(src_i1)*(coa1)),-32);
  src_i1=src_i1+1;
  outr = bitshift(bitshift((int64)(outr),32)+(int64)(src(src_i2)*(-coa2)),-32);
  cob1 = q2int_q31((0.5*(1+sin(ib*p))),1);
  outi = bitshift(bitshift((int64)(outi),32)+(int64)(src(src_i2)*(-cob1)),-32);
  src_i2=src_i2-1;
  outr = bitshift(bitshift((int64)(outr),32)+(int64)(src(src_i2)*(cob1)),-32);
  outi = bitshift(bitshift((int64)(outi),32)+(int64)(src(src_i2)*(-coa2)),-32);
  src_i2=src_i2-1;

  out(out_i1) = outr*2;
  out_i1 = out_i1 +1;
  out(out_i1) = outi*2;
  out_i1 = out_i1 +1;
  out(out_i2) = -(outi*2);
  out_i2 = out_i2-1;
  out(out_i2) = (outr*2);
  out_i2 = out_i2-1;

  ib = ib + ic*2;
  ia = ia + ic*2;
  i--;
end
out(2*fftlen+1) = src(1) - src(2);
out(2*fftlen+2) = src(1) + src(2);
out(1) = src(1) + src(2);
out(2) = src(1) - src(2);
%-----------------------------------%
fwrite(ofid, out, 'int');
fclose(fid);
fclose(ofid);
rifft_q31;
cifft_radix4_rfft_q31;
