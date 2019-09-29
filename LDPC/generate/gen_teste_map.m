function gen_teste_map(quant, ini)

%rng('shuffle')


for i=1:quant
  maps = randperm(16)-1
  mapsBin = dec2bin(maps)

  mapsBin1 = sortrows(mapsBin(1:9,:));
  mapsBin2 = sortrows(mapsBin(10:13,:));
  mapsBin3 = sortrows(mapsBin(14:15,:));
  mapsBin4 = sortrows(mapsBin(16,:));

  val = i+ini;
  filename=sprintf('genTeste70_6_3_6_2_map%d.sh',val);
  fid = fopen(filename, 'w');
  fprintf(fid, '#!/bin/ksh\n');
  fprintf(fid, '#\n');
  fprintf(fid, '# @ class = serial\n');
  fprintf(fid, '# @ restart = no\n');
  fprintf(fid, '# @ output = saida_70_6_3_6_2_map%d.out\n', val);
  fprintf(fid, '# @ error = saida_70_6_3_6_2_map%d.err\n', val);
  fprintf(fid, '# @ queue\n\n\n');
  fprintf(fid, 'localdir=`pwd`\n');
  fprintf(fid, 'echo "iniciando"\n');
  fprintf(fid, 'export LD_LIBRARY_PATH=${localdir}/..:${localdir}/../src_orig/lib/:${LD_LIBRARY_PATH}\n');
  fprintf(fid, 'echo "${LD_LIBRARY_PATH}"\n');
  fprintf(fid, '../encode_decode options_70_6_3_6_2_map%d.txt\n', val);
  fclose(fid);
  filename=sprintf('options_70_6_3_6_2_map%d.txt',val);

  fid = fopen(filename, 'w');
  fprintf(fid, 'arqinp=<2000,12000000>\n');
  fprintf(fid, 'pchk-file=pchkm_50000_70000_6_3_6_2.dat\n');
  fprintf(fid, 'gen-file=gen_50000_70000_6_3_6_2.dat\n');
  fprintf(fid, 'arqout=result.txt\n');
  fprintf(fid, 'noise-level = 16.2:18:0.1\n');
  fprintf(fid, '\n');
  fprintf(fid, '<QAM>\n');
  fprintf(fid, '#LEVELS\n');
  fprintf(fid, '4\n');
  fprintf(fid, '0.0\n');
  fprintf(fid, '0.0666\n');
  fprintf(fid, '0.1355\n');
  fprintf(fid, '0.2153\n');
  fprintf(fid, '\n');
  fprintf(fid, '<MAP>\n');
  fprintf(fid, '#NBITS\n');
  fprintf(fid, '4\n');
  fprintf(fid, '%s,%s,%s,%s,%s,%s,%s,%s,%s,\n', mapsBin1(1,:), mapsBin1(2,:), mapsBin1(3,:), mapsBin1(4,:), mapsBin1(5,:), mapsBin1(6,:), mapsBin1(7,:), mapsBin1(8,:), mapsBin1(9,:));
  fprintf(fid, '%s,%s,%s,%s\n', mapsBin2(1,:), mapsBin2(2,:), mapsBin2(3,:), mapsBin2(4,:));
  fprintf(fid, '%s,%s\n', mapsBin3(1,:), mapsBin3(2,:));
  fprintf(fid, '%s\n', mapsBin4(1,:));
  fclose(fid);
end

end

