function [ h s v ] = myrgb2hsv( A )
%Convert Red Green Blue pixels to Hue Saturation Value
% return as percentages like native rgb2hsv function
%A = [r g b];
A = double(A);
r = A(1); g = A(2); b = A(3);
M = max(A);
m = min(A);

% Chroma
C = M-m;

% Brightness (Value)
v = M / 255;

% Saturation
if M > 0
    s = C/M;
else
    s = 0;
end

% Hue
if C == 0
    h = 0;
end
if M == r
    h = mod((g-b)/C, 6);
end
if M == g
    h = (b-r)/C + 2;
end
if M == b
    h = (r-g)/C + 4;
end
h = h / 6;


        
end

