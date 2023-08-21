clear;
clc;

m= 14;
n = 16384; % n = 2^m;

%-----------------------------------%
fid = fopen('input_f32.bin', 'r');
%ofid = fopen('golden_cfft_radix2_f32.bin', 'wb');
%-----------------------------------%

for lop = 1 : n
  x(lop) = fread(fid, 1, 'float');
% y(lop) = fread(fid, 1, 'float');
end

pp = (2*pi)/n;
N = n;
M = m;

m = m -1;
n = n /2;

%cfft_radix2
p = (2*pi)/n;
n2 = n;

xa = x(1);
ya = x(2);
xb = x(n+1);
yb = x(n+2);
x(1) = xa + xb;
x(2) = ya+ yb ;
x(n+1) = xa - xb ;
x(n+2) = ya - yb;

ia = 0;
ptre = n;
ptrs = 2;
%first stage
while (ptrs != ptre)
  ia++;
  c = cos(ia*p);
  s = sin(ia*p);
  ptr = ptrs + n;
  xa = x(ptrs + 1);
  ya = x(ptrs + 2);
  xb = x(ptr + 1);
  yb = x(ptr + 2);
  xt = xa - xb;
  x(ptrs+1) = xa + xb;
  yt = ya - yb;
  x(ptrs+2) = ya + yb;
  x(ptr+1) = (c * xt + s * yt);
  x(ptr+2) = (c * yt - s * xt);
  ptrs += 2;
endwhile
%%%has been modified


%middle stage%
ie = 2;
k = m - 2;
while (k)
  n2 = n2 / 2;
  ptrs = 0;
  ptre = n2;
  ptr1 = ptrs;
  j= ie;

  while(j)
  {
     ptr2 = ptr1 + n2;
     xa = x(ptr1 + 1);
     ya = x(ptr1 + 2);
     xb = x(ptr2 + 1);
     yb = x(ptr2 + 2);
     x(ptr1 + 1) = xa  + xb;
     x(ptr1 +2) = ya + yb;
     x(ptr2 +1) = xa - xb;
     x(ptr2 +2) = ya - yb;
     ptr1 = ptr2 +n2;
     j--;
  }
  endwhile

  ia = 0;
  ptrs +=2;
  while(ptrs != ptre)
   ia = ia + ie;
   ptr1 = ptrs;
   j = ie;
      while(j)
          ptr2 = ptr1 + n2;
          xa = x(ptr1+1);
          ya = x(ptr1+2);
          xb = x(ptr2+1);
          yb = x(ptr2+2);
          xt = xa - xb;
           c = cos(ia*p);
           s = sin(ia*p);
          x(ptr1+1) = (xa + xb);
          yt = ya - yb;
          x(ptr1+2) = (ya + yb);
          x(ptr2 +1) = (c *xt + s*yt);
          x(ptr2 +2) = (c*yt - s *xt);
          ptr1 = ptr2 + n2;
           j--;
        endwhile
      ptrs+=2;
  endwhile
    ie = ie*2;
    k--;
endwhile
%%%has been modified

%for last stage (has been verified)

ptre = 2 * n;
ptrs= 0;
while (ptrs != ptre)
   xa = x(ptrs+1);
   ya = x(ptrs+2);
   xb = x(ptrs+3);
   yb = x(ptrs+4);
   x(ptrs+1) =  (xa + xb);
   x(ptrs+2) =  (ya + yb);
   x(ptrs+3) =  (xa - xb);
   x(ptrs+4) =  (ya - yb);
   xa = x(ptrs+5);
   ya = x(ptrs+6);
   xb = x(ptrs+7);
   yb = x(ptrs+8);
   x(ptrs+5) =  (xa + xb);
   x(ptrs+6) =  (ya + yb);
   x(ptrs+7) =  (xa - xb);
   x(ptrs+8) =  (ya - yb);
   ptrs += 8;
endwhile

function ret = TRANSFORM_MATH_BIT_REV(src, xx, yy)
    temp = src(2*xx+1); src(2*xx+1) = src(2*yy+1); src(2*yy+1) = temp ;
    temp = src(2*xx+2); src(2*xx+2) = src(2*yy+2); src(2*yy+2) = temp ;
    ret = src;
endfunction


function ret = TRANSFORM_MATH_BIT_REVF1(src, xx, yy, f1)
    temp = src(2*xx+f1); src(2*xx+f1) = src(2*yy +f1); src(2*yy+f1) = temp ;
    temp = src(2*xx+f1-1 ); src(2*xx+f1-1) = src(2*yy +f1-1); src(2*yy+f1-1) = temp ;
    ret = src;
endfunction

%bit reverse

%BIT_REV
%f11 = 2* n -1;
f11 = 2 * n;
nh = n / 2;
k = 2;
r = nh;
x = TRANSFORM_MATH_BIT_REV(x, 1, nh);

while(1)
    r = bitxor(r, nh);
    %k even

    m = nh / 2;
    r = bitxor(r, m);
    while(not(bitand(r, m)))
         m = m / 2;
         r = bitxor(r, m);
    endwhile

    if(r > k)
     x = TRANSFORM_MATH_BIT_REV(x, k, r);
     x = TRANSFORM_MATH_BIT_REVF1(x, -k, -r, f11);
    end

    k++;

 %   % k odd
    r += nh;
    x = TRANSFORM_MATH_BIT_REV(x, k, r);
    k++;
    if(k >= nh)
         break;
    end

    r = bitxor(r, nh);
    halfnh = nh / 2;
    r = bitxor(r, halfnh);

    %add

    m = nh / 4 ;
    r =  bitxor(r, m);
    while(not(bitand(r, m)))
         m = m /2 ;
         r =  bitxor(r, m);
    endwhile

    %add

    if(r > k)
        x = TRANSFORM_MATH_BIT_REV(x, k, r);
        x = TRANSFORM_MATH_BIT_REVF1(x, -k, -r, f11);
    end

    k++;
    r += nh;

    x =  TRANSFORM_MATH_BIT_REV(x, k, r);
    k++;
endwhile

%rfft
i = N / 4;
j = i;
ppp = (2*pi) / N;

while(i != 0)
     c = cos(i*ppp);
     s = sin(i*ppp);
     xt1 = 0.5 * (x(2*i+1) + x(2*j+1));
     xt2 = 0.5 * (x(2*i+1) - x(2*j+1));
     yt1 = 0.5 * (x(2*i+2) + x(2*j+2));
     yt2 = 0.5 * (x(2*i+2) - x(2*j+2));

     xt3 = yt1 * c - xt2 *s;
     yt3 = -(xt2 *c + yt1 *s);
     x(2 * i + 1) = xt1 + xt3;
     x(2 * i + 2 ) = yt3 + yt2;
     x(2 * j + 1) = xt1 -xt3;
     x(2 * j + 2) = yt3 - yt2;
     j++;
     i--;
endwhile
    xt1 = x(1);
    x(1) += x(2);
    x(2) = xt1 - x(2);

fclose(fid);

for k = 1 : N
    printf("%2.10f, ", x(k));
    if( mod(k, 8) == 0)
    printf("\n");
    end
end
