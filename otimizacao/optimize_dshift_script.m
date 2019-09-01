
nlevels = [2 3 4 5 6];
dimming = 0.2;

type = ['-','-x','o','+','d']

figure();

for i=1:length(nlevels)
    
    res1 = optimize_dshift_range(5, 25, 20*4, dimming, nlevels(i));
    res1.cap
    plot(res1.ruido, -res1.cap, type(i));
    axis([5 20 0 2.7]);
    hold on;
    
end

figure();
res1 = optimize_dshift_range(5, 25, 20*4, dimming, 4);

res2 = optimize_probs_range(5, 25, 20*4, 4, dimming);

plot(res1.ruido, -res1.cap, '-r');
cap1 = zeros(1,length(res2));

for i=1:length(res2)
    cap1(i)=res2(i).cap;
end
hold on;
plot(res1.ruido, cap1);