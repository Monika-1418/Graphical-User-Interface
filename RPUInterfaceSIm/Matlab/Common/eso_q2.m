% ESOQ-2 - Estimators of Optimal Quaternion-2
% Fastest Quaternion Estimation Algorithm
% Work started on 9-11-2004 by M.Ganesan, CSSD

%date - 7-3-2006
% derived from ESOQ2_v3.m

function ESOQ2 = ESOQ2_sp(refmat,senmat,n)

global rot rotationvec;

% ref -- Reference Star Vector (3x1)
% meas -- Measured Star Vector (3x1)
% n -- No of Stars
%flops(0)
rotation_enable = 1;

format long;

cos_phi_min = 1e-06; %representing q4 component

sum = zeros(3,3);
I = eye(3,3);


%for i = 1:n
 %  temp1 = senmat(i,1:3)';
  % temp2 = refmat(i,1:3)';
   %temp3 = temp1 * temp2';
   %sum = sum + temp3;
%end
%B = sum/n;

B = refmat' * senmat/n;

tmp = B';
B = tmp;

S = B;

Sigma = trace(B);

rot = 0;

if (Sigma >= cos_phi_min)
 Sigma = S(1,1) - S(2,2) - S(3,3);
 
 if (Sigma < cos_phi_min)
     
    % S(1,2) = -S(1,2); S(2,2) = -S(2,2); S(3,2) = -S(3,2);
     %S(1,3) = -S(1,3); S(2,3) = -S(2,3); S(3,3) = -S(3,3);
     
     S(:,2) = -S(:,2); S(:,3) = -S(:,3);
     rot = 1;
 else
     Sigma = S(2,2) - S(1,1) - S(3,3);
          if (Sigma < cos_phi_min)
         S(1,1) = -S(1,1); S(2,1) = -S(2,1); S(3,1) = -S(3,1);
         S(1,3) = -S(1,3); S(2,3) = -S(2,3); S(3,3) = -S(3,3);
         rot = 2;
     else
         Sigma = S(3,3)-S(1,1) - S(2,2);
         if (Sigma < cos_phi_min)
             S(1,1) = -S(1,1); S(2,1) = -S(2,1); S(3,1) = -S(3,1);
             S(1,2) = -S(1,2); S(2,2) = -S(2,2); S(3,2) = -S(3,2);
             rot = 3;
         end
         
     end
     
 end

end

SigmaSq = Sigma * Sigma;

Z(1) = S(2,3) - S(3,2);
Z(2) = S(3,1) - S(1,3);
Z(3) = S(1,2) - S(2,1);

S(1,1) = S(1,1) + S(1,1);
S(1,2) = S(2,1) + S(1,2);
S(2,1) = S(1,2);

S(1,3) = S(3,1) + S(1,3);
S(3,1) = S(1,3);

S(2,2) = S(2,2) + S(2,2);

S(2,3) = S(2,3) + S(3,2);
S(3,2) = S(2,3);

S(3,3) = S(3,3) + S(3,3);

SZ = S * Z';
SSZ = S * SZ;

% Compute cofactor

C00 = S(2,2) * S(3,3) - S(2,3) * S(3,2);
C11 = S(1,1) * S(3,3) - S(3,1) * S(1,3);
C22 = S(1,1) * S(2,2) - S(1,2) * S(2,1);
C01 = S(2,3) * S(3,1) - S(2,1) * S(3,3);
C02 = S(2,1) * S(3,2) - S(2,2) * S(3,1);

KTrace = C00 + C11 + C22;

DetS = S(1,1) * C00 + S(1,2) * C01 + S(1,3) * C02;

A = SigmaSq - KTrace;
Bm = SigmaSq + dot(Z,Z);
C = DetS + dot(Z, SZ);
D = dot(Z, SSZ);

opt = 2;

if (opt ==1)

R = (A * Bm + C * Sigma) - D;
P = A + Bm;
P = -P;
C = -C;

% Solve Eqn

L = 1;
Bl = 1 + 2 * P + C;
Al = L + P + C + R;

