clear;
clc;
datasize = 800;
valA = zeros(datasize,1);

%-------------------Read data-------------------------%
fid = fopen('input_f32_2_rand_a.bin', 'rb');
fid2 = fopen('input_f32_2_rand_b.bin', 'rb');
InputA = fread(fid, datasize, 'float');
InputB = fread(fid2, datasize, 'float');
fclose(fid);
fclose(fid2);
%-------------------Data transform--------------------%
InputA
InputB
divout = InputA ./ InputB;


%------------------- Write data-------------------------%
ofid = fopen('golden_div_f32.bin', 'wb');
putdata = fwrite(ofid, divout, 'float');
fclose(ofid);
