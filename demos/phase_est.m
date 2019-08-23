clc;
eps = 1;
shift = 2;

for phases = [-42.2,0,42.2]
    
    N = 2048;
    M = 128;
%     dataR = randn(M,1).*(2^1);
%     data = [zeros(N/2-M/2,1); dataR; zeros(N/2-M/2,1)];
    data = randn(N,1).*(2^1);
    dataC = (complex(data,data));
    dataCref = dataC;
    ff = comm.PhaseFrequencyOffset;
    ff.SampleRate = 1e6;
    ff.PhaseOffset = phases;
    
    % With zero sample delay the peak should be at length(dataC_shift) since
    % the correlated output is length(dataC_shift)+length(dataC) - 1
    %% FFT method
    %%
    ff.release();
    dataC_shift = ff(dataC);
    l = length(dataC_shift);
    l = 0;
    pad_dataC_shift = [zeros(l,1); dataC_shift];
    pad_dataC = [zeros(l,1); dataC];
    write_complex_binary(pad_dataC,'ref.data');
    write_complex_binary(pad_dataC_shift,'shifted.data');
    
    corRfft = ifft(fft(pad_dataC_shift) .* conj(fft(pad_dataC)));
    corRfft = fftshift(corRfft);
    
    [~,i] = max(abs(corRfft));
    v = corRfft(i);
    
    i = length(dataC)/2 + 1 - i;
    
    fprintf('Sample Delay %d (%d,%d)| Phase Shift %f \n',i,length(dataC),length(dataC_shift),angle(v)*180/pi);
    if abs(angle(v)*180/pi-phases)>eps
        error('Phase error');
    end
    dataC = dataCref;
    % GR
    !python2 top_block.py
%     mag = read_complex_binary('mag.data');
%     mag = reshape(mag,N,length(mag)/N);
%     stem(fftshift(  mag(:,1)  ));
    
    loc = read_float_binary('out.data');
    phase = read_float_binary('phase.data');
    phase = phase(100:end);
    loc = loc(100:end);
    fprintf('GR: Sample Delay %d (%d,%d)| Phase Shift %f \n',mean(loc),length(dataC),length(dataC_shift),mean(phase));
    if abs(mean(phase)-phases)>eps
        error('Phase error');
    end
    %%
    ff.release();
    dataC_shift = [zeros(shift,1);ff(dataC(1:end-shift))];
    l = length(dataC_shift);
    l = 0;
    pad_dataC_shift = [zeros(l,1); dataC_shift];
    pad_dataC = [zeros(l,1); dataC];
    
    write_complex_binary(pad_dataC,'ref.data');
    write_complex_binary(pad_dataC_shift,'shifted.data');
    
    corRfft = ifft(fft(pad_dataC_shift) .* conj(fft(pad_dataC)));
    corRfft = fftshift(corRfft);
    
    [~,i] = max(abs(corRfft));
    v = corRfft(i);
    i = length(dataC)/2 + 1 - i;
    
    fprintf('Sample Delay %d (%d,%d)| Phase Shift %f \n',i,length(dataC),length(dataC_shift),angle(v)*180/pi);
    if abs(angle(v)*180/pi-phases)>eps
        error('Phase error');
    end
    dataC = dataCref;
    % GR
    !python2 top_block.py
    loc = read_float_binary('out.data');
    phase = read_float_binary('phase.data');
    phase = phase(100:end);
    loc = loc(100:end);
    fprintf('GR: Sample Delay %d (%d,%d)| Phase Shift %f \n',mean(loc),length(dataC),length(dataC_shift),mean(phase));
    if abs(mean(phase)-phases)>eps
        error('Phase error');
    end
    %%
    ff.release();
    dataC_shift = ff(dataC);
    dataC = [zeros(shift,1);dataC(1:end-shift)];
    l = length(dataC_shift);
    l = 0;
    pad_dataC_shift = [zeros(l,1); dataC_shift];
    pad_dataC = [zeros(l,1); dataC];
    
    write_complex_binary(pad_dataC,'ref.data');
    write_complex_binary(pad_dataC_shift,'shifted.data');
    
    corRfft = ifft(fft(pad_dataC_shift) .* conj(fft(pad_dataC)));
    corRfft = fftshift(corRfft);
    
    [v,i] = max(abs(corRfft));
    v = corRfft(i);
    i = length(dataC)/2 + 1 - i;
    
    fprintf('Sample Delay %d (%d,%d)| Phase Shift %f \n',i,length(dataC),length(dataC_shift),angle(v)*180/pi);
    if abs(angle(v)*180/pi-phases)>eps
        error('Phase error');
    end
    dataC = dataCref;
    % GR
    !python2 top_block.py

    loc = read_float_binary('out.data');
    phase = read_float_binary('phase.data');
    phase = phase(100:end);
    loc = loc(100:end);
    fprintf('GR: Sample Delay %d (%d,%d)| Phase Shift %f \n',mean(loc),length(dataC),length(dataC_shift),mean(phase));
    if abs(mean(phase)-phases)>eps
        error('Phase error');
    end
    %% Correlation
    %%
    ff.release();
    dataC_shift = ff(dataC);
    [corR,lagR] = xcorr(dataC_shift,dataC);
    [v,i] = max(corR);
    
    i = length(dataC) - i;
    
    fprintf('Sample Delay %d (%d,%d)| Phase Shift %f \n',i,length(dataC),length(dataC_shift),angle(v)*180/pi);
    if abs(angle(v)*180/pi-phases)>eps
        error('Phase error');
    end
    dataC = dataCref;

    %%
    ff.release();
    dataC_shift = [zeros(shift,1);ff(dataC(1:end-shift))];
    [corR,lagR] = xcorr(dataC_shift,dataC);
    [v,i] = max(corR);
    
    i = length(dataC) - i;
    
    fprintf('Sample Delay %d (%d,%d)| Phase Shift %f \n',i,length(dataC),length(dataC_shift),angle(v)*180/pi);
    if abs(angle(v)*180/pi-phases)>eps
        error('Phase error');
    end
    dataC = dataCref;

    %%
    ff.release();
    dataC_shift = ff(dataC);
    dataC = [zeros(shift,1);dataC(1:end-shift)];
    [corR,lagR] = xcorr(dataC_shift,dataC);
    [v,i] = max(corR);
    
    i = length(dataC) - i;
    
    fprintf('Sample Delay %d (%d,%d)| Phase Shift %f \n',i,length(dataC),length(dataC_shift),angle(v)*180/pi);
    if abs(angle(v)*180/pi-phases)>eps
        error('Phase error');
    end
    dataC = dataCref;
    fprintf('--------------------\n');
end