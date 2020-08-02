function [result,t1,t2, outputF1, outputF2 ] = ArrayMultiplyAndSum(f1,f2)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
fs = 1000;
%f1 = 100;
%f2 = 100;
N1 = (fs/f1)+1;
N2 = (fs/f2)+1;

c = min(N1,N2);

time1 = N1 * 1/fs;
time2 = N2 * 1/fs;
phi = 0;
phi2 = 0;
t1 = 0: 1/fs: time1-1/fs;
t2 = 0: 1/fs: time1-1/fs;
outputF1 = sin(2*pi*f1*t1+phi);
outputF2 = sin(2*pi*f2*t2+phi2);
multiplyF = outputF1.*outputF2; % create an array with the multiply result
result = outputF1*transpose(outputF2);

plot(1);
axis([0 time1 -inf inf]);
%xlim([0 time1 -1 inf]);
% Create xlabel
xlabel('time');
% Create ylabel
ylabel('amplitude');
hold on; % place the following plots all on the same plot;
plot(t1,outputF1,'blue');  % plot correct number of cycles with time axis
plot(t2,outputF2,'red');
plot(t1,multiplyF,'green');
stem(t1,outputF1,'blue');  % plot correct number of cycles with time axis
stem(t1,outputF2,'red');
stem(t1,multiplyF,'green');
hold off;
fprintf('N - %i',c)
%disp(c);
end
