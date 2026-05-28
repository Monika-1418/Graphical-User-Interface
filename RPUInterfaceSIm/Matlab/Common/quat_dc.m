% pcs-486-with creative cd-rom drive pc
% f:\matlab\models directory of JSR.
% file name quat_dc.m
% quternion related function
% dob 26-01-1999-TUE-12-30

function dcm = quat_dc(quat)
% i/p quat(4)  - quaternion in linear matrix
% o/p dcm(3,3) - corresponding  3 x 3 dcm     

q1 = quat(1); q2 = quat(2); q3 = quat(3); q4 = quat(4);

dcm(1,1) = q4 * q4 + q1 * q1 - q2 * q2 - q3 * q3;
dcm(1,2) = 2.0 * (q1 * q2 + q3 * q4);
dcm(1,3) = 2.0 * (q1 * q3 - q2 * q4);

dcm(2,1) = 2.0 * (q1 * q2 - q3 * q4);
dcm(2,2) = q4 * q4 - q1 * q1 + q2 * q2 - q3 * q3;
dcm(2,3) = 2.0 * (q2 * q3 + q1 * q4);

dcm(3,1) = 2.0 * (q1 * q3 + q2 * q4);
dcm(3,2) = 2.0 * (q2 * q3 - q1 * q4);
dcm(3,3) = q4 * q4 - q1 * q1 - q2 * q2 + q3 * q3;

return;


