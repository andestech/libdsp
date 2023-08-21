clear;
clc;
datasize = 65536;
%datasize = 4;
valA = zeros(datasize,1);
valB = zeros(datasize,1);
%-------------------Read data-------------------------%
fid = fopen('input_cmc_q15_a.bin', 'rb');
fid2 = fopen('input_cmc_q15_b.bin', 'rb');
valA = fread(fid, datasize, 'int16');
valB = fread(fid2, datasize, 'int16');

fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
%-----------------------------------------------------%
for p = 1:datasize/2
  a = (valA(2*p-1)*valB(2*p-1));
  b = (valA(2*p)*valB(2*p));
  tmp =a+b;
  golden(2*p-1) = bitshift(int32(tmp),-17);
  c =  (valA(2*p-1)*valB(2*p));
  d =  (valA(2*p)*valB(2*p-1));
  e =  c-d;
  golden(2*p) = bitshift(int32(e),-17);
end

%-----------------------------------------------------%

%-----------------------------------------------------%
fprintf('%d\n', golden);
%-------------------Data output-----------------------%
ofid = fopen('golden_cmplx_dot_q15.bin', 'wb');
putdata = fwrite(ofid, golden, 'int16');
fclose(ofid);
%-------------------END-------------------------------%
