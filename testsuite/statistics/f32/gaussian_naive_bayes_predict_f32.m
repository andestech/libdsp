clear;
clc;
NUMDIM=3;
NUMCLASS=3;
ADDVAR=0.3;
datasize = 300;
loop = datasize/NUMDIM;

pmean=[0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9];
pvar=[0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9];
classprior=[0.1, 0.2, 0.3];
buffer=zeros(1, NUMCLASS);
golden=zeros(1, loop);


%-------------------Read data-------------------------%
fid = fopen('input_f32.bin', 'rb');
%-------------------Data transform--------------------%
for n = 1 : loop
      input = fread(fid, NUMDIM, 'float');
  for m = 1 : NUMCLASS
      acc1=0;
      acc2=0;
      for k = 1 : NUMDIM
          sigma = pvar(k + (m-1)*NUMCLASS) + ADDVAR;
          tmpin = input(k) - pmean(k + (m-1)*NUMCLASS);
          acc1 = acc1 + log(2 * pi * sigma);
          acc2 = acc2 + (tmpin * tmpin / sigma);
          %fprintf('%f %f %f %f\n', acc2, tmpin, sigma, input(k));
      end
      tmp = -0.5 * acc1;
      tmp = tmp - 0.5 * acc2;
      buffer(m) = tmp + log(classprior(m));
  end
  [maximum(n),in(n)] = max(buffer);
  golden(n) = in(n)-1;
end

%-------------------Data output-----------------------%
 ofid = fopen('golden_gaussian_naive_bayes_predict_f32.bin', 'wb');
 fwrite(ofid, golden, 'float');
 fclose(ofid);
%-------------------END-------------------------------%
