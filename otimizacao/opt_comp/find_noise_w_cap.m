function [ noisedb ] = find_noise_w_cap( levels, dimming, initialnoise, cap )
% This function calculate the noisedb in db that with this levels and
% dimming cause the 
%

%cap=calc_max_cap(levels, dimming, initialnoise);

[noisedb,FVAL,EXITFLAG] = fzero(@(n) calc_max_cap(levels, dimming, n)-cap,...
    [0 50]);

fprintf(1, 'noisedb=%f - FVAL=%f - flag=%d\n', noisedb, FVAL, EXITFLAG);

end
