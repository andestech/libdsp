clear;
clc;
numofvec = 1250;
dimofvec = 8;
totalsize = numofvec * dimofvec;

golden = zeros(1, dimofvec);

%-------------------Read data-------------------------%
fid = fopen('input_f32.bin', 'rb');
fid2 = fopen('input_f32_2.bin', 'rb');
%-------------------Data transform--------------------%

weight = fread(fid2, numofvec, 'float');
accum = sum(weight);

for n = 1 : numofvec
    tmpin = fread(fid, dimofvec, 'float');
  for m = 1 : dimofvec
    golden(m) = golden(m) + tmpin(m) * weight(n);
  end
end

golden = golden / accum;

%-------------------Data output-----------------------%
 ofid = fopen('golden_barycenter_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
