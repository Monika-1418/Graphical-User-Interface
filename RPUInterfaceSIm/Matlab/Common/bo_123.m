% pcs-486-with creative cd-rom drive pc
% f:\matlab\models directory of JSR.
% file name bo_123.m
% body-to-orbit matrix function
% dob 19-02-1999-FRI-09-00

function bo = bo_123(yaw, roll, pitch)
% inputs are in degrees

% usual yaw-rol-pitch or 1-2-3 rotations 

DTR = pi / 180.0;
RTD = 180.0 / pi;

pr = pitch * DTR ;  % about z-axis
rr = roll  * DTR ;  % about y-axis
yr = yaw   * DTR ;  % about x-axis 

pit_rot = [ cos(pr)  sin(pr)  0
           -sin(pr)  cos(pr)  0
           0          0      1];
        
rol_rot = [cos(rr)   0    -sin(rr)
              0      1      0
           sin(rr)   0   cos(rr)];
        
yaw_rot = [1    0         0
           0  cos(yr)   sin(yr)
           0  -sin(yr)  cos(yr)];
        
orbbod = pit_rot*rol_rot*yaw_rot;
        
bo = orbbod';

return;

        
           
           
