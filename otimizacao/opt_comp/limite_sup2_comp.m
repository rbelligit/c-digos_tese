function [ cap ] = limite_sup2_comp( A, sigma, alpha, delta, mu )
%LIMITE_SUP2 Summary of this function goes here
%   Detailed explanation goes here

p1=1-qf((delta+alpha*A)/sigma)...
    -qf((delta+(1-alpha)*A)/sigma);
p1=p1*log((A/sigma)*...
    ( exp(mu*delta/A)-exp(-mu*(1+delta/A)) )...
    /( sqrt(2*pi())*mu*(1-2*qf(delta/sigma)) )...
    );
p2=-0.5+qf(delta/sigma)+delta/( sqrt(2*pi())*sigma ) ...
    *exp(-delta^2/(2*sigma^2));
p3=sigma*mu/(A*sqrt(2*pi())) * ...
    (...
        exp(-delta^2/(2*sigma^2))-...
        exp(-(A+delta)^2/(2*sigma^2))...
    );
p4=mu*alpha*(...
    1-2*qf( (delta+A/2)/sigma ) ...
    );
cap = (p1+p2+p3+p4)*log2(exp(1)); %corrigindo de nats para bits
end

function p=qf(z)
p = 0.5*erfc(z/sqrt(2));
end