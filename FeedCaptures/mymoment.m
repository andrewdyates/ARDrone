function m = mymoment( A, p, q )
% A: 2D image array
% p: x order
% q: y order

m = 0;
% WARNING: (A,1) is ROWS(y), (A,2) is COLS(x)
for i = 1:size(A,2)
    for j = 1:size(A,1)
        m = m + i^p * j^q * A(j, i);
    end
end

end
