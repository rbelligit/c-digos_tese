disp('aqui');


[snrs_1, error_1] = read_result_ldpc('result_70.txt');
[snrs_2, error_2] = read_result_ldpc('saida_70_MM2.out');
[snrs_3, error_3] = read_result_ldpc('saida_70_MM8.out');
[snrs_4, error_4] = read_result_ldpc('saida_70_MM8_MM10.out');

error_1 = error_1 + 1e-6;
error_2 = error_2 + 1e-6;
error_3 = error_3 + 1e-6;
error_4 = error_4 + 1e-6;


semilogy(snrs_1, error_1, 'r')
hold on
semilogy(snrs_2, error_2, 'r')
semilogy(snrs_3, error_3, 'b');
semilogy(snrs_4, error_4, 'k');
xxx=axis();
axis([xxx(1) xxx(2) 1e-6 1]);

legend('70000 - cap=1.1429', '70 MM2 - cap=1.1429', '70 MM8 - cap=1.1429', '70 MM8 MM10- cap=1.1429');

title('4 niveis - 0.0 0.0666 0.1355 0.2153 - probs - 9/16 , 4/16 , 2/16 , 1/16 - cap=1.3925');

