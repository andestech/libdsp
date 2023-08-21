clear;
clc;
datasize = 30000;
blocksize = 15;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_sat_q15.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'short');
  for nn = 1 : blocksize
      fprintf('%d\n',input(nn));
  end
  a = (int32)(input.*input);
  b= sum(a);
  c =bitshift(b,-15);
 % if(c >= (2^15)-1)
 %   c = (2^15)-1;
 % else
 %   c = c;
 % end
  golden(n) =(c/blocksize);
  fprintf('%d\n',golden(n));
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_rms_sqrt_q15.bin', 'wb');
 fwrite(ofid, golden, 'short');
 fclose(ofid);
%-------------------END-------------------------------%
