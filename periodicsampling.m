function [ outputArray ] = SamplingEval(frequency, sampleFrequency, NumberOfSamples)
% semicolon at end of line suppresses the data displaying in the command
% window. When semicolon is missing, Matlab underlines the equal sign

time = NumberOfSamples * 1/sampleFrequency;
t = 0: 1/sampleFrequency: time-1/sampleFrequency; % loop variable

outArrayL = cos(2*pi*frequency*t);   % Generate a sinusoidal wave with given freq in the left earbud.
outArrayR = cos(2*pi*frequency*2*t); % Generate a wave with 2x the given freq in the right earbud.

yL = outArrayL;                      % Set the outputs to a variable
yR = outArrayR;                      % without affecting original vars.

yStereo = [yL;yR];                   % Put both sinusoidal waves into an array.

sound(yStereo, sampleFrequency);     % Output the stereo sounds.
stem(yStereo);                       % Graph the stereo sound.
end
