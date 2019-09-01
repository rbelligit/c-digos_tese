%
% Plota os gráficos do M para dimming de 0.1 até 0.45 e ruído variando de 2
% até 10 dB
close all;
clear all;

noisedbv = 4:.02:14;



dimming = 0.45;
nn = length(noisedbv);

result = zeros(1,nn);
result_eta = zeros(1,nn);
result_cap = zeros(1,nn);

for ii=1:nn
    [m, fval, eta, probs] = find_opt_ndshift_m(noisedbv(ii), dimming, 2, 0);
    result(ii) = m;
    result_eta(ii) = eta;
    result_cap(ii) = fval;
    fprintf('ii=%f - noise=%f - dimming=%f - m=%f - cap=%f - eta=%f\n', ...
            ii, noisedbv(ii), dimming, m, fval, eta);
        probs
end
ff = figure(1);
subplot(311)
plot(noisedbv, result);
grid on
xlabel('Qualidade do canal - dB');
ylabel('número de símbolos - M');
title(sprintf('Fator de Dimming de %0.2f', dimming));

subplot(312)
plot(noisedbv, result_eta);
grid on;
xlabel('Qualidade do canal - dB');
ylabel('\eta');
xxx=axis();
axis([xxx(1) xxx(2) 0 1]);

subplot(313)
plot(noisedbv, result_cap);
grid on;
xlabel('Qualidade do canal - dB');
ylabel('capacidade bits/uso');

figure(1);
ff.InnerPosition(3:4)= [550 550];
nomeArq =  sprintf('dshift_opt_niveis_dim_0_%02d_comp.eps', int32(dimming*100));
fprintf('Nome do arquivo=%s\n', nomeArq);
print(nomeArq, '-depsc')
disp('Imprimido');


