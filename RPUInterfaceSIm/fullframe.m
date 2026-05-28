clear all;
close all;
 

 %filename = input('Enter Filename: ','s'); 
 [file path] = uigetfile('.pgm','Select Image','/home/cssd/SPADEX/RPUInterfaceSimulator_Focussing/images/');
 sat = input('Whether image is saturated? '); 
 
 fout = fopen('../win_cordinates.txt','w');
 filename = strcat(path, file);
 disp(filename);
 if ( sat == 1)
   disttol = 100;%25;
   thresh  =60;%30; %250
   minhit = 3;%3
 else
 %If more clusters are coming in image then increase thresh 
 % else if nothing in clusters is observed then decrease thresh
   disttol = 25;
   thresh  = 30;
   minhit = 2;
 end

 A = imread(filename);
 B = zeros(size(A));
 for i = 1: 1024
   B(1025-i,:) = A(i,:);
 end
B=A;
  
 BinIndex = 0;
 for i = 1: 1024 
  for j = 1: 1024
   
      if (B(i,j) > thresh)
         BinIndex = BinIndex + 1;
         BinPixelsBuffer(BinIndex,1) = i;
         BinPixelsBuffer(BinIndex,2) = j;
         BinPixelsBuffer(BinIndex,3) = B(i,j);
      end
 
  end
 end
 
  
  K = 1;

  StarClusterBuffer(1,1) = BinPixelsBuffer(1,1);
  StarClusterBuffer(1,2) = BinPixelsBuffer(1,2);
  StarClusterBuffer(1,3) = 1;
  StarClusterBuffer(1,4) = abs(BinPixelsBuffer(1,3));
  StarClusterBuffer(1,5) = 0;
  StarClusterBuffer(1,6) = 0;
  StarClusterBuffer(1,7) = 255;
 

  for I = 2 : BinIndex  
   

      BinLineFL4  = BinPixelsBuffer(I,1);
      BinPixelFL4 = BinPixelsBuffer(I,2);
      BinGradient = abs(BinPixelsBuffer(I,3));
  if( I == 44)

      I = 44;
      I = 44;
   end

      Match = 0;

      for J = 1 : K
         
          ClustLineFl4 = StarClusterBuffer(J,1);
          ClustPixFl4  = StarClusterBuffer(J,2);

          Term1   = ClustLineFl4 - BinLineFL4;
          Term2   = ClustPixFl4  - BinPixelFL4;
          DistSq1 = Term1 * Term1;
          DistSq2 = Term2 * Term2;

          if ((DistSq1 <= disttol) && (DistSq2 <= disttol)) 

              M     = StarClusterBuffer(J,3);
              M     = M + 1;
              Term1 = M;


              ClustGrad = StarClusterBuffer(J,4);
              
              StarClusterBuffer(J,4) = ClustGrad + BinGradient;

              StarClusterBuffer(J,1) = ClustLineFl4 + (BinLineFL4 - ClustLineFl4) / Term1;

              StarClusterBuffer(J,2) = ClustPixFl4 + (BinPixelFL4 - ClustPixFl4) / Term1;

              StarClusterBuffer(J,3) = M;

              Match = 1;

              if (StarClusterBuffer(J,5) == 0) && (DistSq1 == 1.0) 
                  StarClusterBuffer(J,5) = 1;
              end 

             if (BinGradient > StarClusterBuffer(J,6)) 
                 StarClusterBuffer(J,6) = BinGradient;                 
             end               
             
             if (BinGradient < StarClusterBuffer(J,7)) 
                 StarClusterBuffer(J,7) = BinGradient;                 
             end   
 
              break;

          end

      end

     
      if ((Match == 0) && (K < 1000)) 
        K = K + 1;
        StarClusterBuffer(K,1) = BinLineFL4;
        StarClusterBuffer(K,2) = BinPixelFL4;
        StarClusterBuffer(K,3) = 1;
        StarClusterBuffer(K,4) = BinGradient;
        StarClusterBuffer(K,5) = 0;
        StarClusterBuffer(K,6) = 0;
        StarClusterBuffer(K,7) = 255;
      end 

  end 

  NumOfTotStarClusters = K;
  NumOfStarClusters = 0;

for I = 1: K
    if (StarClusterBuffer(I,3) >= minhit)

        NumOfStarClusters = NumOfStarClusters + 1;
        LDCluster(NumOfStarClusters ,:) = StarClusterBuffer(I,:);
      
    end
end 


if(NumOfStarClusters > 0)
figure;
%plot(LDCluster(:,2),1025-LDCluster(:,1),'r.'); grid on;
plot(LDCluster(:,2),1025-LDCluster(:,1),'r.'); grid on;
axis([1 1024 1 1024]);
disp('Displaying LDCLuster:');
disp(NumOfStarClusters);
 
 
for i = 1:NumOfStarClusters
  printf("%d  %d\n" ,LDCluster(i,1),LDCluster(i,2));
  
  fprintf(fout, "%d %d\n", LDCluster(i,1),LDCluster(i,2));
end
 
 
 fclose(fout);
disp(LDCluster);
end
if(NumOfTotStarClusters >0)
disp('Displaying StarClusterBuffer:');
disp(NumOfTotStarClusters);
disp('    Line       Pixel       Hits      SumI      LineFlag      MaxCPI      MinCPI');
disp(StarClusterBuffer);
end