for n = 0 : 5
    Cl = A/Bl;
    L = L - Cl;
    C = abs(Cl);
    
    if (Cl < 0)
        break;
    end
    
    if (i~=5)
     L2 = L * L;
     L4 = L2 * L2;
     A = L4+ P *L2 + C * L  + R;
     L4 = L2 + L2;
     Bl = L4 + P;
     Bl = 2 * Bl + L + C;
    end   
    
end

lamdamax = L;
else
    m =1;
    eps = 1e-06;
    
  for n=0:5,
   
f_m =m^4-(A+Bm)*m^2-C*m+(A*Bm+C*Sigma-D);
f_der = 4*m^3-2*(A+Bm)*m-C;
mn = m- f_m/f_der; 
dif_m = (mn-m);
if (dif_m < 0)
   dif_m = -dif_m;
end

lamdamax = mn;

if lamdamax > 1
   lamdamax =1;
end


if (dif_m < eps)
   break
end
end      
lamdamax = mn;   
    
end


 

if lamdamax > 1
   lamdamax =1;
end

P = Sigma - lamdamax;
C = Sigma + lamdamax;

S(1,1) = S(1,1) - C;
S(2,2) = S(2,2) - C;
S(3,3) = S(3,3) - C;

ZZT(1,1) = Z(1) * Z(1);
ZZT(1,2) = Z(1) * Z(2);
ZZT(1,3) = Z(1) * Z(3);

ZZT(2,1) = ZZT(1,2);
ZZT(2,2) = Z(2) * Z(2);
ZZT(2,3) = Z(2) * Z(3);

ZZT(3,1) = ZZT(1,3);
ZZT(3,2) = ZZT(2,3);
ZZT(3,3) = Z(3) * Z(3);

for jj = 1 : 3
    for kk = 1 : 3
        S(jj,kk) = S(jj,kk) * P - ZZT(jj,kk);
    end
end

p1 = abs(S(2,2) * S(3,3) - S(3,2) * S(3,2));
p2 = abs(S(1,1) * S(3,3) - S(3,1) * S(3,1));
p3 = abs(S(1,1) * S(2,2) - S(2,1) * S(2,1));


p = [p1;p2;p3];

maxp = max(p);

if maxp == p1
  % p1
   ek(1) = S(2,2) * S(3,3) - S(3,2) * S(3,2);
   ek(2) = S(2,3) * S(3,1) - S(2,1) * S(3,3);
   ek(3) = S(2,1) * S(3,2) - S(2,2) * S(3,1);
elseif maxp == p2
 %  p2   
 ek(1) = S(3,2) * S(1,3) - S(3,3) * S(1,2);
 ek(2) = S(3,3) * S(1,1) - S(3,1) * S(3,1);
 ek(3) = S(3,1) * S(1,2) - S(3,2) * S(1,1);
 
else
%   p3
  ek(1) = S(1,2) * S(2,3) - S(1,3) * S(2,2);
  ek(2) = S(1,3) * S(2,1) - S(1,1) * S(2,3);
  ek(3) = S(1,1) * S(2,2) - S(2,1) * S(2,1);
end

e = ek;

quat(1:3) = (lamdamax - Sigma) * e;
quat(4) = dot(Z,e); %Z'*e;
qopt = quat / sqrt(quat*quat');

ESOQ2 = qopt;

if rot == 1
   ESOQ2_tmp = [ESOQ2(4) -ESOQ2(3) ESOQ2(2) -ESOQ2(1)];
elseif rot == 2
   ESOQ2_tmp = [ESOQ2(3) ESOQ2(4) -ESOQ2(1) -ESOQ2(2)];
elseif rot == 3
   ESOQ2_tmp = [-ESOQ2(2) ESOQ2(1) ESOQ2(4) -ESOQ2(3)];
else
   ESOQ2_tmp = ESOQ2;
end
 
%ESOQ2 = ESOQ2_tmp;

if (ESOQ2_tmp(4) < 0)
   for i = 1 : 4
      qt(i) = -ESOQ2_tmp(i);
   end
else
   for i = 1 : 4
      qt(i) = ESOQ2_tmp(i);
   end
end


ESOQ2 = qt';

%flops;

return


 