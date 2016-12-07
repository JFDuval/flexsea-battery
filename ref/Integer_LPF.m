
%Luke Mooney
%2016-09-21
% Modified by JFDuval, 09/22/2016
%LPF for integer math

%Filt is a 2x1 array where Filt(1) is the most recent filtered value
%and Filt(2) is the last filtered value. Raw is also a 2x1 array of raw
%signal values where Raw(1) is the most recent raw signal value. This
%scripts finds the integer values of the coefficient a and b in the
%following equation:
%
%Filt(1) = b(1)*Raw(1) + b(2)*Raw(2) - a(2)*Filt(2)
%
%Note that Filt will be filtering (Raw * 2^10) in order to maintain precision
%during integer math. 

clc;
clear all;
close all;

pkg load signal

SF = 1000; %Sampling Frequency
CF = 2.5; %Cutoff Frequency
Order = 1; %filter order, Filt & Raw array size increases with order

[b,a] = butter(Order,CF/(SF/2));%1st order Butterworth LPF

t=0:1/SF:2;

baseline = 24000;   %mV
noise = 500;
drop_t = 0.1;   %s
drop_t_samples = drop_t/(1/SF);  
dropV = 0.1*baseline;
drop_startSample = int32(0.5*length(t));
input = baseline*ones(1,length(t));

%Drop #1: 1ms
drop_t = 0.001;   %s
drop_t_samples = drop_t/(1/SF);  
dropV = 0.1*baseline;
drop_startSample = int32(0.25*length(t));
input(drop_startSample:drop_startSample+drop_t_samples) = dropV;

%Drop #1 B: 1ms
drop_startSample = int32(0.35*length(t));
input(drop_startSample:drop_startSample+drop_t_samples) = dropV;
%Drop #1 B: 1ms
drop_startSample = int32(0.36*length(t));
input(drop_startSample:drop_startSample+drop_t_samples) = dropV;
%Drop #1 B: 1ms
drop_startSample = int32(0.37*length(t));
input(drop_startSample:drop_startSample+drop_t_samples) = dropV;

%Drop #2: 10ms
drop_t = 0.01;   %s
drop_t_samples = drop_t/(1/SF);  
dropV = 0.1*baseline;
drop_startSample = int32(0.5*length(t));
input(drop_startSample:drop_startSample+drop_t_samples) = dropV;

%Drop #3: 50ms
drop_t = 0.05;   %s
drop_t_samples = drop_t/(1/SF);  
dropV = 0.5*baseline;
drop_startSample = int32(0.6*length(t));
input(drop_startSample:drop_startSample+drop_t_samples) = dropV;

%Drop #4: 100ms
drop_t = 0.1;   %s
drop_t_samples = drop_t/(1/SF);  
dropV = 0.75*baseline;
drop_startSample = int32(0.85*length(t));
input(drop_startSample:drop_startSample+drop_t_samples) = dropV;

%Add noise:
input = input + noise.*randn(1,length(t));

% Apply the filter to the input signal and plot input and output.
output=filter(b,a,input);
plot(t, input, 'b')
hold on
plot(t, output, 'g')

%Improvment over my 32-samples moving average?
mvavg = zeros(1, length(t));
mvavgLen = 32;
for i = 1 : length(t)

  if( i  < mvavgLen+1)
  
    mvavg(i) = input(i);
    
  else  
    chunk = input(i-mvavgLen+1:i);
    mvavg(i) = sum(chunk)/mvavgLen;
  end

end

plot(t, mvavg, 'r')

%For integer use:
b = round(b*2^20)
a = round(a*2^10)