function [result]=optimize_dshift_range(db_ini, db_fim, nl, dimming, nlevels)

difftemp = db_fim-db_ini;
inctemp = difftemp/(nl-1);
valuesdb = (0:(nl-1))*inctemp+db_ini;

result=struct;
result.ndshift=zeros(nl,1);
result.cap=zeros(nl,1);
result.probs = zeros(nl,nlevels);
result.ruido = zeros(nl,1);
result.levels = zeros(nl, nlevels);

for i=1:nl
    
    fprintf('otimizando %d levels %f db\n', nlevels, valuesdb(i));
    noise = 10^(-valuesdb(i)/10);
    
    
    [ vals, fval ] = optimize_dshitf( nlevels, noise, dimming );
    result.ndshift(i)=vals(1);
    result.cap(i)=fval;
    result.probs(i,:)=vals(2:end);
    result.ruido(i)=valuesdb(i);
    result.levels(i,:) = calcLevelsFromDshift(vals(1), dimming, nlevels);
end

end

