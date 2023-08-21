clear;
clc;
numSamples = 4;
datasize =20000;


%-------------------Read data-------------------------%
fid = fopen('input_real_a.bin', 'rb');
f2id = fopen('input_real_b.bin', 'rb');
ofid =fopen('golden_mult_real_f32.bin', 'wb');


for mm = 1 : datasize/(2*numSamples)
    inputa = fread(fid, 2*numSamples, 'float');
    inputb = fread(f2id, numSamples, 'float');
    for n = 1 : numSamples
        output(8*(mm-1)+(2*n)-1) = inputa((2*n)-1)*inputb(n);
        output(8*(mm-1)+(2*n)) = inputa(2*n)*inputb(n);
    end
end
fprintf('%f\n', output);
fwrite(ofid, output, 'float');

%-----------------------------------------------------%
fclose(fid);
fclose(f2id);
fclose(ofid);

%-------------------END-------------------------------%

