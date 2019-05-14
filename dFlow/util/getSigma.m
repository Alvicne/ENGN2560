function sigma=getSigma(s,o,sigma0,oR)
% cf. http://www.vlfeat.org/api/scalespace-fundamentals.html
% input
%  s      - sublevel within octave o
%  o      - octave
%  sigma0 - base smoothing
%  oR     - octave resolution
  sigma=sigma0*2^(o+s/oR);

end