%
% Plota os gráficos do M para dimming de 0.1 até 0.45 e ruído variando de 2
% até 10 dB


noisedbv = 2:.2:12;
dimmings = .05:.02:.45;

n_noises = length(noisedbv);
n_dimmings = length(dimmings);

resultsM = zeros(n_noises, n_dimmings);

for i_n = 1:n_noises
    for i_d = 1:n_dimmings
        dimming = dimmings(i_d);
        [m, fval, eta, probs] = find_opt_ndshift_m(noisedbv(i_n), dimming, 2, 0.1);
        resultsM(i_n,i_d) = m;
        fprintf('i_n=%f i_d=%f noise=%f - dimming=%f - m=%f - cap=%f - eta=%f\n', ...
            i_n, i_d, noisedbv(i_n), dimming, m, fval, eta);
    end
end

surf(dimmings, noisedbv, resultsM, zeros(n_noises, n_dimmings));