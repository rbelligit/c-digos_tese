function plot_levels_probs(noise, levels, isToPrint)

if(nargin < 3)
    isToPrint = 0;
end

load(sprintf('var_noise_%f_levels_%d.mat', noise, levels));

close all;


figH=figure(1);

 set(figH, 'Position', [100, 100, 500, 850]);

subplot(311);

set(0,'defaultaxescolororder',[0,0,0]);
set(0,'defaultaxeslinestyleorder',{'-','--','-.','o-','+-', 'x-'}); 
%for i=1:levels
%    plot(dimming, level_best(:,i));
%    hold on;
%end
plot(dimming, level_best);
grid on;
title(sprintf('Otimização Completa\nQualidade %d dB', noise), 'fontweight','bold');
xlabel('dimming');
ylabel('Níveis');
%v = axis;
axis([0.05 0.95 0 1]);

subplot(312);

set(0,'defaultaxescolororder',[0,0,0]);
set(0,'defaultaxeslinestyleorder',{'-','--','-.','o-','+-', 'x-'});   
%for i=1:levels
%    plot(dimming, prob_best(:,i));
%    hold on;
%end
valMax = (floor(max(max(prob_best))/0.1)+1)*0.1;
if(valMax > 1)
    valMax = 1;
end
plot(dimming, prob_best);
grid on;
axis([0.05 0.95 0 valMax])
%title(sprintf('PAM probabilities', noise), 'fontweight','bold');
xlabel('dimming');
ylabel('Probabilidades');


subplot(313);

%fval(:,1)=fval(:,1);

fval(:,2)=min(fval(:,2:3),[],2);

fval(:,3)=[];


plot(dimming, fval);
hold on;
st=load(sprintf('var_only_prob_noise_%f_levels_%d.mat', noise, levels), 'fval', 'dimming');

plot(st.dimming, st.fval(:,1), 'o')

grid on;
xlabel('dimming');
ylabel('Taxa (bits / uso do canal)');

drawArrow = @(x,y) quiver( x(1),y(1),x(2)-x(1),y(2)-y(1),0 );

xx = axis;

ymin=xx(3);
ymax=xx(4);

axis([0.05 0.95 ymin ymax]);

diff = ymax-ymin;

%title('PAM Capacities and Bounds', 'fontweight','bold');

text(0.1, ymin+.1*diff, 'Otimização completa');
text(0.1, ymin+.3*diff, 'Limite Superior');
text(0.1, ymin+.5*diff, 'Limite Inferior');
text(0.1, ymin+.7*diff, 'PAM igualmente espaçado');

%ll = legend('Optimezed PAM', 'Upper Bound', 'Lower Bound', 'Equally Spaced PAM');
%pos = get(ll, 'position')
%set(ll, 'pos', [pos(1) 0.25 pos(3) pos(4)]);


if(isToPrint > 0)
    
end

end