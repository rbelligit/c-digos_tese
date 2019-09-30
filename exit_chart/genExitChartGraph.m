
pesosBits = [0 0.5 0.5];
pesosChecks = [0 0 0.5 0.5];
levels = [0.0 0.0666 0.1355 0.2153];
dados_mapeamento=[1 2 3 1 1 2 1 4 2 1 1 3 1 2 1 1 ];
sigma = 10^(-16/10);
incr = 0.02;
xx = 0:incr:(1-incr/2);

[demtobit, bittodem] = mex_calc_exit_comp(100000, dados_mapeamento, ...
            pesosBits, pesosChecks, levels, sigma, incr);
        
close all;
plot(xx,demtobit,'r');
hold on
plot(bittodem,xx, 'b');

xlabel('$I\left(L_{b_{k}\rightarrow I\left\{ h_{k}b=0\right\} },b_{k}\right)$', 'interpreter','latex');
ylabel('$I\left( L_{I\left\{ h_{k}b=0\right\} \rightarrow b_k}, b_k\right)$', 'interpreter','latex');

posx = 0;
posy=0;

posVect = zeros(2,1000);
posVect(:,1) = [posx ; posy];
index = 2;
contval = true;
while(contval)
    posy = approx(xx,demtobit,posx);
    if(posy > 1)
        posy = 1;
        contval = false;
    end
    posVect(:, index) = [posx ; posy];
    index = index + 1;
    posx = approx(xx, bittodem, posy);
    if(posx > 1)
        posx = 1;
        contval=false;
    end
    posVect(:, index) = [posx ; posy];
    index = index+1;
    fprintf('posx=%f - posy=%f\n', posx, posy);
    if(index > 50)
        contval = false;
    end
end
index = index - 1;
plot(posVect(1,1:index),posVect(2,1:index), 'k')

grid on

legend()


function val = approx(x,y,val)
    index = find(x>val,1);
    if(isempty(index))
       val = (val - x(end)) * ...
           (y(end)-y(end-1)) / (x(end)-x(end-1)) + ...
           y(end);
    elseif(index == 1)
        val = (val - x(1)) * ...
           (y(1)-y(2)) / (x(1)-x(2)) + ...
           y(1);
    else
        val = (val - x(index-1)) * ...
            (y(index)-y(index-1)) / (x(index) - x(index-1)) + ...
            y(index-1);
    end

end