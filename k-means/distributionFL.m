% https://habr.com/ru/post/506526/
% http://extremelearning.com.au/how-to-evenly-distribute-points-on-a-sphere-more-effectively-than-the-canonical-fibonacci-lattice/
% http://extremelearning.com.au/evenly-distributing-points-on-a-sphere/

n = 1000;
goldenRatio = (1 + sqrt(5)) / 2;
i = 0: n - 1;

% равномерно заполенный квадрат [0;1)x[0;1)
x = mod((i ./ goldenRatio), 1);
y = i ./ n;
% plot(y, x,'Color','#A2142F','LineStyle','none','Marker','h');
% axis([0 1 0 1])

% равномерно заполенная окружность радиуса 1, центр в нуле
phi2d = 2 * pi * x;
r2d = sqrt(y);
x2d = r2d .* cos(phi2d);
y2d = r2d .* sin(phi2d);
% plot(y2d, x2d,'Color','#CD853F','LineStyle','none','Marker','h');
% axis([-1.5 1.5 -1.5 1.5])

% evenly distributed points on a sphere
% Technically speaking, it is impossible to equidistribute points on a sphere 
% unless they are the corners of a Platonic solid, so N = 4,6,8,12,20
if (n >= 600000)
  epsilon = 214;
elseif (n >= 400000)
  epsilon = 75;
elseif (n >= 11000)
  epsilon = 27;
elseif (n >= 890)
  epsilon = 10;
elseif ( n>= 177)
  epsilon = 3.33;
elseif (n >= 24)
  epsilon = 1.33;
else
  epsilon = 0.33;
end

%x__ = (i + epsilon) / (n - 1 + 2 * epsilon);
%y__ = mod(i / goldenRatio, 1);
%phi = 2 * pi * x__;
%theta = acos(1 - 2 * y__);
phi = 2 * pi * x;
theta = acos(1 - 2 * y);
% to perform elementwise multiplication, use '.*'
x_ = (cos(phi)) .* (sin(theta));
y_ = sin(phi) .* sin(theta);
z_ = cos(theta);

plot3(x_,y_,z_,'Color','#A2142F','LineStyle','none','Marker','.');
axis([-1.2 1.2 -1.2 1.2 -1.2 1.2])
xlabel('x') 
ylabel('y') 
zlabel('z')

