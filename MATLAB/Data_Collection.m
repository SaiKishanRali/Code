clear data;
close all;
clear all;

%% File name

filename = ' '; % input file name

%% Zoom

window = 0; % 0 - full file, 1 - specific time window

begin = 320;   % time window size (seconds)
finish = 330;

%% Data read

path =  'file location';

path = strcat(path,filename,'.txt');

fid = fopen(path);
data = textscan(fid, '%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f','MultipleDelimsAsOne',true,'Delimiter',',','HeaderLines',0);
fclose(fid);

time = data{1,1};             % time stamp
AccX = data{1,2};             % Accelerometer X-axis 
AccY = data{1,3};             % Accelerometer Y-axis  
AccZ = data{1,4};             % Accelerometer Z-axis 
GyrX = data{1,5};             % Gyroscope X-axis  
GyrY = data{1,6};             % Gyroscope Y-axis  
GyrZ = data{1,7};             % Gyroscope Z-axis
Pressure_left = data{1,9};    % Left foot pressure sensor value
Pressure_right = data{1,8};   % Right foot pressure sensor value
ewma_ax = data{1,10};         % EWMA output for Accelerometer X-axis
ewma_ay = data{1,11};         % EWMA output for Accelerometer Y-axis 
ewma_az = data{1,12};         % EWMA output for Accelerometer Z-axis  
ewma_gx = data{1,13};         % EWMA output for Gyroscope X-axis
ewma_gy = data{1,14};         % EWMA output for Gyroscope Y-axis
ewma_gz = data{1,15};         % EWMA output for Gyroscope Z-axis 
row = length(Pressure_left);

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

%% Plots

unit = 10^(-6);

if(output == 4)

    figure
    subplot(3,1,1)
    hold on
    plot(time(start:stop,1)*unit,AccX(start:stop,1),'-m');
    title('AccX');
    legend('X axis')
    xlabel('t (s)')
    ylabel("m/s^2")
    set(gca,'FontSize',15)
    hold off
    subplot(3,1,2)
    hold on
    plot(time(start:stop,1)*unit,AccY(start:stop,1),'-b');
    title('AccY');
    legend('Y axis')
    xlabel('t (s)')
    ylabel("m/s^2")
    set(gca,'FontSize',15)
    hold off
    subplot(3,1,3)
    hold on
    plot(time(start:stop,1)*unit,AccZ(start:stop,1),'-r');
    title('AccZ');
    legend('Z axis')
    xlabel('t (s)')
    ylabel("m/s^2")
    set(gca,'FontSize',15)
    hold off
    
    figure
    
    subplot(3,1,1)
    hold on
    plot(time(start:stop,1)*unit,GyrX(start:stop,1),'-m');
    title('GyrX');
    legend('X axis')
    xlabel('t (s)')
    ylabel("°/s")
    set(gca,'FontSize',15)
    hold off
    subplot(3,1,2)
    hold on
    plot(time(start:stop,1)*unit,GyrY(start:stop,1),'-b');
    title('GyrY');
    legend('Y axis')
    xlabel('t (s)')
    ylabel("°/s")
    set(gca,'FontSize',15)
    hold off
    subplot(3,1,3)
    hold on
    plot(time(start:stop,1)*unit,GyrZ(start:stop,1),'-r');
    title('GyrZ');
    legend('Z axis')
    xlabel('t (s)')
    ylabel("°/s")
    set(gca,'FontSize',15)
    hold off
    
    figure
    subplot(4,1,1)
    hold on
    plot(time(start:stop,1)*unit,GyrX(start:stop,1),'-m');
    title('GyrX');
    legend('X axis')
    xlabel('t (s)')
    ylabel("°/s")
    set(gca,'FontSize',15)
    hold off
    subplot(4,1,2)
    hold on
    plot(time(start:stop,1)*unit,GyrY(start:stop,1),'-b');
    title('GyrY');
    legend('Y axis')
    xlabel('t (s)')
    ylabel("°/s")
    set(gca,'FontSize',15)
    hold off
    subplot(4,1,3)
    hold on
    plot(time(start:stop,1)*unit,GyrZ(start:stop,1),'-r');
    title('GyrZ');
    legend('Z axis')
    xlabel('t (s)')
    ylabel("°/s")
    set(gca,'FontSize',15)
    hold off
    subplot(4,1,4)
    hold on
    plot(time(start:stop,1)*unit,Pressure_left(start:stop,1),'-g');
    plot(time(start:stop,1)*unit,Pressure_right(start:stop,1),'-k');
    xlabel('t (s)')
    title('Pressure Sensor');
    legend('Left','Right');
    set(gca,'FontSize',15)
    hold off   
    

end

%% Plots- EWMA

if (output == 3)
    
    figure
    subplot(2,1,1)
    hold on
    plot(time(start:stop,1)*unit,AccX(start:stop,1),'-b');
    hold off
    title('AccX');
    subplot(2,1,2)
    hold on
    plot(time(start:stop,1)*unit,ewma_ax(start:stop,1),'-r');
    hold off
    title('Ewma-ax')
    
    figure
    subplot(2,1,1)
    hold on
    plot(time(start:stop,1)*unit,AccY(start:stop,1),'-b');
    hold off
    title('AccY');
    subplot(2,1,2)
    hold on
    plot(time(start:stop,1)*unit,ewma_ay(start:stop,1),'-r');
    hold off
    title('Ewma-ay')
    
    figure
    subplot(2,1,1)
    hold on
    plot(time(start:stop,1)*unit,AccZ(start:stop,1),'-b');
    hold off
    title('AccZ');
    subplot(2,1,2)
    hold on
    plot(time(start:stop,1)*unit,ewma_az(start:stop,1),'-r');
    hold off
    title('Ewma-az')
    
    figure
    subplot(2,1,1)
    hold on
    plot(time(start:stop,1)*unit,GyrX(start:stop,1),'-b');
    hold off
    title('GyrX');
    subplot(2,1,2)
    hold on
    plot(time(start:stop,1)*unit,ewma_gx(start:stop,1),'-r');
    hold off
    title('Ewma-gx')
    
    figure
    subplot(2,1,1)
    hold on
    plot(time(start:stop,1)*unit,GyrY(start:stop,1),'-b');
    hold off
    title('GyrY');
    subplot(2,1,2)
    hold on
    plot(time(start:stop,1)*unit,ewma_gy(start:stop,1),'-r');
    hold off
    title('Ewma-gy')
    
    figure
    subplot(2,1,1)
    hold on
    plot(time(start:stop,1)*unit,GyrZ(start:stop,1),'-b');
    hold off
    title('GyrZ');
    subplot(2,1,2)
    hold on
    plot(time(start:stop,1)*unit,ewma_gz(start:stop,1),'-r');
    hold off
    title('Ewma-gz')
    
end