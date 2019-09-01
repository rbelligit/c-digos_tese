function  graph_max_cap_qal
    
    %graph_max_cap_qal_niv(1, 4, [0.3]);
    %graph_max_cap_qal_niv(1, 4, [0.01 0.025 0.05 0.1 0.2 0.3]);
    %graph_max_cap_qal_niv(2, 4, (10.^(-2:0.05:-0.3)));
    %graph_max_cap_qal_niv(1, 8, [0.01 0.02 0.04 0.08]);
    %graph_max_cap_qal_niv(2, 8, (10.^(-2:0.05:-0.3))/4);
    graph_max_cap_qal_niv(1, 4, [10 11 12 14 17 20]);
end

function graph_max_cap_qal_niv(which, nlevels, noisedb)
    

    if(which == 1)
        fprintf('Iniciando processamento com %d niveis -- gráfico em função do dimming\n', nlevels);
        dimming=[0.05:0.01:0.09 0.1:0.02:.2 0.25:0.05:0.45];
        dimming=[dimming 0.5 fliplr(1-dimming)];
        
        for nn=1:length(noisedb)
            do_plot_dimming(dimming, nlevels, noisedb(nn), ...
                sprintf('graphs/niv%d_prob_noise_%g.eps', nlevels, noisedb(nn)));
        end
        
    end

    

    if(which == 2)
        %noise = (10.^(-2:0.05:-0.3))/divnoise;
        fprintf('Iniciando processamento com %d niveis -- gráfico em função do nível de ruído\n', nlevels);
        dimming = [0.2 0.5 0.8];
        for nn=1:length(dimming)
            do_plot_noise(dimming(nn), nlevels, noisedb, ...
                sprintf('graphs/niv%d_prob_dimm_%g.eps', nlevels, dimming(nn)));
        end
        
       
    end
end



function do_plot_dimming(dimming, nlevels, noisedb, fig_name)

dimming=dimming(:);
n1 = length(dimming);

prob_best=zeros(n1, nlevels);
level_best=zeros(n1, nlevels);
fval=zeros(n1,4);

noise = 10^(-noisedb / 10);

for n=1:n1
    [prob, levels, fval(n,1)] = best_prob_pam2(nlevels, dimming(n), noisedb);
    [ fval(n,2) ] = limite_inf1( 1, noise, dimm_set(dimming(n)));
    [ fval(n,3) ] = limite_sup1( 1, noise, dimm_set(dimming(n)));
    [ fval(n,4) ] = limite_sup2( 1, noise, dimm_set(dimming(n)));
    [levels,ind]=sort(levels);
    prob=prob(ind);
    prob_best(n,:) = prob;
    level_best(n,:) = levels;
end

filename=sprintf('./vars/var_noise_%f_levels_%d.mat', noisedb, nlevels);
save(filename, 'prob_best', 'level_best', 'fval', 'dimming');

co = get(0,'defaultaxescolororder');
lso = get(0,'defaultaxeslinestyleorder');

set(0,'defaultaxescolororder',[0,0,0]);
set(0,'defaultaxeslinestyleorder',{'-','--','-.','o-','+-'}); 

close(figure(1));

fig = figure(1);

set(fig, 'Units', 'Centimeters');
set(fig, 'Position', [2, 2, 30, 11]);
set(fig, 'PaperUnits','inches','PaperPosition',[0 0 12 3.3])
%ppp=get(fig, 'OuterPosition');
%ppp(2)=ppp(2)-0.2;
%ppp(4)=ppp(4)+0.2;
%set(fig, 'OuterPosition', ppp);

heigh=0.8;
width=0.25;
lastwidth=0.34;
space=0.05;
fix_size=width*2+lastwidth+space*4;
width=width/fix_size;
space=space/fix_size;
lastwidth=lastwidth/fix_size;
bottom=0.1;
posinit=space;

g(1)=axes('Position',[posinit bottom width heigh]);

plot(dimming, level_best);
axis([0 1 -.1 1.1]);
ylabel('Niveis')
xlabel('dimming');
grid on;
posinit=posinit+width+space;
g(2)=axes('Position',[posinit bottom width heigh]);

plot(dimming, prob_best);
axis([0 1 -.1 1.1]);
ylabel('probabilidades');
xlabel('dimming');
grid on;
title(sprintf('noisedb=%0.1fdB', noisedb))

fval(:,1)= fval(:,1);
fval(:,2:4)=fval(:,2:4)*log2(exp(1));
posinit=posinit+width+space;
g(3)=axes('Position',[posinit bottom lastwidth heigh]);
hhh=plot(dimming, fval);

