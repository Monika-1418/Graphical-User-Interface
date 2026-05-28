% taninv.m - tan inverse function with correct mapping of 
%					the angle to 0 to 360.0 deg

function [ang] = taninv(y,x)

PI = 3.141592653589793;
RTD = 180.0 / PI;

ang = atan(y/x) * RTD;

if((x < 0.0) && (y >= 0.0)) 
   ang = ang + 180.0;
end
if((x < 0.0) && (y < 0.0))
   ang = ang + 180.0;
end

if((x>0.0) && (y < 0.0))
   ang = ang + 360.0;
end




