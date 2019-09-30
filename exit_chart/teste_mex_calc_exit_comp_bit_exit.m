%mex_calc_exit_comp_bit_exit

%[exit_bit_to_check, exit_check_to_bit] = mex_calc_exit_comp(nSimuls, mapInv, pesosBit, pesosCheck, levels, sigma, incremento)

nSimuls = 20000;
dados_mapeamento = [ 1 3 2 1 1 1 1 2 1 4 2 1 1 3 1 2]; %mapInv
pesosBits = [0 0.5 0.5];
pesosChecks = [0 0 0.5 0.5];
levels = [0.0 0.0666 0.1355 0.2153];
noise = 11;
sigma = 10^(-noise/10);
incremento = 0.02;

values = 0:incremento:1.2;

[exit_bit_to_check, exit_check_to_bit] = mex_calc_exit_comp_bit_exit(nSimuls, dados_mapeamento, pesosBits, pesosChecks, levels, sigma, incremento)
values = values(1:length(exit_bit_to_check));

plot(exit_bit_to_check, values);
hold on
plot(values, exit_check_to_bit);

