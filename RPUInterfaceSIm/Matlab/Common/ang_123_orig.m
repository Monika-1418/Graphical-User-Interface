% ang_123.m  - DCM to Euler angles for 1-2-3 rotation
% this function gives the euler angles corresponding to the rotation 1-2-3
% input: dcm matrix correponding to 1-2-3 rotation
% dob: 03-10-2002
%

function [angx, angy, angz] = ang_123_orig(dcm)

RTD = 180 / pi;

%angx = arctan(-dcm(3,2), dcm(3,3));
angx = taninv(-dcm(3,2),dcm(3,3));
angy = asin(dcm(3,1)) * RTD;
%angz = arctan(-dcm(2,1),dcm(1,1));
angz = taninv(-dcm(2,1),dcm(1,1));

return
