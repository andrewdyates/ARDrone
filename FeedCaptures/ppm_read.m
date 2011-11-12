Img = imread('Fluorescent/fluorescent_low_left_far.ppm');
HSVImg = zeros(size(Img));

% I will compare this with my own function
hsv_image = rgb2hsv(rgb_image);
