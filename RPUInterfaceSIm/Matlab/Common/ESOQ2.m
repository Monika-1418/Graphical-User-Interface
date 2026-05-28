% ESOQ-2 - Estimators of Optimal Quaternion-2
% Fastest Quaternion Estimation Algorithm
% Work started on 9-11-2004 by M.Ganesan, CSSD

%date - 7-3-2006
% derived from ESOQ2_v3.m

function ESOQ2 = ESOQ2(refmat,senmat,n)

global rot rotationvec;

% ref -- Reference Star Vector (3x1)
% meas -- Measured Star Vector (3x1)
% n -- No of Stars
%flops(0)
rotation_enable = 1;

%SP,AA Added 02-12-2020, now performance same as ESOQ2_sp.m
rot = 0;
%%%%%

format long;

cos_phi_min = 1e-06; %representing q4 component

sum = zeros(3,3);
I = eye(3,3);


for i = 1:n
   temp1 = senmat(i,1:3)';
   temp2 = refmat(i,1:3)';
   temp3 = temp1 * temp2';
   sum = sum + temp3;
end
B = sum/n;

S = B + B';

z =[B(2,3)-B(3,2) B(3,1)-B(1,3) B(1,2)-B(2,1)]';

del = det(S);
%del = 1.4e-4;
sig = trace (S)*0.5;
%sig = trace (S);

adj_S = inv(S)* del;
K = trace(adj_S);

A = sig^2-K;
Bm = sig^2 + z'*z;
C = del + z'*S*z;
D = z'*S^2*z;

%to solve m^4-(A+Bm)*m^2-C*m+(A*Bm+C*sig-D)=0;
m =1;
eps = 1e-6;
for n=0:5,
   
f_m =m^4-(A+Bm)*m^2-C*m+(A*Bm+C*sig-D);
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

if rotation_enable == 1
   
   tr_B = trace(B);
   
   tr_B1 = B(1,1) - B(2,2) - B(3,3);
   tr_B2 = B(2,2) - B(1,1) - B(3,3);
   tr_B3 = B(3,3) - B(2,2) - B(1,1);  
   
   %pause

if (tr_B >= cos_phi_min)
   
   tr_B1 = B(1,1) - B(2,2) - B(3,3);
   tr_B2 = B(2,2) - B(1,1) - B(3,3);
   tr_B3 = B(3,3) - B(2,2) - B(1,1);   
   temp = 0;
   
   if (tr_B1 < cos_phi_min)
      B_tmp(:,1) =  B(:,1); % B(1,:);
      B_tmp(:,2) = -B(:,2); %-B(2,:);
      B_tmp(:,3) = -B(:,3); %-B(3,:);  
      temp = 1;
      rot =  1;

  elseif (tr_B2 < cos_phi_min)
      B_tmp(:,1) = -B(:,1); %-B(1,:);
      B_tmp(:,2) =  B(:,2);
      B_tmp(:,3) = -B(:,3);
      temp = 2;
       rot =  2;

   elseif (tr_B3 < cos_phi_min)
      B_tmp(:,1) = -B(:,1);
      B_tmp(:,2) = -B(:,2);
      B_tmp(:,3) =  B(:,3);
      temp = 3;
      rot =  3;
    else
       B_tmp = B;
       rot =0;
       temp = 0;
   end
  % rot = rot + 1;
   rotationvec= temp;
   B = B_tmp;
end  

end

S = B + B';

z =[B(2,3)-B(3,2) B(3,1)-B(1,3) B(1,2)-B(2,1)]';

%flops;

%flops(0);

%t = sig;
%S = S - I * sig; % Eq.7
%lamdamx = 1;

temp1 = trace(B)-lamdamax;
temp2 = S - (trace(B)+lamdamax)*I;
temp3 = z*z';

M = temp1*temp2 - temp3;

m1 = M(1,:)';
m2 = M(2,:)';
m3 = M(3,:)';

p1 = abs(M(2,2)*M(3,3)-M(3,2)^2);
p2 = abs(M(1,1)*M(3,3)-M(3,1)^2);
p3 = abs(M(1,1)*M(2,2)-M(2,1)^2);

p = [p1;p2;p3];

maxp = max(p)';

if maxp == p1
  % p1
   ek = cross(m2,m3);
elseif maxp == p2
 %  p2
   ek = cross(m3,m1);
else
%   p3
   ek = cross(m1,m2);
end

e = ek;

quat(1:3) = (lamdamax - trace(B)) * e;
quat(4) = z'*e;
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

ESOQ2 = validateq(ESOQ2_tmp)';

%flops;

return