function cap=calc_max_cap(nlevels, dimming, noisedb)
%calcula a maxima capacidade para um canal pam com:
% nlevels : numero de níveis
% dimming : fator de dimming
% noisedb = 10*log10(A/noise)
% Utiliza a funcao best_prob_pam2 para isso.

noise = 1/(10^(noisedb/10)); % A = 1

[prob_best, level_best, cap] = best_prob_pam2(nlevels, dimming, noise)
cap = -cap;

end
