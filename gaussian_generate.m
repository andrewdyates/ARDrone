s = zeros(1,13);
sigma = 2;
q = 2*sigma^2;
c = 1/(sqrt(2*pi*sigma^2));
for i = -6:6
    e = exp(-i^2/q);
    s(i+7) = c*e;
end
s