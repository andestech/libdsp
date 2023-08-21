clear;
clc;
m= 3;
datasize = 2*64;
n = 64; % n = 4^m;
%-----------------------------------%
fid = fopen('input_q15.bin', 'rb');
ofid = fopen('golden_cfft_radix4_q15.bin', 'wb');
%-----------------------------------%
src = fread(fid, datasize, 'short');
p = 2*pi/n;
n2 = n;
%-----------------------------------%
n1 = n2;
n2 = n2 /4;
ic = 0;
i0=1;
j = n2;
ie = 1;
while(1)
  i1 = i0 + n2;
  i2 = i1 + n2;
  i3 = i2 + n2;
  T0 = src(2*i0-1)/4;
  T1 = src(2*i0)/4;
  S0 = src(2*i2-1)/4;
  S1 = src(2*i2)/4;
  R0 = satura(T0+S0,16);
  R1 = satura(T1+S1,16);
  S0 = satura(T0-S0,16);
  S1 = satura(T1-S1,16);
  T0 = src(2*i1-1)/4;
  T1 = src(2*i1)/4;
  U0 = src(2*i3-1)/4;
  U1 = src(2*i3)/4;
  T0 = satura(T0+U0,16);
  T1 = satura(T1+U1,16);
  src(2*i0-1) = R0/2 + T0/2;
  src(2*i0) = R1/2 + T1/2;
  R0 = satura(R0-T0,16);
  R1 = satura(R1-T1,16);
  co2 = q2int_q15(cos(2*(ic)*p),1);
  si2 = q2int_q15(sin(2*(ic)*p),1);
  out1 =(bitshift((co2*R0 + si2*R1),-16));
  out2 =(bitshift((-si2*R0 + co2*R1),-16));
  T0 = src(2*i1-1)/4;
  T1 = src(2*i1)/4;
  src(2*i1-1) = out1;
  src(2*i1) = out2;
  U0 = src(2*i3-1)/4;
  U1 = src(2*i3)/4;
  T0 = satura(T0-U0,16);
  T1 = satura(T1-U1,16);
  R0 = (satura(((int32)(S0-T1)),16));
  R1 = (satura(((int32)(S1+T0)),16));
  S0 = (satura(((int32)(S0+T1)),16));
  S1 = (satura(((int32)(S1-T0)),16));
  co1 = q2int_q15(cos((ic)*p),1);
  si1 = q2int_q15(sin((ic)*p),1);
  out1 =(bitshift((si1*S1 + co1*S0),-16));
  out2 =(bitshift((-si1*S0 + co1*S1),-16));
  src(2*i2-1) = out1;
  src(2*i2) = out2;
  co3 = q2int_q15(cos(3*(ic)*p),1);
  si3 = q2int_q15(sin(3*(ic)*p),1);
  out1 =(bitshift((si3*R1 + co3*R0),-16));
  out2 =(bitshift((-si3*R0 + co3*R1),-16));
  src(2*i3-1) = out1;
  src(2*i3) = out2;
  ic = ic + ie;
  i0 = i0 + 1;
  j=j-1;
  if(j==0)
	break;
  end
end
%------------------------------------%
ie = ie *4;
k=n/4;
while(k>4)
  n1 = n2;
  n2 = n2 /4;
  ic = 0;
  for jj = 1 : n2
	co1 = q2int_q15(cos((ic)*p),1);
	si1 = q2int_q15(sin((ic)*p),1);
	co2 = q2int_q15(cos(2*(ic)*p),1);
	si2 = q2int_q15(sin(2*(ic)*p),1);
	co3 = q2int_q15(cos(3*(ic)*p),1);
	si3 = q2int_q15(sin(3*(ic)*p),1);
	ic = ic + ie;
	for i0 = jj : n1: n
	  i1 = i0 + n2;
	  i2 = i1 + n2;
	  i3 = i2 + n2;
      T0 = src(2*i0-1);
      T1 = src(2*i0);
      S0 = src(2*i2-1);
      S1 = src(2*i2);
      R0 = satura(T0+S0,16);
      R1 = satura(T1+S1,16);
      S0 = satura(T0-S0,16);
      S1 = satura(T1-S1,16);
      T0 = src(2*i1-1);
      T1 = src(2*i1);
      U0 = src(2*i3-1);
	  U1 = src(2*i3);
	  T0 = satura(T0+U0,16);
      T1 = satura(T1+U1,16);
	  out1 = (R0/2 + T0/2)/2;
	  out2 = (R1/2 + T1/2)/2;
      src(2*i0-1) = out1;
      src(2*i0) = out2;
      R0 = R0/2 - T0/2;
	  R1 = R1/2 - T1/2;
      out1 =(bitshift((co2*R0 + si2*R1),-16));
      out2 =(bitshift((-si2*R0 + co2*R1),-16));
      T0 = src(2*i1-1);
      T1 = src(2*i1);
      src(2*i1-1) = out1;
      src(2*i1) = out2;
      U0 = src(2*i3-1);
      U1 = src(2*i3);
      T0 = satura(T0-U0,16);
      T1 = satura(T1-U1,16);
      R0 = S0/2 - T1/2;
	  R1 = S1/2 + T0/2;
	  S0 = S0/2 + T1/2;
	  S1 = S1/2 - T0/2;
      out1 =(bitshift((si1*S1 + co1*S0),-16));
      out2 =(bitshift((-si1*S0 + co1*S1),-16));
      src(2*i2-1) = out1;
      src(2*i2) = out2;
      out1 =(bitshift((si3*R1 + co3*R0),-16));
      out2 =(bitshift((-si3*R0 + co3*R1),-16));
      src(2*i3-1) = out1;
      src(2*i3) = out2;
	end
  end
  k=k/4;
  ie = ie*4;
end
%------------------------------------%
  n1 = n2;
  n2 = n2 /4;
	for i0 = 1 : n1: (n-n1+1)
	  i1 = i0 + n2;
	  i2 = i1 + n2;
	  i3 = i2 + n2;
      T0 = src(2*i0-1);
      T1 = src(2*i0);
      S0 = src(2*i2-1);
      S1 = src(2*i2);
      R0 = satura(T0+S0,16);
      R1 = satura(T1+S1,16);
      S0 = satura(T0-S0,16);
      S1 = satura(T1-S1,16);
      T0 = src(2*i1-1);
      T1 = src(2*i1);
      U0 = src(2*i3-1);
	  U1 = src(2*i3);
	  T0 = satura(T0+U0,16);
      T1 = satura(T1+U1,16);
	  src(2*i0-1) = R0/2 + T0/2;
	  src(2*i0) = R1/2 + T1/2;
      R0 = R0/2 - T0/2;
	  R1 = R1/2 - T1/2;
      T0 = src(2*i1-1);
      T1 = src(2*i1);
      src(2*i1-1) = R0;
      src(2*i1) = R1;
      U0 = src(2*i3-1);
      U1 = src(2*i3);
      T0 = satura(T0-U0,16);
      T1 = satura(T1-U1,16);
      src(2*i2-1) = S0/2 + T1/2;
      src(2*i2) = S1/2 - T0/2;
      src(2*i3-1) = S0/2 - T1/2;
      src(2*i3) = S1/2 + T0/2;
	end

%------------------------------------%
  out = reverse(src, n);
  fwrite(ofid, out, 'short');
%-----------------------------------%
fclose(fid);
fclose(ofid);
cifft_radix4_q15;