legend(g(3),'PAM','infer','sup1', 'sup2', 'Location','eastoutside');
ylabel('Capacidade bits');
xlabel('dimming');

grid on;


%suptitle(sprintf('noise=%f', noise));

for i=1:3
    fix_position(g(i))
end

print(fig, '-deps2', '-r100', fig_name);

set(0,'defaultaxescolororder',co);
set(0,'defaultaxeslinestyleorder',lso); 

end

function do_plot_noise(dimming, nlevels, noisedb, fig_name)

noisedb=noisedb(:);
n1 = length(noisedb);

prob_best=zeros(n1, nlevels);
level_best=zeros(n1, nlevels);
fval=zeros(n1,4);

for n=1:n1
    [prob, levels, fval(n,1)] = best_prob_pam2(nlevels, dimming, noisedb(n));
    noise = 10^(-noisedb(n) / 10);
    [ fval(n,2) ] = limite_inf1( 1, noise, dimm_set(dimming));
    [ fval(n,3) ] = limite_sup1( 1, noise, dimm_set(dimming));
    [ fval(n,4) ] = limite_sup2( 1, noise, dimm_set(dimming));
    [levels,ind]=sort(levels);
    prob=prob(ind);
    prob_best(n,:) = prob;
    level_best(n,:) = levels;
end

max_noise=max(noisedb);


filename=sprintf('./vars/var_dimming_%f_levels_%d.mat', dimming, nlevels);
save(filename, 'prob_best', 'level_best', 'fval', 'noisedb');

co = get(0,'defaultaxescolororder');
lso = get(0,'defaultaxeslinestyleorder');

set(0,'defaultaxescolororder',[0,0,0]);
set(0,'defaultaxeslinestyleorder',{'-','--','-.','o-','+-'}); 

close(figure(1));

fig = figure(1);

set(fig, 'Units', 'Centimeters');
set(fig, 'Position', [2, 2, 30, 11]);
set(fig, 'PaperUnits','inches','PaperPosition',[0 0 12 3.3])
%ppp=get(fig, 'OuterPosition');
%ppp(2)=ppp(2)-0.2;
%ppp(4)=ppp(4)+0.2;
%set(fig, 'OuterPosition', ppp);

heigh=0.8;
width=0.25;
lastwidth=0.34;
space=0.05;
fix_size=width*2+lastwidth+space*4;
width=width/fix_size;
space=space/fix_size;
lastwidth=lastwidth/fix_size;
bottom=0.1;
posinit=space;

g(1)=axes('Position',[posinit bottom width heigh]);

semilogx(noisedb, level_best);
axis([0 max_noise -.1 1.1]);
ylabel('Niveis')
xlabel('Qualidade do canal (dB)');
grid on;
posinit=posinit+space+width;
g(2)=axes('Position',[posinit bottom width heigh]);

semilogx(noise, prob_best);
axis([0 max_noise -.1 1.1]);
ylabel('probabilidades');
xlabel('Qualidade do canal (dB)');
grid on;
title(sprintf('dimming=%f', dimming))

fval(:,1)=-fval(:,1);
fval(:,2:4)=fval(:,2:4)*log2(exp(1));
posinit=posinit+space+width;
g(3)=axes('Position',[posinit bottom lastwidth heigh]);
semilogx(noise, fval);
ttt=axis;
axis([0 max_noise ttt(3) ttt(4)]);
legend(g(3),'PAM','infer','sup1', 'sup2', 'Location','eastoutside');
ylabel('Taxa bits por uso');
xlabel('Qualidade do canal (dB)');
grid on;


%suptitle(sprintf('dimming=%f', dimming));

for i=1:3
    fix_position(g(i))
end

print(fig, '-deps2', '-r100', fig_name);

set(0,'defaultaxescolororder',co);
set(0,'defaultaxeslinestyleorder',lso); 


end

function [probs, levels, fval] = best_prob_pam2(nlevels, dimming, noisedb)

[levels, probs, fval] = optimize_levels_probs(nlevels, dimming, noisedb);

end

function fix_position(fid)
ppp=get(fid,'OuterPosition');
ppp(2)=ppp(2)+0.03;
ppp(4)=ppp(4)-0.02;
set(fid, 'OuterPosition', ppp);
end

function d=dimm_set(dim)

d = dim*(dim<0.5)+(1-dim)*(dim>=0.5);    
end