 
close all;
clear all;
 

format long g;
global lo po foc DistRadius ZonesCoefficientLine ZonesCoefficientPixel  ;
addpath('Common/');
DTR = pi/180;
 

fin = fopen('Coefficients_Onboard.txt','r');
fout = fopen('distcorr_cent.txt','w');
 
%Data = [370.013 608.447; 398.484 365.724; 559.309 632.480; 569.375 366.149]; %redundant diode-1
%Data = [368.793304 612.527466; 396.905792  370.55361; 558.510559  637.117432; 568.024963 369.58895]; %redundant diode-2
Data = [369.331024 613.452209;396.993103 371.496796;558.742126 637.232056;587.034485  395.4621]; %main diode at null

Data = [368.990082 613.80926; 396.666779 371.769836;557.840515  637.180237;586.642090 395.743347];%main diode at null post centering
 
% Data = [368.214661 654.559265; 397.510376 412.48962;558.238464  678.577759;585.795471 436.116547];%main diode at az2deg post centering
% Data = [325.219452 614.479980;353.732117  371.407410; 514.991211  637.595703;543.373230  396.150116];%main diode at el2deg post centering
% Data = [321.436371 609.71527;358.242310  368.295654;511.098389  639.512390;546.825378  398.825012];%main diode at z2deg post centering
%Data = [316.05 604.10; 365.46 364.13; 503.16 643.72; 552.0 405.0];%main diode at z5deg post centering
Data = [362.706  619.412
399.588  378.118
550.714  649.5
588  408.789];

Data = load ('../cent.txt');

Data(1,1:2)
 
cam =2;
idealframe = 0;
rotangle = [0 0 0];

if(cam == 1)
  fov = 5.0;
   
else
  fov = 21.0;  
end 
 
m = size(Data,1);
CorrData = size(Data);


   InpCoefficients= load('Coefficients.txt');
 
    cnt = 0;
    n=3;
    totzones = 1;  
    nullst = n*totzones*2 + (totzones-1)*2 + 7;    
  
   % Step-1 Fetch Null and Focal length from file
    while(~feof(fin))
       cnt = cnt + 1;
       str = fgets(fin);
       if(cnt == nullst)
       
         lo = str2double(str(33:40));
         po = str2double(str(43:50));
         foc_pix = str2double(str(53:59));
       end

       if((cnt == nullst+10) || (cnt == nullst+11) || (cnt == nullst+12))                    
          rotangle(cnt-nullst-9,1) = str2double(str);
       end
    end
  po
      foc =  foc_pix*0.018;  
      rpix = (foc_pix*tand(21));
      resol = 21.0/rpix;
      PIX_PER_1RAD = 1/(resol*DTR); 

      DistRadius = [360];
      %DistRadius = [30 60 90 120 150 180 180+30 180+60 270 270+30 270+60];
    
  % Step-2 Fetch Coefficients
 for i = 1:totzones
    indx = i*n;
    ZonesCoefficientLine(i,:) = InpCoefficients(indx-(n-1):indx);
 end


 for i = totzones+1:totzones*2
    indx = i*n;
    ZonesCoefficientPixel(i-totzones,:) = InpCoefficients(indx-(n-1):indx);
 end
 
ktr = 1;
 
 for i = 1:m
    
    % Step-3 Rotation correction
    ZForLine  = rotangle(3)*(Data(i,2) - po);
    ZForPixel = rotangle(3)*(Data(i,1) - lo);
    YForLine  = rotangle(2)*PIX_PER_1RAD;
    XForPixel = rotangle(1)*PIX_PER_1RAD;
    
    Data(i,1) = Data(i,1) - YForLine - ZForLine;
    Data(i,2) = Data(i,2) + XForPixel + ZForPixel;
 
     Lcd =  Data(i,1) - lo;
     Pcd =  Data(i,2) - po;
     PcdSq  = Pcd * Pcd;
     LcdSq  = Lcd * Lcd;
     SqSum  = LcdSq + PcdSq ;
     raddist = (SqSum^0.5);
     quadang = taninv(Data(i,1)-lo,Data(i,2)-po);
     rad(i,1) = raddist*resol;
     LcdRot =  -Lcd * sin(rotangle(3)) + Pcd * cos(rotangle(3));
     PcdRot =  (Lcd * cos(rotangle(3)) + Pcd * sin(rotangle(3)));
     Term1  = Lcd*Pcd;
     Term2  = LcdRot*PcdRot;
     Term3 = SqSum + (foc_pix*foc_pix);
 
     %[thL, thP, zonenum] =  rFindQuadForDistortion(quadang, idealframe);    
      zone = 1;
      thL  = ZonesCoefficientLine(zone,:)';
      thP  = ZonesCoefficientPixel(zone,:)';

      if (idealframe == 1)
        thL = -thL;
        thP = -thP;
      end
  
     % Step-4 Distortion correction
     thL = [1; thL];
     thP = [1; thP];

     X(1,1:n+1) = [Lcd; raddist; Lcd*Pcd;  LcdRot*PcdRot]; 
     delL =  X*thL;
     X(1,1:n+1) = [Pcd; raddist; Lcd*Pcd;  LcdRot*PcdRot]; 
     delP =  X*thP;
     
     CorrData(i,1) = lo + delL; 
     CorrData(i,2) = po + delP;
     fprintf(fout , '%f %f \n',CorrData(i,1),CorrData(i,2)); 
     ktr =ktr + 1;
 end 
  
 
 fclose all;
Data
CorrData
 
