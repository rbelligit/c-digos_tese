close all;
clear all;
dados_mapeamento = [ 1 3 2 1 1 1 1 2 1 4 2 1 1 3 1 2];
%dados_mapeamento = [ 1 1 1 1 1 1 1 1 1 2 2 2 2 3 3 4];
pesosBits = [0 0.5 0.5];
pesosChecks = [0 0 0.5 0.5];
levels = [0.0 0.0666 0.1355 0.2153];

noises = 12:.5:14;

resp = cell(1,length(noises));
i=1;
for noise=noises

    sigma = 10^(-noise/10);
    incremento = 0.02;


    [demtobit, bittodem] = mex_calc_exit_comp(100000, dados_mapeamento, pesosBits, pesosChecks, levels, sigma, incremento);
    
    resp1.demobit = demtobit;
    resp1.bittodem = bittodem;
    
    

    x = 0:incremento:(1-incremento/2);
    close all;
    figure(1)
    plot(demtobit, x);
    hold on
    plot(x,bittodem);
    
    xlabel('I_{demtobit}');
    ylabel('I_{bittodem}');
    title(sprintf('SNR %f', noise));
    grid on;
    
    minDiff = 1;
    for jj=1:length(x)
        dd = demtobit(jj);
        kk = x(jj);
        kk = calcInvAprox(kk, x, bittodem);
        fprintf('dd=%f - kk=%f - diff=%f\n', dd, kk, dd-kk);
        if(minDiff > dd-kk)
            minDiff=dd-kk;
        end
    end
    fprintf('MinDiff=%f\n',minDiff);
    resp1.minDiff = minDiff;
    resp1.noise = noise;
    resp{i}=resp1;
    i=i+1;
end

function r = calcInvAprox(val, valx, valfunc)
    valAnt = 0;
    xant = 0;
    r=-1;
    for i=1:length(valfunc)
        if(valfunc(i) > val)
            r = (val - valAnt)/(valfunc(i)-valAnt)*(valx(i)-xant) + xant;
            return;
        end
        valAnt = valfunc(i);
        xant = valx(i);
    end
end