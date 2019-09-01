function graph_max_cap_pam_only_prob
    graph_max_cap_qal_niv(1, (0:3)/3, [10 11 12 14 17 20]);
    %graph_max_cap_qal_niv(2, (0:3)/3, (10.^(-2:0.05:-0.3)));
    %graph_max_cap_qal_niv(1, (0:7)/7, [0.001 0.01 0.02 0.04 0.08]);
    %graph_max_cap_qal_niv(2, (0:7)/7, (10.^(-2:0.05:-0.3))/4);
    
end

function graph_max_cap_qal_niv(which, levels, noise)
    
    nlevels=length(levels);
    if(which == 1)
        fprintf('Iniciando processamento com %d niveis -- gráfico em função do dimming\n', nlevels);
        dimming=[0.1:0.02:.2 0.25:0.05:0.45];
        dimming=[dimming 0.5 fliplr(1-dimming)];
        
        for nn=1:length(noise)
            do_plot_dimming(dimming, levels, noise(nn), ...
                sprintf('graphs/niv%d_level_fix_prob_noise_%g.eps', nlevels, noise(nn)));
        end
    end

    if(which == 2)
        %noise = (10.^(-2:0.05:-0.3))/divnoise;
        fprintf('Iniciando processamento com %d niveis -- gráfico em função do nível de ruído\n', nlevels);
        dimming = [0.2 0.5 0.8];
        for nn=1:length(dimming)
            do_plot_noise(dimming(nn), levels, noise, ...
                sprintf('graphs/niv%d_level_fix_prob_dimm_%g.eps', nlevels, dimming(nn)));
        end
    end
end


function do_plot_dimming(dimming, levels, noisedb, fig_name)

nlevels=length(levels);
dimming=dimming(:);
n1 = length(dimming);

noise = 10^(-noisedb/10);

prob_best=zeros(n1, nlevels);
level_best=zeros(n1, nlevels);
fval=zeros(n1,4);

for n=1:n1
    %[prob, levels, fval(n,1)] = best_prob_pam2(nlevels, dimming(n), noise);
    [levels_calc, prob, fval(n,1)] = optimize_probs(levels, dimming(n), noisedb);
    [ fval(n,2) ] = limite_inf1( 1, noise, dimm_set(dimming(n)));
    [ fval(n,3) ] = limite_sup1( 1, noise, dimm_set(dimming(n)));
    [ fval(n,4) ] = limite_sup2( 1, noise, dimm_set(dimming(n)));
    [levels_calc,ind]=sort(levels_calc);
    prob=prob(ind);
    prob_best(n,:) = prob;
    level_best(n,:) = levels_calc;
end

filename=sprintf('./vars/var_only_prob_noise_%f_levels_%d.mat', noisedb, nlevels);
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
title(sprintf('noise=%f', noise))

%fval(:,1)=fval(:,1);
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

function do_plot_noise(dimming, levels, noise, fig_name)

nlevels=length(levels);
noise=noise(:);
n1 = length(noise);

prob_best=zeros(n1, nlevels);
level_best=zeros(n1, nlevels);
fval=zeros(n1,4);

for n=1:n1
    %[prob, levels, fval(n,1)] = best_prob_pam2(nlevels, dimming, noise(n));
    [prob, levels_calc, fval(n,1)] = optimize_probs(levels, dimming, noise(n));
    [ fval(n,2) ] = limite_inf1( 1, noise(n), dimm_set(dimming));
    [ fval(n,3) ] = limite_sup1( 1, noise(n), dimm_set(dimming));
    [ fval(n,4) ] = limite_sup2( 1, noise(n), dimm_set(dimming));
    [levels_calc,ind]=sort(levels_calc);
    prob=prob(ind);
    prob_best(n,:) = prob;
    level_best(n,:) = levels_calc;
end

filename=sprintf('./vars/var_only_prob_dimming_%f_levels_%d.mat', dimming, nlevels);
save(filename, 'prob_best', 'level_best', 'fval', 'noise');

max_noise=max(noise);


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

semilogx(noise, level_best);
axis([0 max_noise -.1 1.1]);
ylabel('Niveis')
xlabel('noise');
grid on;
posinit=posinit+space+width;
g(2)=axes('Position',[posinit bottom width heigh]);

semilogx(noise, prob_best);
axis([0 max_noise -.1 1.1]);
ylabel('probabilidades');
xlabel('noise');
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
ylabel('Capacidade bits');
xlabel('noise');
grid on;


%suptitle(sprintf('dimming=%f', dimming));

for i=1:3
    fix_position(g(i))
end

print(fig, '-deps2', '-r100', fig_name);

set(0,'defaultaxescolororder',co);
set(0,'defaultaxeslinestyleorder',lso); 


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
