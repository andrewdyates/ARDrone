% % attempt to display pixel buffer
%
% % this is a mirror image
% load pixbuf_rows2.txt;
% img = reshape(pixbuf_rows2, 320, 240, 3);
% img = imrotate(img, -90, 'nearest');
% imshow(uint8(img))

load pixbuf_rows2.txt;
A = zeros(240, 320, 3);
for i=0:size(pixbuf_rows2,1)-1
    row = floor(i/320) + 1;
    col = mod(i, 320) + 1;
    A(row,col,:) = pixbuf_rows2(i+1,:);
end
imshow(uint8(A))