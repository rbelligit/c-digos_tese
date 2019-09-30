function [resp] = calc_value_exit_1(mapeamento,noise,levels)
%CALC_VALUE_EXIT_1 Summary of this function goes here
%   Detailed explanation goes here
sigma_canal = 10^(-(noise/10));
Ler = [];
Le2r = [];
for La1 = 0:0.05:1
    [Le, Le2] = calcExitDecInc_mex(mapeamento, levels, La1, sigma_canal);
    Ler=[Ler Le];
    Le2r=[Le2r Le2];
end

resp = mean(Ler(1:end-1));

end