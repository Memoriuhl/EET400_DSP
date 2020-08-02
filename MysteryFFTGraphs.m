Use Dft1.p - Dft6.p as the Magnitude, Phase, Input Waveform, Etc.
			  Just figure out which one is which. Dft3 = Magnitude? Dft4 = Phase?
			

function [ outputArray, mag, waveform ] = Lab5( DFT,samples,numberOfZero,O)

switch DFT
    case 1
        waveform=Dft1(samples);     % The output array of values from Dft1 is saved as a "waveform" 
        mag = abs(fft(waveform));   % Finds the magnitude of the output of a FFT on the waveform
        figure(1);                  % 
        stem(waveform);             % Graphs the waveform
        % Create xlabel
        xlabel('Samples');              
        % Create ylabel
        ylabel('amplitude');

        figure(2);
        stem(mag);                  % Graphs the magnitude of the FFT
        % Create xlabel
        xlabel('frequency');
        % Create ylabel
        ylabel('magnitude');

    case 2
        waveform=Dft2(samples);
        mag = abs(fft(waveform));
        outputArray = [waveform,zeros(1,numberOfZero*3)];
        Zeros = abs(fft(outputArray));
        figure(1);
        stem(waveform);
        % Create xlabel
        xlabel('Samples');
        % Create ylabel
        ylabel('amplitude');

        figure(2);

        stem(mag);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
        

        figure (3);
        stem(Zeros);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
    case 3

        waveform=Dft3();
        mag = abs(fft(waveform));
        outputArray = [waveform,zeros(1,numberOfZero*3)];
        Zeros = abs(fft(outputArray));
        figure(1);
        stem(waveform);
        % Create xlabel
        xlabel('Samples');
        % Create ylabel
        ylabel('amplitude');

        figure(2);
        stem(mag);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
        
        figure (3);
        stem(Zeros);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
    case 4
        waveform=Dft4(samples,O);
        mag = abs(fft(waveform));
        WindowArray = waveform.*transpose(hamming(O)); 
        outputArray = [waveform,zeros(1,numberOfZero*3)];
        Zeros = abs(fft(outputArray));
        window=abs(fft(WindowArray));
        
        figure(1);
        stem(waveform);
        % Create xlabel
        xlabel('Samples');
        % Create ylabel
        ylabel('amplitude');

        figure(2);
        stem(mag);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
        
        figure (3);
        stem(Zeros);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
        
        figure (4);
        stem(window);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
    case 5
        waveform=Dft5();
        mag = abs(fft(waveform));
        WindowArray = waveform.*transpose(hann(O)); 
        outputArray = [waveform,zeros(1,numberOfZero*3)];
        Zeros = abs(fft(outputArray));
        window=abs(fft(WindowArray));
        
        figure(1);
        stem(waveform);
        % Create xlabel
        xlabel('Samples');
        % Create ylabel
        ylabel('amplitude');

        figure(2);
        stem(mag);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
        
        figure (3);
        stem(Zeros);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
        
        figure (4);
        stem(window);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
    case 6
        waveform=Dft6(samples);
        mag = abs(fft(waveform));
        WindowArray = waveform.*transpose(hann(O)); 
        outputArray = [waveform,zeros(1,numberOfZero*3)];
        Zeros = abs(fft(outputArray));
        window = abs(fft(WindowArray));
        
        figure(1);
        stem(waveform);
        % Create xlabel
        xlabel('Samples');
        % Create ylabel
        ylabel('amplitude');

        figure(2);
        stem(mag);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
        
        figure (3);
        stem(Zeros);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
        
        figure (4);
        stem(window);
        % Create xlabel
        xlabel('Bins');
        % Create ylabel
        ylabel('magnitude');
    otherwise
        error('it broke, eat pant')
end




