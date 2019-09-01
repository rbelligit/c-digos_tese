function [ cap ] = limite_sup1(  A, sigma, alpha )
%LIMITE_SUP1 Summary of this function goes here
%   Detailed explanation goes here
cap = 0.5*log2(1+alpha*(1-alpha)*A^2/sigma^2);

end

