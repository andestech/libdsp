clear;
clc;

totalsize = 9500;
numofbool = 95;
loop = totalsize/numofbool;
%-------------------Read data-------------------------%
fid = fopen('input_bool_a.bin', 'rb');
fid2 = fopen('input_bool_b.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
    tt=0;
    ff=0;
    tf=0;
    ff=0;
    InputA = fread(fid, numofbool, 'uint32');
    InputB = fread(fid2, numofbool, 'uint32');
    [tt, ff, tf, ft] = boolean_dist_TT_FF_TF_FT(InputA, InputB, numofbool);
    r = 2.0*(tf + ft);
    s = 1.0*(ff + tt);
    out = (r / (s+r));
    golden(n) = out;
end
golden
%-------------------Data output-----------------------%
ofid = fopen('golden_sokalmichener_dist.bin', 'wb');
putdata = fwrite(ofid, golden, 'float');
a=fclose(ofid);
%-------------------END-------------------------------%
fclose(fid);
fclose(fid2);
