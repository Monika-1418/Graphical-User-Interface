% For ATM Understanding.
clear all;
close all;

addpath('Common/');
global lo po foc
iz = 1; qf = 1; qn = 1;
lo = 478.9161;
po =  496.0227;                         

foc =  20.91762;
F = foc/0.018;
f = foc/1000;
run = 2;
Det_limit1 = 930;
Det_limit2 = 70;
 
 
 Data = load('distcorr_cent.txt');
 
%%%%%%%%%%% RS Set %%%%%%%%%%%
%  tar_vec_err2 = [-508.0         -57.7            0   4  1
%                  -156.5        425.42            0   5  1
%                   135.45         48.2         -100  21  1
%                  -246.69        814.37           0   6  1];
 %tar_vec_err2(:,1:3) = tar_vec_err2(:,1:3)/1000;   
% 
% tar_vec_err2 =   [ -0.07718       -0.32481        -0.01220      4         1
%                     0.40602        0.02676        -0.01220      5         2
%                     0.02875        0.31875        -0.11220     21         2
%                     0.79497       -0.06344        -0.01220      6         1 ];

%%%%%%%%%%% PDS Set %%%%%%%%%%%
%old
% tar_vec_err2 =   [ -0.05960 -0.08074  -0.01220   1  1
%                     0.05720  0.11416  -0.01220   2  2  
%                     0.06945 -0.05434  -0.11220  11  1
%                    -0.08124  0.09617  -0.01220   3  2];
 
%rsl
% tar_vec_err2 =   [ -0.05980 -0.09115  -0.01220   1  1
%                     0.05734  0.10429  -0.01420   2  2  
%                     0.06964 -0.06528  -0.11221  11  1                
%                    -0.08120  0.08509  -0.01427   3  2];

%stf
tar_vec_err2 =   [ -0.05619 -0.09363  -0.01220   1  1
                    0.05388  0.10513  -0.01220   2  2  
                    0.07187 -0.06282  -0.11220  11  1                
                   -0.08386  0.08242  -0.01220   3  2]; 
                   
tar_vec_err2 =   [ -0.05980 -0.09115  -0.01220   1  1
                   0.05734  0.10295  -0.01420   2  2  
                 0.06964 -0.06528  -0.11221  11  1                
                 -0.08120  0.08509  -0.01427   3  2];                   
 
 tar_vec_err2(:,2) = -tar_vec_err2(:,2);
 
  
i1 = 0 ;
i2 = 0 ;
i3 = 0 ;
i4 = 0 ;
 
 
 
ix = 1;
diodearr = [1 2 11 3];% [4 5 21 6];%[1 2 11 3];
ktr = 0;
 
for i = 1:size(Data,1)
 
if(mod(i,4) == 2)
Vorig(ktr+1,1) = Data(ktr*4+2,1);  Uorig(ktr+1,1) = Data(ktr*4+2,2); %LD1
elseif(mod(i,4) == 3)
Vorig(ktr+1,2) = Data(ktr*4+3,1);  Uorig(ktr+1,2) = Data(ktr*4+3,2); %LD2
elseif(mod(i,4) == 0)
Vorig(ktr+1,3) = Data(ktr*4+4,1);  Uorig(ktr+1,3) = Data(ktr*4+4,2); %LD11
ktr = ktr + 1;
elseif(mod(i,4) == 1)
Vorig(ktr+1,4) = Data(ktr*4+1,1);  Uorig(ktr+1,4) = Data(ktr*4+1,2); %LD3

end
end
% V(1,1) = 452.32647705; U(1,1) = 545.09082031; %LD4
% V(1,2) = 434.44488525; U(1,2) = 569.97143555; %LD5
% V(1,3) = 419.37750244; U(1,3) = 550.83538818; %LD21
% V(1,4) = 439.16485596; U(1,4) = 589.82855225; %LD6


