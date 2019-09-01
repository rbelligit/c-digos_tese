function [ cap ] = limite_inf1( A, sigma, alpha )
% Calcula o limite inferior da capacidade de um canal com as seguintes
% caracteristicas:
% Ruido gaussiano com desvio padrao sigma,
% e o sinal de entrada com as seguintes condições:
% P(X<0)=0, P(X>A)=0, E[X]=alpha*A

%calcula o valor de mu*, que é tirado da seguinte relação:
%alpha=1/mu*-e^(-mu*)/(1-e^(-mu*))
mu_s=calc_mu_star(alpha);

cap = 0.5*log(1+A.^2*((exp(2*alpha*mu_s))/(2*pi()*exp(1)*sigma^2)) ...
    * ((1-exp(-mu_s))/(mu_s))^2 );

end

