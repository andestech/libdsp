clear;
clc;
datasize = 2047;
high = 0.5;
low = -0.5;

%-------------------Read data-------------------------%
fid = fopen('input_f32.bin', 'rb');
Inputdata = fread(fid, datasize, 'single');
a=fclose(fid);
%-------------------clip data --------------------%
for nn = 1 : datasize
    if(Inputdata(nn) > high)
        outputdata(nn) = high;
    elseif (Inputdata(nn) < low)
        outputdata(nn) = low;
    else
        outputdata(nn) = Inputdata(nn);
    end
end
%-------------------Data output-----------------------%
ofid = fopen('golden_clip_f32.bin', 'wb');
putdata = fwrite(ofid, outputdata, 'single');
a=fclose(ofid);
