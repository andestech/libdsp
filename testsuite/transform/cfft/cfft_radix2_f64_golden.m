clear;
clc;

m = TEMP_LOGN;
n = 2^m; % n = 2^m;

%-----------------------------------%
fid = fopen('tmp_input.bin', 'r');
%-----------------------------------%

for lop = 1 : n
  src(lop) = fread(fid, 1, 'double');
end

for k = 1 : n
   x(2*k-1) = src(k);
   x(2*k) = src(k);
end

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

%middle stage%
ie = 2;
k = m - 2;
while (k)
  n2 = n2 / 2;
  ptrs = 0;
  ptre = n2;
  ptr1 = ptrs;
  j = ie;

  while(j)
  {
     ptr2 = ptr1 + n2;
     xa = x(ptr1 + 1);
     ya = x(ptr1 + 2);
     xb = x(ptr2 + 1);
     yb = x(ptr2 + 2);
     x(ptr1 + 1) = xa  + xb;
     x(ptr1 + 2) = ya + yb;
     x(ptr2 + 1) = xa - xb;
     x(ptr2 + 2) = ya - yb;
     ptr1 = ptr2 + n2;
     j--;
  }
  endwhile

  ia = 0;
  ptrs += 2;
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

%last stage
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

    % k odd
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


for k = 1 : 2*n
    printf("%2.10f, ", x(k));
    if( mod(k, 8) == 0)
    printf("\n");
    end
end


%-----------------------------------%
oid = fopen('tmp_output.bin', 'wb');
%-----------------------------------%
putdata = fwrite(oid, x, 'double');
a=fclose(oid);
