clear data;
close all;
clear all;
%% File name

filename = ' '; % input file name


%% Zoom
window = 0; % 0 - full file, 1 - specific time window

begin = 234;   % time window size (seconds)
finish = 248;

%% Data read

path =  'file path';
path = strcat(path,filename,'.txt');
fid = fopen(path);
data = textscan(fid, '%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f','MultipleDelimsAsOne',true,'Delimiter',',','HeaderLines',0);
fclose(fid);

time = data{1,3};            % time stamp
sampleTime = data{1,2};      % FNSW window time 
sample = data{1,1};          % Number of sample points collected in FNSW window`  
AccX = data{1,4};            % Acceleration X-axis
AccY = data{1,5};            % Acceleration Y-axis 
AccZ = data{1,6};            % Acceleration Z-axis
GyrX = data{1,7};            % Gyroscope X-axis  
GyrY = data{1,8};            % Gyroscope Y-axis
GyrZ = data{1,9};            % Gyroscope Z-axis
PeakY = data{1,12};          % Peaks detected for Y-axis
PeakY_time = data{1,13};     % Y-axis detected peaks time stamp
PeakZ = data{1,14};          % Peaks detected for Z-axis  
PeakZ_time = data{1,15};     % Z-axis detected peaks time stamp
ewma_ay = data{1,17};        % EWMA output for accelerometer Y-axis 
ewma_az = data{1,18};        % EWMA output for accelerometer Z-axis  

row = [length(time),length(sample),length(AccX),length(AccY),length(AccZ),length(GyrX),length(GyrY),length(GyrZ),length(PeakY),length(PeakY_time),length(PeakZ),length(PeakY_time)];
row = min(row);

%% Time Window

if( window == 0)
    start = 1;
    stop = row;

else
    begin = begin*10^6;
    finish = finish*10^6;
    
    posBeg = find( (begin-1000000) <= time(1:row) & (begin+1000000)>=time(1:row));
    posFin = find( (finish-1000000) <= time(1:row) & (finish+1000000)>=time(1:row));
    
    start = posBeg(1,1);

    stop = posFin(1,1);
    
end

%% Slots - Identifying the window starting time

zero = find(sample == 0);
sampleFlag(length(zero),1) = 0;
j = 1;
for i = 1:length(zero)
    sampleFlag(j,1) = sampleTime(zero(i,1),1);
    j = j+1;
end
    

%% Plots

unit = 1;

figure
subplot(3,1,1)
hold on
plot(time(start:stop,1)*unit,AccX(start:stop,1),'-');
title('AccX');
xlabel('t (s)')
ylabel("m/s^2")
hold off
subplot(3,1,2)
hold on
plot(time(start:stop,1)*unit,AccY(start:stop,1),'-');
title('AccY');
xlabel('t (s)')
ylabel("m/s^2")
hold off
subplot(3,1,3)
hold on
plot(time(start:stop,1)*unit,AccZ(start:stop,1),'-');
title('AccZ');
xlabel('t (s)')
ylabel("m/s^2")
hold off

% Gyroscope

figure
subplot(3,1,1)
hold on
plot(time(start:stop,1)*unit,GyrX(start:stop,1),'-');
title('GyrX');
xlabel('t (s)')
ylabel("°/s")
hold off
subplot(3,1,2)
hold on
plot(time(start:stop,1)*unit,GyrY(start:stop,1),'-');
title('GyrY');
xlabel('t (s)')
ylabel("°/s")
hold off
subplot(3,1,3)
hold on
plot(time(start:stop,1)*unit,GyrZ(start:stop,1),'-');
title('GyrZ');
xlabel('t (s)')
ylabel("°/s")
hold off

%% FNSW - Y-axis

PeakY_time(PeakY_time == 0) = NaN;
PeakZ_time(PeakZ_time == 0) = NaN;
PeakY(PeakY == 0) = NaN;
PeakZ(PeakZ == 0) = NaN;

figure

hold on

plot(time(start:stop,1)*unit,ewma_ay(start:stop,1),'-');
plot(PeakY_time(start:stop,1)*unit,PeakY(start:stop,1),'+r','markersize',10);
for i =1:length(sampleFlag)
    line([sampleFlag(i,1)*unit sampleFlag(i,1)*unit], [8.9 9.15],'Color','black','LineStyle','--');
end
title('AccY');
xlabel('t (s)')
ylabel("m/s^2")
hold off

%% FNSW - Z-axis

figure

hold on

plot(time(start:stop,1)*unit,ewma_az(start:stop,1),'-');

plot(PeakZ_time(start:stop,1)*unit,PeakZ(start:stop,1),'+r','markersize',10);
for i =1:length(sampleFlag)
    line([sampleFlag(i,1)*unit sampleFlag(i,1)*unit], [1.35 1.7],'Color','black','LineStyle','--');
end
title('AccZ');
xlabel('t (s)')
ylabel("m/s^2")
hold off

%% EWMA

figure
subplot(2,1,1)
hold on
plot(time(start:stop,1)*unit,AccY(start:stop,1),'-b');
hold off
title('AccY');
xlabel('t (s)')
ylabel("m/s^2")
subplot(2,1,2)
hold on
plot(time(start:stop,1)*unit,ewma_ay(start:stop,1),'-r');
hold off
title('Ewma-ay')
xlabel('t (s)')
ylabel("m/s^2")

figure
subplot(2,1,1)
hold on
plot(time(start:stop,1)*unit,AccZ(start:stop,1),'-b');
hold off
title('AccZ');
xlabel('t (s)')
ylabel("m/s^2")
subplot(2,1,2)
hold on
plot(time(start:stop,1)*unit,ewma_az(start:stop,1),'-r');
hold off
title('Ewma-az')
xlabel('t (s)')
ylabel("m/s^2")


