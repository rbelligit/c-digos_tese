%Calcula a melhor M para cada nível de ruído e para cada fator de dimming.

function [m, fval, eta, probs] = find_opt_ndshift_m(noisedb, dimming, mini, minval)
    noise = 10^(-noisedb/10);
    M = mini;
    capAt = 0;
    valst = [0 0];
    while(1)
       [ vals_c, fval_c ] = optimize_dshitf( M, noise, dimming, minval);
       %disp(vals_c)
       
       a = min(vals_c(2:end));
       b = min([vals_c(end) vals_c(2)]);
       %fprintf('M=%d - fval=%f - noise=%f a=%f - b=%f-\n ', M, fval_c, noise, a, b);
       if(fval_c < (capAt * 1.0001) || a < (b/3))
           m = M-1;
           fval = capAt;
           eta = valst(1);
           probs = valst(2:end);
           return;
       end
       capAt = fval_c;
       valst = vals_c;
       M=M+1;
    end
end
