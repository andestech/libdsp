clear;
clc;
fftlen = 64; % n = 2^m;
outlen = 2*fftlen;
%-----------------------------------%
fid = fopen('golden_rfft_q31.bin', 'rb');
ofid = fopen('golden_rifft_ncff_q31.bin', 'wb');
%-----------------------------------%
src = fread(fid, 2*fftlen+2, 'int');
p = pi/(fftlen);
ic = 0.5;
ia = 0;
ib = 0;
src_i1 = 1;
src_i2 = 2*fftlen+2;
out_i = 1;
%-----------------------------------%
while(fftlen > 0)
  coa1 = q2int_q31((0.5*(1-sin(ia*p))),1);
  coa2 = q2int_q31((0.5*(-cos(ia*p))),1);
  outr = bitshift((int64)(src(src_i1)*coa1),-32);
  outi = -bitshift((int64)(src(src_i1)*coa2),-32);
  src_i1=src_i1+1;
  outr = bitshift(bitshift((int64)(outr),32)+(int64)(src(src_i1)*(coa2)),-32);
  outi = bitshift(bitshift((int64)(outi),32)+(int64)(src(src_i1)*(coa1)),-32);
  src_i1=src_i1+1;
  outr = bitshift(bitshift((int64)(outr),32)+(int64)(src(src_i2)*(coa2)),-32);
  cob1 = q2int_q31((0.5*(1+sin(ib*p))),1);
  outi = bitshift(bitshift((int64)(outi),32)-(int64)(src(src_i2)*(cob1)),-32);
  src_i2=src_i2-1;
  outr = bitshift(bitshift((int64)(outr),32)+(int64)(src(src_i2)*(cob1)),-32);
  outi = bitshift(bitshift((int64)(outi),32)+(int64)(src(src_i2)*(coa2)),-32);
  src_i2=src_i2-1;

  out(out_i) = outr*2;
  out_i = out_i +1;
  out(out_i) = outi*2;
  out_i = out_i +1;

  ib = ib + ic*2;
  ia = ia + ic*2;
  fftlen--;
end
%-----------------------------------%
fwrite(ofid, out, 'int');
fclose(fid);
fclose(ofid);
cifft_radix4_rfft_q31;
