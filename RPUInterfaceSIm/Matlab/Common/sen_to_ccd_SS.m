% function to convert sensor vector to CCD co-ordinates

function [lin, pix] = sen_to_ccd_SS(senvec)

global l0 p0 foc

%p0 = 507.6154;
%l0 = 536.856;
%foc = 48.1283; %2831.07645
%p0 = 511.69;
%l0 = 529.14;
%foc = 51.27;
pix_mm = 0.018;
%  l0 = 547.2172;
%  p0 = 513.2512;
%  foc = 2824.282353;
foc_pix = foc / pix_mm;

pix =  p0 - ((senvec(2) / senvec(3))) * foc_pix ;
lin =  l0 - ((senvec(1) / senvec(3))) * foc_pix ;

%end