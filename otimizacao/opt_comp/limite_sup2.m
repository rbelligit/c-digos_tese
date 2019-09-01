function [ cap ] = limite_sup2(  A, sigma, alpha )
%LIMITE_SUP2 Summary of this function goes here
%   Detailed explanation goes here

delta = sigma*log(1+A/sigma);

mu_s = calc_mu_star(alpha);

mu = mu_s*(1-exp(-alpha*delta^2/(2*sigma^2)) );

[ cap ] = limite_sup2_comp( A, sigma, alpha, delta, mu );

end

