clear;
clc;
datasize = 5000;
numSamples = 125;
loop = datasize / numSamples;


%-------------------Read data-------------------------%
fid = fopen('input_cmplx_f32_a.bin', 'rb');
f2id = fopen('input_cmplx_f32_b.bin', 'rb');
ofid =fopen('golden_cdprod_typ2_f32.bin', 'wb');

for nn = 1 : loop
realsum = 0;
imagsum = 0;
for mm = 1 : numSamples
    x0 = fread(fid, 1, 'float');
    x1 = fread(fid, 1, 'float');
    y0 = fread(f2id, 1, 'float');
    y1 = fread(f2id, 1, 'float');

    realsum += x0 * y0;
    imagsum += x0 * y1;
    realsum -= x1 * y1;
    imagsum += x1 * y0;

end
fwrite(ofid, realsum, 'float');
fwrite(ofid, imagsum, 'float');
end

%-----------------------------------------------------%
fclose(fid);
fclose(f2id);
fclose(ofid);

%-------------------END-------------------------------%
