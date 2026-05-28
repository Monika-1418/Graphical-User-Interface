% ang_213ex.m  - DCM to Euler angles for 2-1-3 rotation
% this function gives the euler angles corresponding to the rotation 2-1-3
% input: dcm matrix correponding to 2-1-3 rotation
%  dou: 16-6-2005;
% refer to ang_231_variable.m
%orbbod =
 
%[               cpr*crr,      spr,              -cpr*srr]
%[ srr*syr - crr*cyr*spr,  cpr*cyr, crr*syr + cyr*spr*srr]
%[ cyr*srr + crr*spr*syr, -cpr*syr, crr*cyr - spr*srr*syr];%
%this also correct
function [angy, angz, angx] = ang_231(dcm)

RTD = 180 / pi;

angz = asin(dcm(1,2)) * RTD; %yaw-rot
 angx = taninv(-dcm(3,2),dcm(2,2)); %rol-rot
 angy = taninv(-dcm(1,3),dcm(1,1)); %pit-rot
%angy = acos(dcm(1,1)/cos(angz*pi/180)) * RTD;
%angx = acos(dcm(2,2)/cos(angz*pi/180)) * RTD;


return
