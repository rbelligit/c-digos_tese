clear all;
close all;

nlevels = 4;
noisedb = 7;
dimmingIni = .05;
dimmingEnd = .45;
nPoints = 9;

dimmings = dimmingIni + (dimmingEnd - dimmingIni)/(nPoints-1) * ((1:nPoints) - 1);

[resultDShift, resultComp] = comp_ndhift_complete(nlevels, noisedb, dimmingIni, dimmingEnd, nPoints);

ff = figure(1);

subplot(311);
plot(dimmings, resultDShift.tax, '-k');
hold on;
plot(dimmings, resultComp.tax, '-.b');
title(sprintf('Otimização completa e por {\\eta}DShift com S = %ddB', noisedb));
grid on;
ylabel('taxa bits por uso do canal');
xlabel('Dimming');
%legend('');

subplot(312);
for i=1:nlevels
    plot(dimmings, resultDShift.levels(i,:), '-k');
    hold on;
    plot(dimmings, resultComp.levels(i,:), '-.b');
end
grid on;
ylabel('Níveis M-PAM');
xlabel('Dimming');

subplot(313);
for i=1:nlevels
    plot(dimmings, resultDShift.probs(i,:), '-k');
    hold on;
    plot(dimmings, resultComp.probs(i,:), '-.b');
end
grid on;

ylabel('Probabilidade dos níveis M-PAM');
xlabel('Dimming');


ff.InnerPosition = [665   247   589   724];

nomeArq =  sprintf('comp_opt_dshift_comp_noise_%02d.eps', noisedb);


print(nomeArq, '-depsc');

figure(2);
plot(dimmings, resultDShift.eta);