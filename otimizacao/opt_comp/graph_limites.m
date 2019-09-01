function graph_limites(noise, name)

alpha = [.01:.002:.07  .08:0.01:.5];

cap_inf = zeros(1, length(alpha));
cap_sup = zeros(1, length(alpha));
for ind=1:length(alpha)
    cap_inf1 = limite_inf1( 1, noise, alpha(ind) );
    cap_sup1 = limite_sup1( 1, noise, alpha(ind) );
    cap_sup2 = limite_sup2( 1, noise, alpha(ind) );
    cap_inf(ind) = cap_inf1;
    if(cap_sup1 < cap_sup2)
        cap_sup(ind)=cap_sup1;
    else
        cap_sup(ind)=cap_sup2;
    end;
end

cap_sup = [cap_sup fliplr(cap_sup(1,1:end-1))];
cap_inf = [cap_inf fliplr(cap_inf(1,1:end-1))];
alpha = [alpha 1-fliplr(alpha(1:end-1))];

figure(1);
plot(alpha, cap_inf*log2(exp(1)));
hold on;
plot(alpha, cap_sup*log2(exp(1)));

title(sprintf('Capacidade para ruido de %f ', noise));

xlabel('fator do dimming');
ylabel('bits / simbolo');
grid on;

print('-dpng', '-r200', name);

 
end