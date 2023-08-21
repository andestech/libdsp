clear;
clc;
numSamples = 8;
datasize =131072;

%-------------------Read data-------------------------%
fid = fopen('input_cmplx_f32.bin', 'rb');
ofid =fopen('golden_conj_f32.bin', 'wb');
for mm = 1 : datasize/(2*numSamples)
    input = fread(fid, 2*numSamples, 'float');
    for n = 1: numSamples
        output((2*n)-1) = input((2*n)-1);  %real part
        output((2*n)) = -input((2*n));  %imag part
    end
    fprintf('%f\n', output);
%-------------------Data output-----------------------%
    A=fwrite(ofid, output, 'float');
end
%-----------------------------------------------------%
fclose(fid);
fclose(ofid);
%-------------------END-------------------------------%

