function [ mu_star, fval, ex_flag ] = calc_mu_star( alpha )
%Resolve o valor de mu* na equacao:
%alpha = 1./mu-exp(-mu)/(1-exp(-mu))
%onde alpha varia de 0 ate 0.5
    if(alpha < 0 || alpha > 0.5)
        error('Wrong value of alpha, should be between 0 and 0.5');
    end
    options = optimoptions('fsolve','Display','off', 'TolFun', 1e-10); 
    [mu_star, fval, ex_flag]= fsolve(@(x)alpha_from_mu(x,alpha), 10, options);
    
end

function erro=alpha_from_mu(mu, alpha)
    erro = 1./mu-exp(-mu)/(1-exp(-mu))-alpha;

end