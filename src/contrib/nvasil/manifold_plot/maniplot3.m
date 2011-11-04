function maniplot3(x, xcolor)
figure;
hold on
[len,dim]=size(x);
for i=1:len
    h=plot3(x(i,1), x(i,2), x(i,3), 'o');
    set(h, 'Color', [xcolor(i), xcolor(i), xcolor(i)]);
    set(h, 'MarkerFaceColor', [min(xcolor(i)+0.1,0.8), min(xcolor(i)^2,0.8),...
        0]);
    set(h, 'MarkerEdgeColor','none');
end
hold off
  