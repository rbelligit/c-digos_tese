function graph_noise_cap( levels, cap )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
dimming=[0.02 0.03 0.05:.05:0.4 0.42 0.44 0.46 0.48 0.49 0.50 0.51 ...
    0.52 0.54 0.56 0.58 0.6:0.05:.95 0.97 0.98];
noisedb=zeros(1,length(dimming));
for i=1:length(dimming)
     noisedb(i) = find_noise_w_cap( levels, dimming(i), 0, cap );
end

plot(dimming, noisedb);

save(sprintf('vars/noise_cap_%d_levels_%dpam.mat', floor(cap*100), levels), 'dimming', 'noisedb');

end

