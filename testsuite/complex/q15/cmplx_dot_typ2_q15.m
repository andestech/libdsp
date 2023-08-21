clear;
clc;
datasize = 12700;
numSamples = 127;
loop = datasize/ numSamples;
%-------------------Read data-------------------------%
fid = fopen('input_cmc_q15_a.bin', 'rb');
f2id = fopen('input_cmc_q15_b.bin', 'rb');
ofid = fopen('golden_cdprod_typ2_q15.bin', 'wb');

%-------------------Data transform--------------------%
%-----------------------------------------------------%
for nn = 1 : loop
    realsum = 0;
    imagsum = 0;
    for mm = 1 : numSamples
        x0 = fread(fid, 1, 'int16');
        x1 = fread(fid, 1, 'int16');
        y0 = fread(f2id, 1, 'int16');
        y1 = fread(f2id, 1, 'int16');
        realsum += x0.*y0;
        imagsum += x0.*y1;
        realsum -= x1.*y1;
        imagsum += x1.*y0;
    end
    realsum1 = bitshift(realsum, -6);
    imagsum1 = bitshift(imagsum, -6);

    fwrite(ofid, realsum1, 'int32');
    fwrite(ofid, imagsum1, 'int32');
end

fclose(fid);
fclose(f2id);
fclose(ofid);
%-------------------END-------------------------------%
