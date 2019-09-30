function plotExit(dados_mapeamento, pesosBits, pesosChecks, levels, sigma, incr)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
[demtobit, bittodem] = mex_calc_exit_comp(100000, dados_mapeamento, ...
            pesosBits, pesosChecks, levels, sigma, incr);
        
xx = 0:incr:(1-incr/2);

figure;
plot(xx,demtobit);
xlabel('I_{bitToDem}');
ylabel('I_{demToBit}');
hold on;
plot(bittodem,xx);
        
end

