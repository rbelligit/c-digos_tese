
clear all;
close all;
% bbbb
% iiii
% tttt
% 4321  <<<--- Posição no vetor
%
% 0000 - 1
% 0001 - 1
% 0010 - 1
% 0011 - 1
% 0100 - 1
% 0101 - 1
% 0110 - 1
% 0111 - 1
% 1000 - 1
% 1001 - 2
% 1010 - 2
% 1011 - 2
% 1100 - 2
% 1101 - 3
% 1110 - 3
% 1111 - 4
%
%

% MAP17
%0000,0001,0010,0100,0110,1010,1011,1110,1111,
%0101,0111,1000,1100
%1001,1101
%0011
%
% 0000 - 1
% 0001 - 1
% 0010 - 1
% 0011 - 4
% 0100 - 1
% 0101 - 2
% 0110 - 1
% 0111 - 2
% 1000 - 2
% 1001 - 3
% 1010 - 1
% 1011 - 1
% 1100 - 2
% 1101 - 3
% 1110 - 1
% 1111 - 1

dados_mapeamento = [ 1 1 1 4 1 2 1 2 2 3 1 1 2 3 1 1];

% MAP10
%1100,1110,1111,0000,0011,1010,0100,1001,0110,
%1101,0101,1000,0001
%1011,0010
%0111
% 0000 - 1
% 0001 - 2
% 0010 - 3
% 0011 - 1
% 0100 - 1
% 0101 - 2
% 0110 - 1
% 0111 - 4
% 1000 - 2
% 1001 - 1
% 1010 - 1
% 1011 - 3
% 1100 - 1
% 1101 - 2
% 1110 - 1
% 1111 - 1
%dados_mapeamento = [ 1 2 3 1 1 2 1 4 2 1 1 3 1 2 1 1];



%MAP7
%0101,0011,1110,1000,0000,1100,0100,1011,0110,
%1111,0111,0010,1010
%0001,1101
%1001
% 0000 - 1
% 0001 - 3
% 0010 - 2
% 0011 - 1
% 0100 - 1
% 0101 - 1
% 0110 - 1
% 0111 - 2
% 1000 - 1
% 1001 - 4
% 1010 - 2
% 1011 - 1
% 1100 - 1
% 1101 - 3
% 1110 - 1
% 1111 - 2
dados_mapeamento = [ 1 3 2 1 1 1 1 2 1 4 2 1 1 3 1 2];


niveis = [0.0 0.0666 0.1355 0.2153];
%La = 0.2;

noisedb = 17;
sigma_canal = 10^(-noisedb/10)
La=0:.1:0.9;
Ler = zeros(1,length(La));
Le2r = zeros(1,length(La));
Le3r = zeros(4,length(La));
Le_cpp = zeros(1,length(La));
pos = 1;
for La1=La
    Le = 0;Le2=0;Le3=0;
    [Le, Le2, Le3] = calcExitDecInc_mex(dados_mapeamento, niveis, La1, sigma_canal);
    Le_cpp(pos) = mex_calc_exit(dados_mapeamento, niveis, La1, sigma_canal, 10000);
    fprintf(1,"%f - %f\n", La1, Le_cpp(pos));
    Ler(pos)=Le;
    Le2r(pos)=Le2;
    Le3r(:,pos) = Le3(:);
    pos=pos+1;
end

plot(La, Ler);
hold on;
plot(La, Le2r);
hold on;
plot(La, Le_cpp, 'o');

%figure


%for i=1:4
%    plot(La, Le3r(i,:));
%    hold on
%end

% simb_probs = [1 1 100 1];
% 
% LL = [5 0 0 0]
% 
% probs_0 = exp(LL);
% probs_0 = 1./(1+probs_0);
% probs_1 = 1 - probs_0;
% probs_1 = probs_1(:);
% probs_0 = probs_0(:);
% probs = [probs_0 probs_1];
% nn=length(dados_mapeamento);
% nbitss = log2(nn);
% 
% probs
% 
% for i=2:2
%     mm = calc_message_simb_to_bit_mex(i, dados_mapeamento, probs, nn, simb_probs, nbitss)
%     
% end
