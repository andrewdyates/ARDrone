%Img = imread('Fluorescent/fluorescent_low_left_far.ppm');
%Img = imread('Fluorescent/fluorescent_clipped_top.ppm');
%Img = imread('Fluorescent/fluorescent_close_left_bottom.ppm');
%Img = imread('Fluorescent/big blur.ppm');
%Img = imread('Natural Light/pixbuf_275.ppm');
%Img = imread('Natural Light/pixbuf_109.ppm');
%Img = imread('Natural Light/pixbuf_25.ppm');
%Img = imread('Outdoors/outdoors_ctrl2.ppm');
%Img = imread('Outdoors/outdoors_mid_left.ppm');
Img = imread('Outdoors/pixbuf_126.ppm');

% I need readings for inside the classroom


%HSVImg = zeros(size(Img));

% I will compare this with my own function
HSVImg = rgb2hsv(Img);

% verify myrgb2hsv works as expected
% [h s v] = myrgb2hsv(Img(1,1,:))
% HSVImg(1,1,:)

% test threshold
Mask = zeros(size(Img));
for row = 1:size(Mask,1)
    for col = 1:size(Mask,2)
        % saturation .35 basement, .2 for outdoors
        if HSVImg(row,col,1) > 70/360 ...
            && HSVImg(row,col,1) < 140/360 ...
            && HSVImg(row,col,2) > .2 ...
            && HSVImg(row,col,2) < .8 ...
            && HSVImg(row,col,3) > .25
            Mask(row, col) = 1;
        end
        % handle ball glow
        if HSVImg(row,col,1) > 70/360 ...
            && HSVImg(row,col,1) < 140/360 ...
            && HSVImg(row,col,2) > .1 ...
            && HSVImg(row,col,3) > .90
            Mask(row, col) = 1;
        end
    end
end

imshow(Mask);