function [ outMag ] = RectangleDFT(oddNumOnes, evenNumZeros  )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
zArray = zeros(1,evenNumZeros);
oArray = ones(1,oddNumOnes);
a = [zArray, oArray, zArray];

output = fft(a);
ifout = ifft(output);

outMag = abs(output);
stem(outMag);
figure(2)
t1 = (oddNumOnes+evenNumZeros*2)/2;
t2 = round(t1);
b = [outMag(t2+1:oddNumOnes+evenNumZeros*2),outMag(1:t2)];
stem(b);

figure(3)
stem(ifout);
xlabel('time');
ylabel('amplitude');
end




function [ outputArray] = DFT2( frequency1,f1Amp, frequency2, f2Amp, numberOfSamples, zeroMultiplier, windowType )
% frequency1 is the freq with amplitude of f1Amp
% frequency2 is the freq with amplitude of f2Amp
% numberOfSamples is the number of samples user desires
% zeroMultiplier is a value that ranges from 0 to n. The number of zeros
%          added to the data is the numberOfSamples*zeroMultiplier
% windowOn - 'rectangle' defaults to rectangular window
%            'hamming' value windows the data with a Hamming window.
%            'triangle' value windows the data with a triangle window
%
% as a start, try this function DFT2(2010,1,2250,0.1,100,3,'rectangle');
%  zero padding can be modified and windowing can be used or not
% to view the DFT of a window function, make the freq of frequency1 = 0 and
% the amplitude of frequency2 = 0
% DFT2(0,1, 2290,0, 100, 5, 'rectangle');  % this will show the DFT of the
% rectangular window with the required zero padding to view it correctly

sampleFrequency = 1000; 
%numberOfSamples = 200; 
binFreq = sampleFrequency/numberOfSamples;
%frequency1 = 2010;
%frequency2 = frequency1 + binFreq*3;
phaseAngle = 0;

%zeroMultiplier = 3;
%time = numberOfSamples * 1/sampleFrequency;
%t = 0: 1/sampleFrequency: time-1/sampleFrequency; % loop variable
ts = 1/sampleFrequency;  % define sample period
n = 0: 1 : numberOfSamples -1; % array of integers, one for each of the input samples
outputArray1 = f1Amp*cos(2*pi*frequency1*n*ts + phaseAngle); % compute the input data
outputArray2 = f2Amp*cos(2*pi*frequency2*n*ts + phaseAngle); % compute the input data
outputArray = outputArray1 + outputArray2;
switch (windowType) 
    case 'rectangle'
        outputArray = outputArray.*transpose(rectwin(numberOfSamples)); 
    case 'hamming'
        outputArray = outputArray.*transpose(hamming(numberOfSamples));  %(rectwin(numberOfSamples));
    case 'triangle'
        outputArray = outputArray.*transpose(triang(numberOfSamples)); 
    otherwise
        outputArray = outputArray.*transpose(rectwin(numberOfSamples)); 
end
outputArray = [outputArray,zeros(1,numberOfSamples*zeroMultiplier)];
% create array of values for the frequency (x) axis
xaxisLabel = 0 : sampleFrequency/(numberOfSamples*(zeroMultiplier+1)) : sampleFrequency - sampleFrequency/(numberOfSamples*(zeroMultiplier+1));

% compute the dft of the input data array
ifftinput = ifft(outputArray);
dftOutput = fft(ifftinput); % compute the DFT
magnitude = abs(dftOutput);  % magnitude of the DFT result
phase = angle(dftOutput);
% clear out the useless phase values
phase = phase.*lt(abs(phase),magnitude);
% plot of real part of DFT output
figure(1);
stem(xaxisLabel,real(dftOutput));
% Create xlabel
xlabel('frequency');
% Create ylabel
ylabel('real');
% plot of imaginary part of DFT output
figure(2);
stem(xaxisLabel,imag(dftOutput));
% Create xlabel
xlabel('frequency');
% Create ylabel
ylabel('imaginary');
% plot of magnitude of DFT output
figure(3);
stem(xaxisLabel,magnitude);
% Create xlabel
xlabel('frequency');
% Create ylabel
ylabel('magnitude');
% plot of phase (radians) of DFT output
figure(4);
stem(xaxisLabel,phase);
% Create xlabel
xlabel('frequency');
% Create ylabel
ylabel('phase');
end


