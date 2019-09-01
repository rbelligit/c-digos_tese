function [resultDShift, resultComp] = comp_ndhift_complete(nlevels, noisedb, dimmingIni, dimmingEnd, nPoints)

incremento = (dimmingEnd - dimmingIni) / (nPoints - 1);

dimmings = (0:(nPoints-1))*incremento + dimmingIni;

resultDShift.tax = zeros(1,nPoints);
resultDShift.levels = zeros(nlevels,nPoints);
resultDShift.probs = zeros(nlevels, nPoints);
resultDShift.eta = zeros(1,nPoints);

resultComp.tax = zeros(1,nPoints);
resultComp.levels = zeros(nlevels, nPoints);
resultComp.probs = zeros(nlevels, nPoints);

for i=1:nPoints
    
    dimming = dimmings(i);
    
    [ vals, fval ] = optimize_dshitf( nlevels, noisedb, dimming, 0);
    resultDShift.tax(i) = fval;
    levels = calcLevelsFromDshift(vals(1), dimming, nlevels);
    resultDShift.levels(:,i) = levels(:);
    resultDShift.probs(:,i) = vals(2:end);
    resultDShift.eta(i) = vals(1);
    
    [levels, probs, cap] = optimize_levels_probs(nlevels, dimming, noisedb);
    resultComp.tax(i) = cap;
    resultComp.levels(:,i) = levels(:);
    resultComp.probs(:,i) = probs(:);
    
end


end

