clear;
clc;
fftlen = 64; % n = 2^m;
outlen = 2*fftlen;
%-----------------------------------%
fid = fopen('golden_rfft_f32.bin', 'rb');
ofid = fopen('golden_rifft_ncff_f32.bin', 'wb');
%-----------------------------------%
src = fread(fid, 2*fftlen+2, 'float');
p = pi/(fftlen);
ic = 0.5;
ia =0;
ib=0;
src_i1 = 1;
src_i2 = 2*fftlen+2;
out_i1 = 1;
%-----------------------------------%
while(fftlen > 0)
  coa1 = 0.5*(1-sin(ia*p));
  coa2 = 0.5*(-cos(ia*p));
  outr = src(src_i1)*coa1;
  outi = -src(src_i1)*coa2;
  src_i1=src_i1+1;
  outr = outr + ((src(src_i1)+src(src_i2))*coa2);
  outi = outi + src(src_i1)*coa1;
  src_i1=src_i1+1;
  cob1 = 0.5*(1+sin(ib*p));
  outi = outi - src(src_i2)*cob1;
  src_i2=src_i2-1;
  outr = outr + src(src_i2)*cob1;
  outi = outi+src(src_i2)*coa2;
  src_i2=src_i2-1;
  out(out_i1) = outr;
  out_i1 = out_i1+1;
  out(out_i1) = outi;
  out_i1 = out_i1+1;
  ib = ib + ic*2;
  ia = ia + ic*2;
  fftlen--;
end
%-----------------------------------%
fwrite(ofid, out, 'float');
fclose(fid);
fclose(ofid);
cifft_radix4_rfft_f32;
