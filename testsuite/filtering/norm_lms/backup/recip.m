
function [ret, ret2] = recip(src, dst)

    RecipTable=  [ 0x7F03F03F 0x7D137420 0x7B31E739 0x795E9F94 0x7798FD29 0x75E06928 0x7434554D 0x72943B4B 0x70FF9C40 0x6F760031 0x6DF6F593 0x6C8210E3 0x6B16EC3A 0x69B526F6 0x685C655F 0x670C505D 0x65C4952D 0x6484E519 0x634CF53E 0x621C7E4F 0x60F33C61 0x5FD0EEB3 0x5EB55785 0x5DA03BEB 0x5C9163A1 0x5B8898E6 0x5A85A85A 0x598860DF 0x58909373 0x579E1318 0x56B0B4B8 0x55C84F0B 0x54E4BA80 0x5405D124 0x532B6E8F 0x52556FD0 0x5183B35A 0x50B618F3 0x4FEC81A2 0x4F26CFA2 0x4E64E64E 0x4DA6AA1D 0x4CEC008B 0x4C34D010 0x4B810016 0x4AD078EF 0x4A2323C4 0x4978EA96 0x48D1B827 0x482D77FE 0x478C1657 0x46ED801D 0x4651A2E5 0x45B86CE2 0x4521CCE1 0x448DB244 0x43FC0CFA 0x436CCD78 0x42DFE4B4 0x42554426 0x41CCDDB6 0x4146A3C6 0x40C28923 0x40408102];

       sig = 0;
       index = 0;
       INDEX_MASK= 0x0000003F;
       if(src > 0)
          sig = clz(src) - 1;
       else
          sig = clz(-src) - 1;
       endif
       src = bitshift(src, sig);
       index = bitshift(src, -24);
       index = bitand(index, INDEX_MASK);
       %octave index starts from 1 not zero. therefore, add one.
       index++;
       out = RecipTable(index);
    %   printf('index=%f, out=%f\n', index, out);
       for l = 1 : 2
          val = bitshift( src * out ,-31);
          val = 0x7FFFFFFF - val;
          out = clip_q63_to_q31(bitshift( out * val ,-30));
       end
       dst=out;
       ret2 = out;
       ret = sig +1;
