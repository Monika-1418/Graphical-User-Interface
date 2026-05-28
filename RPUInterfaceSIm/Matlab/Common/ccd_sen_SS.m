
function Dcvec = ccd_sen_SS(li,pi)
     format long;

global lo po foc

%l0 = input("Null Line: ");
%p0 = input("Null pixel: ");
%foc = input("Focal length in mm: ");

pix_mm = 0.018;
%  l0 = 547.2172;
%  p0 = 513.2512;
%  foc = 2824.282353;
foc_pix = foc / pix_mm;

LineDiff = (li  - lo) / foc_pix; 
PixelDiff = (pi - po) / foc_pix; 

   LineDiffSq  = LineDiff  * LineDiff;
   PixelDiffSq = PixelDiff * PixelDiff;

   Mag      = 1.0 + LineDiffSq + PixelDiffSq;
   OneByMag = 1.0 / Mag^0.5;

   Dcvec(1) = LineDiff * OneByMag;
   Dcvec(2) = PixelDiff * OneByMag;
   Dcvec(3) = -OneByMag;
   
   return
