clear;
clc;
numSamples = 4;
datasize =32768;

%-------------------Read data-------------------------%
fid = fopen('input_conj_q31.bin', 'rb');
ofid =fopen('golden_conj_q31.bin', 'wb');
for mm = 1 : datasize/(2*numSamples)
    input = fread(fid, 2*numSamples, 'int');
    for n = 1: numSamples
        output((2*n)-1) = input((2*n)-1);  %real part
        output((2*n)) = -input((2*n));  %imag part
    end
    fprintf('%f\n', output);
%-------------------Data output-----------------------%
    A=fwrite(ofid, output, 'int');
end
%-----------------------------------------------------%
fclose(fid);
fclose(ofid);
%-------------------END-------------------------------%

