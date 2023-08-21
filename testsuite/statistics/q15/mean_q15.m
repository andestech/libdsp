clear;
clc;
datasize = 13100;
blocksize = 131;
loop = datasize/blocksize;

%-------------------Read data-------------------------%
fid = fopen('input_q15.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'short');
  golden(n) = mean(input);
end
 goldenin = fix(golden)
  %fprintf('%f\n',golden);
  %fprintf('%f\n',goldenin);
%-------------------Data output-----------------------%
 ofid = fopen('golden_mean_q15.bin', 'wb');
 fwrite(ofid, goldenin, 'short');
 fclose(ofid);
%-------------------END-------------------------------%
