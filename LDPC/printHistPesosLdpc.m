

close all;
clear all;

fid = fopen('countOnesOutput.txt', 'r');

pesos = fscanf(fid, '%d');

fclose(fid);

pesos2 = vec2mat(pesos(:),10);

pesosf = sum(pesos2,2);
pesosValue = 1:length(pesosf);
pesosValue = ((pesosValue - 1) * 10) + 5;


bar(pesosValue, pesosf);

xlabel('Peso do código');
ylabel('Número de palavras código');
grid on;