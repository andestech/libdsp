function out = satura(input, cl)

posmax = 2^(cl-1)-1;
negmin = -2^(cl-1);

  if(input>posmax)
	out = posmax;
  elseif(input<negmin)
	out = negmin;
  else
	out = input;
  end
