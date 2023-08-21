clear;
clc;
numSamples = 2;
datasize =65536;

%-------------------Read data-------------------------%
fid = fopen('input_conj_q15.bin', 'rb');
ofid =fopen('golden_conj_q15.bin', 'wb');
for mm = 1 : datasize/(2*numSamples)
    input = fread(fid, 2*numSamples, 'short');
    for n = 1: numSamples
        output((2*n)-1) = input((2*n)-1);  %real part
        output((2*n)) = -input((2*n));  %imag part 
    end
    fprintf('%f\n', output);
%-------------------Data output-----------------------%
    A=fwrite(ofid, output, 'short');
end
%-----------------------------------------------------%
fclose(fid);
fclose(ofid);
%-------------------END-------------------------------%

