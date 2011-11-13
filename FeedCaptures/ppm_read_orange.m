N = 13;
Imgs = zeros(240,320,3,N);
for i=1:N
    file = sprintf('Orange in Lab/it/pixbuff_%d.ppm', i);
    Imgs(:,:,:,i) = imread(file);
end

% verify myrgb2hsv works as expected
% [h s v] = myrgb2hsv(Img(1,1,:))
sigma = 2;
G = fspecial('gaussian', 2*ceil(3*sigma)+1, sigma);


% test threshold
Masks = zeros(240,320,N);
for i = 1:N
    img = uint8(Imgs(:,:,:,i));
    img = imfilter(img, G, 'replicate');
    HSVImg = rgb2hsv(img);

    Mask = zeros(240,320);
    for row = 1:size(Masks,1)
        for col = 1:size(Masks,2)
           hue = HSVImg(row,col,1);
           sat = HSVImg(row,col,2);
           val = HSVImg(row,col,3);
           % highlights
           if val >= .99 && (hue < 30/360 || hue > 355/360) ...
               && sat > .1
               Mask(row,col) =1;
           end
           if (hue > 5/360 && hue < 15/360) && sat > .55
               Mask(row,col) =1;
           end
           if (hue < 25/360 || hue > 358/360) && sat > .6
               Mask(row,col) =1;
           end
           % shadows
           if (val < .6 && val > .3) ...
                   && (hue < 20/360 || hue > 355/360) && sat > .65
               Mask(row,col) =1;
           end
        end
    end
    % clean mask
    Mask = medfilt2(Mask);
    Mask = bwmorph(bwmorph(Mask, 'dilate'), 'dilate');
    Mask = bwmorph(bwmorph(Mask, 'erode'), 'erode');
    % get largest connected component (assume this doesn't need to be done)
    % compute center of mass
    m10 = mymoment(Mask,1,0);
    m01 = mymoment(Mask,0,1);
    m00 = mymoment(Mask,0,0);
    if m00 > 0
        x = m10/m00;
        y = m01/m00;
    else
        x = NaN; y = NaN;
    end
    subplot(1,2,1);
    imshow(Mask);
    subplot(1,2,2);
    imshow(uint8(img));
    hold on
    plot(x, y, 'g*');
    hold off
    pause;
end

