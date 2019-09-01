function [levels, probs, cap] = optimize_probs(levels, dimming, noisedb)
%[levels, probs, cap] = optimize_levels_probs(nlevels, noise)
%nlevels -> número de níveis da modulação M-PAM
%dimming -> valor do dimming entre 0 e 1
%noise   -> nível de ruído considerando que os sinais variam de 0 até 1
%
%levels -> níveis após otimização
%probs  -> probabilidades após otimização
%cap    -> capacidade após otimização
    nlevels = length(levels);

    noise = 10^(-noisedb / 10);
    
    %levels=(0:(nlevels-1))/(nlevels-1);

    fun1 = @(x)(calcCap(x, nlevels, levels, dimming, noise));
    
    % Garante soma das probabilidades igual a 1
    Aeq1 = [ones(1,nlevels) ;... % Garante que a soma das probabilidades eh 1
            levels]; %garante o valor do dimming
    beq1 = [1 ; dimming];
    
    %Ax<=b
    A1=[ ];
    b1= [];
    
    % Limites minimos e máximos da variável 
    lb=zeros(1,nlevels);
    ub=ones(1,nlevels);
    
   
    
    
    %Opções da otimização
    opts = optimoptions('fmincon','TolFun', 1e-9, 'TolCon', 1e-8,...
     'Display', 'off', 'Algorithm', 'sqp');
 
    x_ini = ones(1, nlevels)/nlevels;
    
    
    [vals, fval]=fmincon(fun1, x_ini, A1, b1, ...
        Aeq1, beq1, lb, ub, funcNiv, ...
        opts);
    
    probs=vals;
    
    
    cap = -fval;
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function cap=calcCap(x, nlevels, levels, dimming, noise)
%
    
    cap = -capacity_vlc( levels, x, noise);
    
end

function [c,ceq]=funcNiv(x)
    c=[];
    ceq=[];
end



