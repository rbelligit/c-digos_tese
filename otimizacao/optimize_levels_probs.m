function [levels, probs, cap] = optimize_levels_probs(nlevels, dimming, noisedb)
%[levels, probs, cap] = optimize_levels_probs(nlevels, noise)
%nlevels -> número de níveis da modulação M-PAM
%dimming -> valor do dimming entre 0 e 1
%noisedb   -> qualidade do canal em db
%
%levels -> níveis após otimização
%probs  -> probabilidades após otimização
%cap    -> capacidade após otimização

    noise = 10^(-noisedb/10);

    timestart=tic;

    fun1 = @(x)(calcCap(x, nlevels, dimming, noise));
    
    % Garante soma das probabilidades igual a 1
    Aeq1 = [zeros(1,nlevels-1) ones(1,nlevels)]; % Garante que a soma das probabilidades eh 1
    beq1 = 1;
    
    %Ax<=b
    A1=[ ];
    b1= [];
    
    % Limites minimos e máximos da variável 
    lb=zeros(1,2*nlevels-1);
    ub=ones(1,2*nlevels-1);
    
    %restrição linear que garante níveis entre 0 e 1, inclusive do nível 
    %faltante
    funcNiv=@(x)(cfunc(x,dimming, nlevels));
    
    %VALOR INICIAL
    
    [levelsIni, probsIni] = calc_dshift_for_ini(dimming, nlevels, noisedb);
    %levelsIni = calc_best_levels_low_noise(dimming, nlevels);
    x_ini = [levelsIni(1:end-1) probsIni];
    
    %Opções da otimização
    opts = optimoptions('fmincon','TolFun', 1e-9, 'TolCon', 1e-8,...
     'Display', 'off', 'Algorithm', 'sqp');
    
    
    [vals, fval]=fmincon(fun1, x_ini, A1, b1, ...
        Aeq1, beq1, lb, ub, funcNiv, ...
        opts);
    
    x_ini = [(0:(nlevels-2))/(nlevels-1) ones(1,nlevels)/nlevels];
    [vals2, fval2]=fmincon(fun1, x_ini, A1, b1, ...
            Aeq1, beq1, lb, ub, funcNiv, ...
            opts);
    if(fval2 < fval)
        fval=fval2;
        vals=vals2;
    end
    
    fval2 = -capacity_vlc(calc_best_levels_low_noise(dimming, nlevels), ...
        (ones(1,nlevels)/nlevels), noise ) ;
    
    fprintf('fval2 (%17.14f) - fval (%17.14f)\n', fval2, fval);
    
    if(fval2 <= fval)
        fprintf('fval2 is better diff =%e\n', fval2-fval);
        probs  = ones(1,nlevels)/nlevels;
        levels = calc_best_levels_low_noise(dimming, nlevels);
        fval = fval2;
    else
        fprintf('fval2 is not better diff = %e\n', fval2-fval);
        probs  = vals(nlevels:end);
        levels = [vals(1:nlevels-1) calc_miss_level(vals, dimming, nlevels)];

    end
    
    
    timeelapsed=toc(timestart);
    fprintf('nlevels=%d - dimming=%f - noise=%f - fval=%f - time=%f\n',nlevels, dimming, noise, fval, timeelapsed);

    [levels, probs] = fix_level_and_probs(levels, probs);
    
    cap = -fval;
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function cap=calcCap(x, nlevels, dimming, noise)
%
    mis_level = calc_miss_level(x, dimming, nlevels);
    
    cap = -capacity_vlc( [ x(1:nlevels-1) mis_level ], ...
        x(nlevels:end), noise);
    
    
    levels1=sort([x(1:nlevels-1) mis_level]);
    if dimming > 0.5
        if(levels1(end)<1)
            cap=cap+(1-levels1(end))*3;
        end
    else
        if(levels1(1)>0)
            cap=cap+levels1(1)*3;
        end
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function mis_level=calc_miss_level(x, alfa, nlevels)
    temp1 = sum(x(1:(nlevels-1)).*x(nlevels:(end-1)));
    mis_level = (alfa-temp1)/x(end);
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [ levels ] = calc_best_levels_low_noise( dimming, nlevels )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

invert=0;

if (dimming > 0.5)
   invert=1;
   dimming = 1-dimming;
end

k=1:nlevels;

levels = ( (2*(k-1))/(nlevels-1) ) * dimming;

if(invert == 1)
    levels = 1-levels;
end

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [levels, probs] = calc_dshift_for_ini(dimming, nlevels, noisedb)

[ vals, fval ] = optimize_dshitf( nlevels, noisedb, dimming, 0.001 );

probs = vals(2:end);
levels = calcLevelsFromDshift(vals(1), dimming, nlevels);

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [c, ceq] = cfunc(x, alfa, nlevels)
  mis_level = calc_miss_level(x, alfa, nlevels);
  c = [-mis_level,mis_level-1] ;
  ceq=[];
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [level1, prob1] =fix_level_and_probs(level, prob)
    [level1,ind]=sort(level);
    prob1=prob(ind);
    for i=2:length(level1)
       
   
        if(abs(level1(i)-level1(i-1)) < 1e-2)
            if(level1(i)<0.5)
                prob1(i-1)=prob1(i-1)+prob1(i);
                prob1(i)=0;
            else
                prob1(i)=prob1(i-1)+prob1(i);
                prob1(i-1)=0;
            end
        end
    end
    
end
