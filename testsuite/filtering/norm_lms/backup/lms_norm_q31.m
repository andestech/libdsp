clear;
clc;
BLOCKSIZE = 4;
NUMSTAP = 5;
mu = 0x500;
loopsize = 110;
statesize = BLOCKSIZE + NUMSTAP -1
postshift = 8;
ushift = postshift +1;
lshift = 32 - ushift;
b = zeros(1, NUMSTAP);
b_n = zeros(1, NUMSTAP);
y_n =0;
y = zeros(1, BLOCKSIZE);
x = zeros(1, BLOCKSIZE);
x0 = zeros(1, NUMSTAP-1);

%--------------------------------%
fid = fopen('input_q31.bin', 'rb');
f2id = fopen('input_ref_q31.bin', 'rb');
ofid = fopen('golden_lms_norm_q31.bin', 'wb');
o2fid = fopen('golden_err_norm_q31.bin', 'wb');
%--------------------------------%
x_d = fread(fid, BLOCKSIZE, 'int'); %in
d_d = fread(f2id, BLOCKSIZE, 'int'); %d(n)
x = x_d;
d = d_d;
%---------------------------------%

x00 = 0x20000;
energy =  0x20000;
DELTA_Q31= 0x100;
onebyenergy = 0;
s_index =0;
u_index = NUMSTAP;


for loop = 1 : loopsize
    for nn = 1 : BLOCKSIZE
	    y_i =0;
	    E=0;
        in = x(nn);
        state(u_index) = in;
        u_index++;
        tmp1 = energy * 4294967296; % 4294967296 is 2^ 32
        tmp2 = x00 *x00 * 2;

        energy = bitshift(tmp1 - tmp2, -32);
        tmp1 = in * in * 2;
        tmp2 = energy * 4294967296; % 4294967296 is 2^ 32
        energy =  bitshift(tmp1+ tmp2,-32);


        for m =  1 : NUMSTAP
	        y_n = y_i + b(m) * state(m + s_index);
            y_i=y_n;
	    end

        acc_l = mod(y_i,  bitshift(1, 32));
        acc_h = y_i /(bitshift(1, 32));

        acc = bit_or( bitshift(acc_l, -lshift), bitshift(acc_h, ushift));
        e(nn) = d(nn) - acc;
        [postshift , onebyenergy] =  recip(energy + DELTA_Q31, onebyenergy);

        errormu =  bitshift(e(nn) * mu, -31); %output is int
        errormu2=  (e(nn) * mu) / 2147483648; %output is float
        if(errormu < 0)
            if((errormu - errormu2) !=0)
             errormu =  (errormu - 1) ;
            endif
        endif

        w = clip_q63_to_q31(bitshift(errormu * onebyenergy, -(31 -postshift)));
	    for m =  1 : NUMSTAP
            coef = bitshift( w * state(m + s_index), -32);
	        b(m) = clip_q63_to_q31(b(m) + coef * 2 );
        end
        x00 = state(nn);
	    y(nn) = acc;

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
  fwrite(ofid, yy, 'int');
  fprintf('%d out:', loop);fprintf('%d ',yy);
  fprintf('\n');
  fwrite(o2fid, ee, 'int');
  fprintf('%d err:', loop);fprintf('%d ',ee);
  fprintf('\n');
end
%-----------------------------------%
fclose(fid);
fclose(ofid);
fclose(f2id);
fclose(o2fid);
