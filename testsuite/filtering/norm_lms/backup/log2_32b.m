function out = log2_32b(v)
      r = 0;
      s = 0;
      if(v > 0xFFFF)
          r = bitshift(1, 4);
      else
          r = 0;
      endif
      v = bitshift(v, -r);

      if(v > 0xFF)
          s = bitshift(1, 3);
      else
          s = 0;
      endif
      v = bitshift(v ,-s);
      r = bitor(r , s);

      if(v > 0xF)
          s = bitshift(1, 2);
      else
          s =0;
      endif
      v = bitshift(v ,-s);
      r = bitor(r , s);


      if(v > 0x3)
          s = 1 * 2;
      else
          s = 0;
      endif
      v = bitshift(v ,-s);
      r = bitor(r , s);
      tmpv = bitshift(v, -1);
      r = bitor(r, tmpv);
      out = r;
