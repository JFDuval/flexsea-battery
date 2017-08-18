% I2t current limit: user
% JFDuval, Dephy, Inc. 06/14/2017

% Note: use this file to get your #define values. i2t_1.m can be used to test
% your values with different profiles.

clc;
close all;
clear all;
format short eng

% Period, i2t_compute() call:
dt = 100e-3;    %Every 100ms

disp('Program these values in i2t-current-limit.h:')
disp('============================================')

% The algorithm uses 8bit values: we need to scale down the maximum current
% accordingly.
% Ex.: if you read ±11bits (±2048), 2048/256 = 8, so shift 3
% In this project, we use ctrl.current.actual_val, an int32 value. 
% It is limited to ±30000mA (max that our sensor can read)
% 30000/256 = 117 => shift 7 (div by 128). 30A will give us 234

I2C_SCALE_DOWN_SHIFT = 7  % Closest bitshift available
SCALE = 128;              % (Octave only, avoids bitwise operations)

% Maximum average current you want to support:
maxAvgCurrent = 6000;    %mA
I2T_LEAK = (maxAvgCurrent / SCALE)^2

% Maximum current you want to support, and duration:
currentLimit = 10000;     %mA
currentLimitTime = 3;   %s
I2T_LIMIT = (currentLimitTime / dt) * ((currentLimit/SCALE)^2 - I2T_LEAK)

%At what fraction of the max to you want a warning? (0-1)
warn = 0.8;
I2T_WARNING = warn * I2T_LIMIT

% Plotting:
CURR = 6000:1000:25000;
tmp = (CURR./SCALE).^2;
time = (I2T_LIMIT * dt) ./ ( tmp - I2T_LEAK );
figure()
plot(CURR, time)
title('Time before reaching the I2t limit')
xlabel('Current (mA)')
ylabel('Time (s)')