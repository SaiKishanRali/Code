close all;
clear all;
clc;
%% Output file name

pick = 1;  % 1 - without prosthesis, 2 - with prosthesis
finalfile = 'healthy_rand_adjusted'; % output file name
% Output file will be saved in the same path as mentioned in the below

%% Reading the files from the folder

if pick == 1
    path = ' '; % path for without prosthesis files
    candidate = 7; % total number of candidates
end
if pick == 2
    path = ' '; % path for with prosthesis files
    candidate = 12; % total number of candidates
end

filecount = 0;

for i = 1:candidate
    for j = 1:4
        filename = 'c';
        filename = strcat(filename,string(i),string(j));
        
        if pick == 1
            path = ' ';
        end
        if pick == 2
            path = ' ';
        end
        
        path = strcat(path,filename,'.txt');
        fid = fopen(path);
        if(fid ~= -1)
            data{i,j} = textscan(fid, '%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f','MultipleDelimsAsOne',true,'Delimiter',',','HeaderLines',0);
            fclose(fid);
            filecount = filecount +1;
        else
            display(filename);
        end

    end
end

%% Randomization of the data

if pick == 1
    path = ' '; % path for without prosthesis files
%     row = [1 1 1 1 2 2 2 2 3 3 3 3 4 4 4 4 5 5 5 5 6 6 6 6 7 7 7 7];
%     col = [1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4];
    
    row = [6 5 1 2 7 5 6 2 2 1 7 1 3 3 1 4 2 6 7 5 7 4 3 5 4 4 6 3];
    col = [1 2 3 1 4 3 4 3 2 1 1 2 4 2 4 2 4 3 3 1 2 1 3 4 3 4 2 1];
    
    % adjusted data randomized
    row = [6 5 1 2 7 5 6 2 2 1 7 1 3 3 1 2 6 7 5 7 3 5 6 3];
    col = [1 2 3 1 4 3 4 3 2 1 1 2 4 2 4 4 3 3 1 2 3 4 2 1];    
    
end
if pick == 2
    path = ' '; % path for without prosthesis files
%     row = [1 1 1 1 2 2 2 2 3 3 3 3 4 4 4 4 5 5 5 5 6 6 6 6];
%     col = [1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4 1 2 3 4];

    % random combination for the first 6 candidates with prosthesis
%     row = [3 3 6 3 6 1 2 6 1 5 5 1 5 2 4 6 1 5 4 2 3 4 2 4]; 
%     col = [4 3 4 1 2 3 2 1 1 1 3 4 4 3 4 3 2 2 1 1 2 3 4 2];

    % random combination for hip prosthesis candidates
%     row = [8 11 3 1 9 8 3 3 1 9 8 3 1 9 8 1 9]; 
%     col = [1 1 3 4 3 2 2 1 2 1 4 4 1 2 3 3 4];

    % random combination for knee prosthesis candidates
%     row = [4 5 7 2 12 6 4 5 2 12 6 4 5 2 12 6 4 5 2 12 6]; 
%     col = [1 3 1 4 3 4 3 4 2 1 2 2 1 4 4 3 4 2 3 2 1];

    % random combination for all the prosthesis candidates
    row = [3 9 2 7 3 9 5 12 6 5 4 11 8 1 2 5 3 1 1 9 8 2 4 6 12 1 8 5 3 6 9 8 4 4 12 6 2 12]; 
    col = [3 3 4 1 2 1 4 1 3 2 1 1 1 2 2 1 4 1 3 4 4 4 3 4 3 4 2 3 1 2 2 3 2 4 2 1 3 4];

end

path = strcat(path,finalfile,'.txt');

fileID = fopen(path,'w');
fprintf(fileID,'%s\n\n','STEVAL-MKI197V1 (LSM6DSOX)');
fprintf(fileID,'%s\n','A_X [mg] 	 A_Y [mg] 	 A_Z [mg] 	G_X [dps] 	 G_Y [dps] 	 G_Z [dps] ');
fclose(fileID);

for p = 1:length(row)
   filenum(p) = strcat(string(row(p)),string(col(p)));
   data_c = data{row(p),col(p)};
   AccX = data_c{1,2};
   AccY = data_c{1,3};
   AccZ = data_c{1,4};
   GyrX = data_c{1,5};
   GyrY = data_c{1,6};
   GyrZ = data_c{1,7};
   A = [AccX AccY AccZ GyrX GyrY GyrZ];
   fileID = fopen(path,'a+');
   for i= 1:length(AccX)
       for j= 1:6
           if(j ~= 6)
            
               fprintf(fileID,'%12.2f',A(i,j));

           end
           if(j==6)

               fprintf(fileID,'%12.2f\n',A(i,j));

           end
       end
   end
   fclose(fileID);
end
