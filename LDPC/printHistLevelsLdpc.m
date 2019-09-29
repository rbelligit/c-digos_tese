

close all;
clear all;

fid = fopen('countHistLevel.txt', 'r');

pesosf = fscanf(fid, '%d');

fclose(fid);


pesosValue = 1:length(pesosf);
pesosValue = (pesosValue / length(pesosf)) ;


bar(pesosValue, pesosf);

xlabel('Peso do código');
ylabel('Número de palavras código');
grid on;