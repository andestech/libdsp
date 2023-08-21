clear;
clc;
datasize = 170;
blocksize = 17;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_rms_q31.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'int');
  a = (int64)(input.*input);
  b = (int64)(sum(a));
  c =(int32)(bitshift(b,-31));
  if(c >= (2^30)-1)
	c = (2^30)-1;
  else
	c = c;
  end
  golden(n) =(int32)(c /blocksize);
  fprintf('%d\n',golden(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_rms_sqrt_q31.bin', 'wb');
 fwrite(ofid, golden, 'int32');
 fclose(ofid);
%-------------------END-------------------------------%

datasize2 = 10;

%-------------------Read data-------------------------%
fid = fopen('golden_rms_sqrt_q31.bin', 'rb');
Inputdata = fread(fid, datasize, 'int');
%-------------------convert q number------------------%
val = int2q_q31(Inputdata,datasize2);
%-------------------Data output-----------------------%
output = sqrt(val);
golden = q2int_q31(output,datasize2);
fprintf('%d\n', golden);
a=fclose(fid);
%-----------------------------------------------------%
ofid = fopen('golden_rms_q31.bin', 'wb');
putdata = fwrite(ofid, golden, 'int32');
a=fclose(ofid);
%-------------------END-------------------------------%
