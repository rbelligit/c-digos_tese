close all;
plot(xx,demtobit,'r');
hold on
plot(bittodem,xx, 'b');

xlabel('I_{B \rightarrow D}');
ylabel('I_{D \rightarrow B}');

posx = 0;
posy=0;

posVect = zeros(2,1000);
posVect(:,1) = [posx ; posy];
index = 2;
contval = true;
while(contval)
    posy = approx(xx,demtobit,posx);
    if(posy > 1)
        posy = 1;
        contval = false;
    end
    posVect(:, index) = [posx ; posy];
    index = index + 1;
    posx = approx(xx, bittodem, posy);
    if(posx > 1)
        posx = 1;
        contval=false;
    end
    posVect(:, index) = [posx ; posy];
    index = index+1;
    fprintf('posx=%f - posy=%f\n', posx, posy);
    if(index > 50)
        contval = false;
    end
end
index = index - 1;
plot(posVect(1,1:index),posVect(2,1:index))

grid on


function val = approx(x,y,val)
    index = find(x>val,1);
    if(isempty(index))
       val = (val - x(end)) * ...
           (y(end)-y(end-1)) / (x(end)-x(end-1)) + ...
           y(end);
    elseif(index == 1)
        val = (val - x(1)) * ...
           (y(1)-y(2)) / (x(1)-x(2)) + ...
           y(1);
    else
        val = (val - x(index-1)) * ...
            (y(index)-y(index-1)) / (x(index) - x(index-1)) + ...
            y(index-1);
    end

end
