clear;
clc;
datasize = 10000;

%-------------------save positive data-------------------------%
fid = fopen('input_f32.bin', 'rb');
half_data = fread(fid, datasize/2, 'single');
a=fclose(fid);

fprintf('%f\n', half_data);
ofid = fopen('input_abs_f32.bin', 'w+b');
putdata = fwrite(ofid, half_data, 'single');
a=fclose(ofid);
%-------------------save negtive data transform--------------------%
negtivedata = -half_data;
ofid = fopen('input_abs_f32.bin', 'ab');		%--append binary--
putdata = fwrite(ofid, negtivedata, 'single');
a=fclose(ofid); 

blocksize = 8;
loop = datasize/blocksize;
%-------------------Read input data-------------------------%
fid = fopen('input_abs_f32.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
  input = fread(fid, blocksize, 'float');  
  [abs_minimum(n),in(n)] = min(abs(input)); %-abs-%
  fprintf('%f %d\n',abs_minimum(n), in(n));
end

 fclose(fid);
%-------------------Data output-----------------------%
 ofid = fopen('golden_absmin_f32_absmin.bin', 'wb');
 o2fid = fopen('golden_absmin_f32_index.bin', 'wb');
 fwrite(ofid, abs_minimum, 'float');
 fwrite(o2fid, in, 'int');
 fclose(ofid);
 fclose(o2fid);
%-------------------END-------------------------------%
