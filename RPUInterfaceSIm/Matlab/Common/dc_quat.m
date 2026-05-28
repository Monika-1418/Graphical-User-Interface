% pcs-486-with creative cd-rom drive pc
% f:\matlab\models directory of JSR.
% file name dc_quat.m
% quternion related function
% dob 25-01-1999-MON-16-30

function quat_op = dc_quat(dcm)
% echo off
% procedure to convert dcm to quaterninon
% i/p's d11, d12, d13
%       d21, d22, d23
%       d31, d32, d33  passed as global now
%
% o/p's qs1, qs2, qs3, qs4
%

d11 = dcm(1,1);
d12 = dcm(1,2);
d13 = dcm(1,3);

d21 = dcm(2,1);
d22 = dcm(2,2);
d23 = dcm(2,3);

d31 = dcm(3,1);
d32 = dcm(3,2);
d33 = dcm(3,3);
d44 = d11+d22+d33;

term1 = 1.0 + 2.0 * d11 - d44 ;
term2 = 1.0 + 2.0 * d22 - d44 ;
term3 = 1.0 + 2.0 * d33 - d44 ;
term4 = 1.0+d44 ;



p0  = sqrt(1.0+d44) ;
p1  = sqrt(1.0 + 2.0 * d11 - d44) ;
p2  = sqrt(1.0 + 2.0 * d22 - d44) ;
p3  = sqrt(1.0 + 2.0 * d33 - d44) ;

% finding the maximum of p0 through p3

p_vec = [p0 p1 p2 p3];
p_max = max(p_vec);

if(p_max == p0) 
   p1 = (d23 - d32) / p_max;
   p2 = (d31 - d13) / p_max;
   p3 = (d12 - d21) / p_max;
end
if(p_max == p1) 
   p0 = (d23 - d32) / p_max;
   p2 = (d21 + d12) / p_max;
   p3 = (d13 + d31) / p_max;
end
if(p_max == p2) 
   p0 = (d31 - d13) / p_max;
   p1 = (d21 + d12) / p_max;
   p3 = (d32 + d23) / p_max;
end
if(p_max == p3) 
   p0 = (d12 - d21) / p_max;
   p1 = (d13 + d31) / p_max;
   p2 = (d32 + d23) / p_max;
end


if (p0 < 0.0)
   qs1 = -0.5 * p1;
   qs2 = -0.5 * p2;
   qs3 = -0.5 * p3;
   qs4 = -0.5 * p0;
else
   qs1 = 0.5 * p1; 
   qs2 = 0.5 * p2;
   qs3 = 0.5 * p3;
   qs4 = 0.5 * p0;
end

quat_op = [qs1 qs2 qs3 qs4];

% end-of-the function.

return




