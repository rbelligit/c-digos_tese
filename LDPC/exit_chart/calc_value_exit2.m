function [val] = calc_value_exit(mapeamento, noise, levels)
% Summary of this function goes here
% Detailed explanation goes here
ruido = 10^(-noise/10);
lav = 0:0.05:0.95;
result = zeros(1,length(lav));
ii=1;
val = 0.0;
for la = lav
    result(ii) = mex_calc_exit(mapeamento, levels, la, ruido, 20000);
    ii = ii+1;
end

val = mean(result(1:(end/2)));


%Ler = [];
%Le2r = [];

%for La1 = lav
%    [Le, Le2] = calcExitDecInc_mex(mapeamento, levels, La1, ruido);
%    Ler=[Ler Le];
%    Le2r=[Le2r Le2];
%end

%val(2) = mean(Ler(1:end-1));
%val(2)=val(1);

end

