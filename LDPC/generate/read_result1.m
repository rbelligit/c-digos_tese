close all
clear all

[ruido1, erros1]=read_file_result('saida_70_6_5_3_map.out');
[ruido2, erros2]=read_file_result('saida_70_6_3_5_2.out');

erros1 = erros1+1e-10;
erros2 = erros2+1e-10;

semilogy(ruido1, erros1, '-');
hold on;
semilogy(ruido2, erros2, '-x');
xx=axis;
axis([xx(1) xx(2) 1e-7 1]);

title('4 níveis 0 0.0666 0.1355 0.2153');

legend('6-3\_3-2 mapeamento alterado', '6-3\_3-2 - mapeamento normal');