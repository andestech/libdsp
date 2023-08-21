function out = clip_q63_to_q31(src)

    MAX_INT=2147483647;
    MIN_INT=-2147483648;
    if(src > MAX_INT)
        out = MAX_INT;
    elseif( src < MIN_INT)
        out = MIN_INT;
    else
        out = src;
    endif
