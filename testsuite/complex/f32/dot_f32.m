clear;
clc;
numSamples = 4;
datasize =10000;


%-------------------Read data-------------------------%
fid = fopen('input_cmplx_f32_a.bin', 'rb');
f2id = fopen('input_cmplx_f32_b.bin', 'rb');
ofid =fopen('golden_cmplx_dot_f32.bin', 'wb');


for mm = 1 : datasize/(2*numSamples)
    inputa = fread(fid, 2*numSamples, 'float');
    inputb = fread(f2id, 2*numSamples, 'float');
    for n = 1 : numSamples
        A = inputa((2*n)-1) + i*inputa(2*n);
        B = inputb((2*n)-1) + i*inputb(2*n);
        C = dot(A, B);
        output(8*(mm-1)+(2*n)-1) = real(C);
        output(8*(mm-1)+(2*n)) = imag(C);
    end
end
fprintf('%f\n', output);
fwrite(ofid, output, 'float');

%-----------------------------------------------------%
fclose(fid);
fclose(f2id);
fclose(ofid);

%-------------------END-------------------------------%