figure;
plot(Uorig(:,1),Vorig(:,1),'r*')
title('Centroids From Static Test Facility');
hold on
plot(Uorig(:,2),Vorig(:,2),'b*')
hold on
plot(Uorig(:,3),Vorig(:,3),'g*')
hold on
plot(Uorig(:,4),Vorig(:,4),'m*')
hold on 
 
ylabel('L coordinate(pixels)');
xlabel('P coordinate(pixels)');
grid on

    
    % Line, Pixel coordinates ( they form the centroids) 
 
for samp = 1: size(Vorig,1)  

    clear V U
    V = Vorig(samp,:);
    U = Uorig(samp,:);
     numdiodes = 4;

     tar_vec_err(1,:) = tar_vec_err2(1,1:3);
     tar_vec_err(2,:) = tar_vec_err2(2,1:3);
     tar_vec_err(3,:) = tar_vec_err2(3,1:3);
     tar_vec_err(4,:) = tar_vec_err2(4,1:3);
     
     diodeelm = zeros(1,8);
   
    for delk = 1:numdiodes
        if ((V(delk) >=Det_limit1 || V(delk) <= Det_limit2) || (U(delk) >=Det_limit1 || U(delk) <= Det_limit2)) 
       
            numdiodes = numdiodes - 1;
      
            diodeelm(1,delk) = 1;
            
            V(delk) = 0;
            U(delk) = 0;
            
        end 
    end
        
   
 
    k = 0;
    clear drq;
    for i = 1:numdiodes
        if diodeelm(1,i) == 0
            k = k + 1;
            drq(k) = i;
            if run==1
                if drq(k) == 3 
                    dcix = k;
                end
            else
                if drq(k) == 3 
                    dcix = k;
                end
            end
            
        end
    end
    
       
   
    clear vecd;
    clear Lang;
    clear aeucl;
    clear F;
    clear J;
    for tark = 1:numdiodes
        vecd(tark,:) = ccd_sen_SS(V(tark),U(tark));
    end
    
    tark = 0;
     
   % drq1 = drq(1:4);
 
    tarkrs = 0;tarkpds = 0;
    for tarki = 1:length(drq)-1 
        for tarkj = tarki+1:length(drq)
        % STEP-2: Calculate the cosine of the angle between each pair of unit vectors
        tark = tark + 1;
        Lang(tark)=vecd(drq(tarki),:)*vecd(drq(tarkj),:)';
    
        %STEP-3 : Calculate Euclidiean distance   
        aeucl(tark)= sqrt((tar_vec_err(drq(tarki),1) -tar_vec_err(drq(tarkj),1))^2 + (tar_vec_err(drq(tarki),2) -tar_vec_err(drq(tarkj),2))^2  + (tar_vec_err(drq(tarki),3) -tar_vec_err(drq(tarkj),3))^2);
        
        VUDiff(tark) = sqrt((V(tarki)-V(tarkj))^2 + (U(tarki) - U(tarkj))^2);
        RR(tark) = (aeucl(tark)*1000)*(f/(VUDiff(tark)*0.018));
        end
    end
    
    % STEP-4: Derive ranges of individal Chaser centroids to individal
    % diodes.  Jacobian matrix for Newton Raphson (Secant attempting Later)
    % Starting of Newton - Raphson iterations
    
    tol = 1E-06;
    error1 = 1E+08;
    iter = 0;
    itermax = 35;%10%25%50;   
   
     qpos1 = mean(RR);%min(RR); min(RR) is correct, just to deviate a bit intentionally, mean is taken
     %X_S = qpos1*[1;1; -1;1;1;1;-1;1];
     X_S = qpos1*[1;1;-1;1];
     X_S = X_S(1:length(drq));
    
    
     tic
    while (error1 > tol)
          
          iter=iter+1;

          Rs = X_S;
           
          tark = 0; tjac = 0;
        
          
          for tarki = 1:length(drq)-1 
              
             for tarkj = tarki+1:length(drq)
                 
                 if ((diodeelm(tarki) ~=1 && diodeelm(tarkj) ~= 1))
                     
                     tark = tark + 1;   
             
                     % Objective Function
              
                     F(tark,1) =  (Rs(drq(tarki))^2+Rs(drq(tarkj))^2-2*Rs(drq(tarki))*Rs(drq(tarkj))*Lang(tark))-aeucl(tark)^2;
              
                     % Calculate Jacobian Matrix
              
                     tjac = tjac + 1;
              
                     J(tjac,1:length(drq)) = zeros(1,length(drq));
                     
                     J(tjac,drq(tarki)) = 2*Rs(drq(tarki))-2*Rs(drq(tarkj))*Lang(tjac);
              
                     J(tjac,drq(tarkj)) = 2*Rs(drq(tarkj))-2*Rs(drq(tarki))*Lang(tjac);
                     

                     
                 end
             end
          end
           
  
          y = -mat_inv2_new((J'*J))*J'*F;
          X_S = X_S + y; 
       
         % calculate norms 
          sumy = sum(y.^2);
          error1=sqrt(sumy);
   
         ii(iter)=iter;
         if (iter > itermax)
            error1 = 0.0;
            %s=sprintf('****Did not converge within %3.0f iterations.****',itermax);
            %disp(s)
         end
    end
    ttttt = toc;
    dcix = 3;
    if run == 1
    X_Srs(qf,1:4) = X_S(1:4);
    else
    X_Spds(qn,1:4) = X_S(1:4);
    end
    
    prevX_S = X_S;
 
    clear R_c;
    clear R_t;
    chin = 0;
    for tark = 1:length(drq)
        if dcix ~= tark 
            chin = chin + 1;
            R_c(chin,:) = X_S(dcix) * vecd(dcix,:)' - X_S(drq(tark)) * vecd(drq(tark),:)'; 
            R_c(chin,:) = R_c(chin,:)/norm(R_c(chin,:));
            R_t(chin,:) = tar_vec_err(dcix,1:3) - tar_vec_err(drq(tark),1:3);
            R_t(chin,:) = R_t(chin,:)/norm(R_t(chin,:));
        end
    end
  
    
            
    % STEP-7: Relative attitude determination
    nvec = (length(drq) - 1);
    %relative quaternion (T to C)
    TCatt = ESOQ2(R_t,R_c,nvec); 
    
    if run == 1
        TCattf = TCatt;  
        MRelAtt = quat_dc(TCattf);
	    dcm1 = quat_dc(TCattf); 
        [ang(1,1),ang(1,2),ang(1,3)] = ang_123_orig(dcm1');

 
    else
        %For PDS it is X.
        TCattf = TCatt;
        MRelAtt = quat_dc(TCattf);
        dcm1 = quat_dc(TCattf); 
        [ang(1,1),ang(1,2),ang(1,3)] = ang_123_orig(dcm1');
           
    end
    
        
    % STEP-8; Relative Position determination
     sumpos = [0 0 0];
     clear mesp;
     for chin = 1: length(drq)  
         mesp(chin,:) = ((vecd(drq(chin),:)*prevX_S(chin))' - MRelAtt *tar_vec_err(drq(chin),1:3)');
         sumpos(1,:) = sumpos(1,:) + mesp(chin,:);
     end
      poso = (sumpos)/ length(drq);
      
      
     % ATM
     
%      for chin = 1: 4%length(drq)  
%          cc(chin,:) = MRelAtt *tar_vec_err2(chin,1:3)' + poso';
%          [Vnn(chin), Unn(chin)] = sen_to_ccd_SS(cc(chin,:));
%      end
%      diffL(samp,1:4) = Vnn -V;
%      diffP(samp,1:4) = Unn -U;
     
end
format short g
poso.*1000
ang
 
 
