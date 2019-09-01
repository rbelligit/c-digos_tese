function [ vals, fval ] = optimize_dshitf( nlevels, noisedB, dimming, varargin )
%Otimização de probabilidades e do eta para maximizar taxa de transmissão
%[ vals, fval ] = optimize_dshitf( nlevels, noise, dimming, varargin )
% varargin pode ter mais um parâmetro que é a probabilidade mínima que uma
% variável pode ter
    noise = 10^(-noisedB / 10);
    dshift = abs(1-2*dimming);
    if(dimming < 0.5)
        sinal = -1;
    else
        sinal = 1;
    end
    if(nargin == 4)
        minProb = varargin{1};
    else
        minProb = 0;
    end

    fun1 = @(x)(calcCap(x, dshift, sinal, nlevels, noise));
    
    % Garante soma das probabilidades igual a 1
    % x = [eta px1 ... pxm]
    Aeq1 = [0 ones(1,nlevels)]; % Garante que a soma das probabilidades eh 1
    beq1 = 1;
    
    %Ax<=b
    A1 = zeros(nlevels-1,nlevels+1);
    b1 = zeros(1,nlevels-1);
    %nlevels
    for ii=1:(nlevels-1)
        if(dimming < 0.5)
            A1(ii,:) = [0 zeros(1,ii-1) -1  1 zeros(1,nlevels-ii-1)];
        else
            A1(ii,:) = [0 zeros(1,ii-1)  1 -1 zeros(1,nlevels-ii-1)];
        end
    end
    
    %A1
    %b1
    
    %A1=[];
    %b1= [];
    
    % Limites minimos e máximos da variável 
    lb=[0 minProb * ones(1, nlevels)];
    ub=ones(1,nlevels+1);
    
    %restrição linear que garante níveis entre 0 e 1, inclusive do nível 
    %faltante
    funcNiv=@(x)(cfunc(x, dshift, sinal, dimming, nlevels));
    
    %VALOR INICIAL
    x_ini = [0 ones(1,nlevels)/nlevels];
    
    %Opções da otimização
    opts = optimoptions('fmincon','TolFun', 1e-9, 'TolCon', 1e-8,...
     'Display', 'off', 'Algorithm', 'sqp');
    
    
    [vals, fval]=fmincon(fun1, x_ini, A1, b1, ...
        Aeq1, beq1, lb, ub, funcNiv, ...
        opts);
    
    fval = -fval;

end

function [c,ceq] = cfunc(x, dshift, sinal, dimming, nlevels)
    eta = x(1);
    probs=x(2:end);
    if(sinal > 0)
        nn = (0:(nlevels-1))/(nlevels-1)*(1-eta*dshift)+eta*dshift;
    else
        nn = (0:(nlevels-1))/(nlevels-1)*(1-eta*dshift);
    end
    
    ceq= (sum(nn.*probs)-dimming);
    c=[];
end

function cap=calcCap(x, dshift, sinal, nlevels, noise)
       
    eta = x(1);
    probs=x(2:end);
    if(sinal > 0)
        nn = (0:(nlevels-1))/(nlevels-1)*(1-eta*dshift)+eta*dshift;
    else
        nn = (0:(nlevels-1))/(nlevels-1)*(1-eta*dshift);
    end
    
    cap = -capacity_vlc(nn, probs, noise);
end
