disp('aqui');


[snrs_1, error_1] = read_result_ldpc('result_70.txt');
[snrs_2, error_2] = read_result_ldpc('saida_70_6_3_3.out');
[snrs_3, error_3] = read_result_ldpc('saida_70_6_5_3.out');
%[snrs_4, error_4] = read_result_ldpc('saida_70_MMMM2.out');
%[snrs_5, error_5] = read_result_ldpc('saida_70_MMMMM2.out');

error_1 = error_1 + 1e-9;
error_2 = error_2 + 1e-9;
error_3 = error_3 + 1e-9;
%error_4 = error_4 + 1e-9;
%error_5 = error_5 + 1e-9;


semilogy(snrs_1, error_1, 'r-')
hold on
semilogy(snrs_2, error_2, 'r-.')
semilogy(snrs_3, error_3, 'bx-');
%semilogy(snrs_4, error_4, 'ko-');
%semilogy(snrs_5, error_5, 'k');
xxx=axis();
axis([xxx(1) xxx(2) 1e-9 1]);

legend('70', '70\_6\_3\_3\_2', '70\_6\_3\_5\_2');

title('4 niveis - 0.0 0.0666 0.1355 0.2153 - probs - 9/16 , 4/16 , 2/16 , 1/16 - cap=1.3925');

