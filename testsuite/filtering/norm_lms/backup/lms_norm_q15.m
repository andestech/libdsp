clear;
clc;
BLOCKSIZE = 5;
NUMSTAP = 5;
mu = 0x2000;
loopsize = 100;
statesize = BLOCKSIZE + NUMSTAP -1
postshift = 1;
lshift = 15 - postshift;
ushift = 32 - lshift ;
b = zeros(1, BLOCKSIZE);
b_n = zeros(1, BLOCKSIZE);
y_n =0;
y = zeros(1, BLOCKSIZE);
x = zeros(1, BLOCKSIZE);
x0 = zeros(1, NUMSTAP-1);

%--------------------------------%
fid = fopen('input_q15.bin', 'rb');
f2id = fopen('input_ref_q15.bin', 'rb');
ofid = fopen('golden_lms_norm_q15.bin', 'wb');
o2fid = fopen('golden_err_norm_q15.bin', 'wb');
%--------------------------------%
x_d = fread(fid, BLOCKSIZE, 'short'); %in
d_d = fread(f2id, BLOCKSIZE, 'short'); %d(n)
x = x_d;
d = d_d;
%---------------------------------%

x00 = 0;
energy = 0;
DELTA_Q15= 0x5;
onebyenergy = 0;
s_index = 0;
u_index = NUMSTAP;


for loop = 1 : loopsize
    for nn = 1 : BLOCKSIZE
	    y_i =0;
	    E=0;
        in = x(nn);
        state(u_index) = in;
        u_index++;

        energy -= bitshift(x00*x00, -15);
        energy += bitshift(in*in, -15);

        for m =  1 : NUMSTAP
	        y_n = y_i + b(m) * state(m + s_index);
            y_i=y_n;
	    end

        acc_l = mod(y_i,  bitshift(1, 32));
        acc_h = y_i /(bitshift(1, 32));


        acc = bit_or( bitshift(acc_l, -lshift), bitshift(acc_h, ushift));
        acc = clip_to_q15(acc);

	    y(nn) = acc;

        e(nn) = d(nn) - acc;
        [postshift , onebyenergy] =  recip_q15(energy + DELTA_Q15, onebyenergy);

        errormu =  bitshift(e(nn) * mu, -15); %output is int
        errormu2=  (e(nn) * mu) / 32768; %output is float
        if(errormu < 0)
            if((errormu - errormu2) !=0)
             errormu =  (errormu - 1) ;
            endif
        endif
        acc = (bitshift(errormu * onebyenergy, -(15 -postshift)));

        w =  clip_to_q15(acc);

        for m =  1 : NUMSTAP
            coef = b(m) + bitshift( w * state(m + s_index), -15);
	        b(m) = clip_to_q15( coef );
        end
        x00 = state(nn);

        s_index++;
    end

  sc_index = BLOCKSIZE +1;
  for i = 1 : (NUMSTAP -1)
      state(i) = state(sc_index);
      sc_index++;
  end

  u_index = NUMSTAP;
  s_index =0;
  yy = y;
  ee = e;
  fwrite(ofid, yy, 'short');
  fprintf('%d out:', loop);fprintf('%d ',yy);
  fprintf('\n');
  fwrite(o2fid, ee, 'short');
  fprintf('%d err:', loop);fprintf('%d ',ee);
  fprintf('\n');
end
%-----------------------------------%
fclose(fid);
fclose(ofid);
fclose(f2id);
fclose(o2fid);
