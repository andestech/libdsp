clear;
clc;
datasize = 7500;
numSamples = 60;
loop = datasize/ numSamples;
%-------------------Read data-------------------------%
fid = fopen('input_clx_q31_a.bin', 'rb');
f2id = fopen('input_clx_q31_b.bin', 'rb');
ofid = fopen('golden_cdprod_typ2_q31.bin', 'wb');

%-------------------Data transform--------------------%
%-----------------------------------------------------%
for nn = 1 : loop
    realsum = 0;
    imagsum = 0;
    for mm = 1 : numSamples
        x0 = fread(fid, 1, 'int32');
        x1 = fread(fid, 1, 'int32');
        y0 = fread(f2id, 1, 'int32');
        y1 = fread(f2id, 1, 'int32');
        realsum += bitshift(x0.*y0, -14);
        imagsum += bitshift(x0.*y1, -14);
        realsum -= bitshift(x1.*y1, -14);
        imagsum += bitshift(x1.*y0, -14);
    end

    fwrite(ofid, realsum, 'int64');
    fwrite(ofid, imagsum, 'int64');
end

fclose(fid);
fclose(f2id);
fclose(ofid);
%-------------------END-------------------------------%
