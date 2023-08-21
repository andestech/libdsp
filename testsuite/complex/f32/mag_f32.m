clear;
clc;
numSamples = 8;
datasize =10000;

%-------------------Read data-------------------------%
fid = fopen('input_mag_f32.bin', 'rb');
ofid =fopen('golden_mag_f32.bin', 'wb');
for mm = 1 : datasize/(2*numSamples)
    input = fread(fid, 2*numSamples, 'float');
    for n = 0: numSamples-1
        output(n+1) = sqrt(input((2*n)+1)^2 + input((2*n)+2)^2); 
    end
    fprintf('%f\n', output);
%-------------------Data output-----------------------%
    A=fwrite(ofid, output, 'float');
end
%-----------------------------------------------------%
fclose(fid);
fclose(ofid);
%-------------------END-------------------------------%

