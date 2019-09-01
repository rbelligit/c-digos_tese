function ans = optimize_probs_range(db_ini, db_fim, quant, nlevels, dimming)


difftemp = db_fim-db_ini;
inctemp = difftemp/(quant-1);
valuesdb = (0:(quant-1))*inctemp+db_ini;

temp = struct;


for i=1:quant
    noise = 10^(-valuesdb(i)/10);
    [levels, probs, cap]=optimize_probs(nlevels, dimming, noise);
    disp(sprintf('noise=%e - db=%f\n', noise, valuesdb(i)));
    temp(i).cap = cap;
    temp(i).noise = noise;
    temp(i).noisedb = valuesdb(i);
    temp(i).levels = levels;
    temp(i).probs  = probs;
    
    
end

ans=temp;

end