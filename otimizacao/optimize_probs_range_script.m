


dimmings = [0.05 0.1 0.15 0.2 0.25 0.3 0.35 0.4 0.45];
ndim = length(dimmings);
respF = struct;

for i=1:ndim

    resp = optimize_probs_range(2, 10, 48, 40, dimmings(i));
    
    respf.resp=resp;
    respf.dimming=dimmings(i);
    respVal{i}=respf;
end

save('result_dimmings_005_045.mat', 'respVal'); 