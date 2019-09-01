function lev = calcLevelsFromDshift(eta, dimming, nlevels)

if(dimming < 0.5)
    sinal = -1;
else
    sinal = 1;
end
dshift = abs(1-2*dimming);

if(sinal > 0)
    lev = (0:(nlevels-1))/(nlevels-1)*(1-eta*dshift)+eta*dshift;
else
    lev = (0:(nlevels-1))/(nlevels-1)*(1-eta*dshift);
end

end

