filename = '/Users/georgemilyuchickin/CLionProjects/clusterization/cmake-build-debug/OUTPUT.txt';
fileID = fopen(filename);
C = textscan(fileID,'%d %f %f %d');
fclose(fileID);
% whos C
id = C{1};
x = C{2};
y = C{3};
cluster_number = C{4};

N = length(x);
k = cluster_number(N);
figure('Color','w','Name','kmeans','NumberTitle','off');

%plot(x,y,'bo')
hold on

filename2 = 'colours.txt';
fileID2 = fopen(filename2);
C2 = textscan(fileID2,'%s %s');
fclose(fileID2);
colours = string(C2{2});

% вывод узлов
for i = 1:(N)
    clr = colours(cluster_number(i) + 1);
    %fprintf('%s\n', str);
    if (rem(cluster_number(i),2) == 1)
        plot(x(i),y(i),'Marker','h','Color', clr)
    else
        plot(x(i),y(i),'Marker','p','Color', clr)
    end
end

hold off

    