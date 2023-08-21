function out = clip_to_q15(src)

    MAX_INT=32768;
    MIN_INT=-32767;
    if(src > MAX_INT)
        out = MAX_INT;
    elseif( src < MIN_INT)
        out = MIN_INT;
    else
        out = src;
    endif
