function [val] = calc_value_exit(mapeamento, noise, levels)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
ruido = 10^(-noise/10);
lav = 0:0.05:0.95;
result = zeros(1,length(lav));
ii=1;
val = zeros(1,2);
for la = lav
    result(ii) = mex_calc_exit(mapeamento, levels, la, ruido, 10000);
    
    ii = ii+1;
end

val(1) = mean(result);


Ler = [];
Le2r = [];

for La1 = lav
    [Le, Le2] = calcExitDecInc_mex(mapeamento, levels, La1, ruido);
    Ler=[Ler Le];
    Le2r=[Le2r Le2];
end

val(2) = mean(Ler(1:end-1));
%val(2)=val(1);

end

