[snrs_1, error_1] = read_result_ldpc('result_20000_28000_c_2.txt')
[snrs_2, error_2] = read_result_ldpc('result_20000_28000_c_tot_1.txt');
[snrs_3, error_3] = read_result_ldpc('result_21000_28000_c_1.txt')
[snrs_4, error_4] = read_result_ldpc('result_42.txt')
[snrs_5, error_5] = read_result_ldpc('result_70.txt')

error_1 = error_1 + 1e-6;
error_3 = error_3 + 1e-6;
error_4 = error_4 + 1e-6;
error_5 = error_5 + 1e-6;

semilogy(snrs_1, error_1, 'r')
hold on
semilogy(snrs_3, error_3, 'b');
semilogy(snrs_4, error_4, 'k');
semilogy(snrs_5, error_5, 'g');
xxx=axis();
axis([xxx(1) xxx(2) 1e-6 1]);

legend('20000-28000 - cap=1.1429', '21000-28000 - cap=1.0', '30000-42000 - cap=1.1429', '50000-70000 - cap=1.1429');

title('4 niveis - 0.0 0.0666 0.1355 0.2153 - probs - 9/16 , 4/16 , 2/16 , 1/16 - cap=1.3925');

