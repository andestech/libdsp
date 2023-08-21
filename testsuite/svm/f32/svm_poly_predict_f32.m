clear;
clc;
NUMDIM=10;
NUMVEC=3;
INTERCEPT=-0.1;
COEF0=0.2;
GAMMA=-0.4;
EXPT=3;

datasize = 300;
loop = datasize/NUMDIM;

dualcoe=[-0.4, 0.2, -0.3];
vec=[0.1, 0.2, -0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.1,-0.1, -0.2, 0.3, -0.4, -0.5, 0.6, -0.7, -0.8, 0.9, 0.1, 0.1, 0.2, -0.3, 0.4, 0.5, 0.6, -0.7, 0.8, 0.9, 0.1];

class=[3,7];
golden=zeros(1, loop);


%-------------------Read data-------------------------%
fid = fopen('input_f32.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
      input = fread(fid, NUMDIM, 'float');
      acsum = INTERCEPT;
  for m = 1 : NUMVEC
      dot=0;
      for k = 1 : NUMDIM
          dot = dot + input(k) * vec(k + (m-1)*NUMDIM);
          %fprintf('%d %f %f %f\n', k, dot, input(k), vec(k + (m-1)*NUMVEC));
      end
      base = GAMMA * dot + COEF0;
      acsum = acsum + dualcoe(m) * base^EXPT;
  end
  acsum
  if(acsum <= 0)
      tmp = 1;
  else
      tmp = 2;
  end
  tmp
  golden(n) = class(tmp);
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_svm_poly_predict_f32.bin', 'wb');
 fwrite(ofid, golden, 'int');
 fclose(ofid);
%-------------------END-------------------------------%
