filename = '/Users/georgemilyuchickin/CLionProjects/clusterization/cmake-build-debug/SS(k).txt';
fileID = fopen(filename);
C = textscan(fileID,'%d %f');
fclose(fileID);
whos C
k = C{1};
S = C{2};
figure('Color','w','Name','SS','NumberTitle','off');
plot(k,S,'Color','#5F9EA0')
xlabel('k') 
ylabel('SS') 

