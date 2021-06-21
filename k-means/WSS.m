filename = '/Users/georgemilyuchickin/CLionProjects/clusterization/cmake-build-debug/WSS.txt';
fileID = fopen(filename);
C = textscan(fileID,'%d %d');
fclose(fileID);
whos C
k = C{1};
W = C{2};
figure('Color','w','Name','Within Set Sum of Squared Errors','NumberTitle','off');
plot(k,W,'Color','#5F9EA0')
xlabel('k') 
ylabel('WSS') 
