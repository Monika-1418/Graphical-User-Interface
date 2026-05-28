
%d:\padma\mlab\qtrmul
%qtrmul.m multiplies two quaternions
%input:2 sets of quaternions
%output:product quaternion
%author:S.Padmasree
%date:10-06-1999

function quatr = qtrmul(q , p)
%q(i)'s & p(i)'s are input quaternions
%qtr(i) is product quaternion
%quatr(1) = quatr(1) * 2147483647;


%q = [-207178654 1383574475 -1611262340 241491950] / 2147483647;
%p = [978754348 1160983521 1513023300 128855313] / 2147483647;
 
quatr(1) =  q(1) * p(4) + q(2) * p(3) - q(3) * p(2) + q(4) * p(1);
quatr(2) = -q(1) * p(3) + q(2) * p(4) + q(3) * p(1) + q(4) * p(2);
quatr(3) =  q(1) * p(2) - q(2) * p(1) + q(3) * p(4) + q(4) * p(3);
quatr(4) = -q(1) * p(1) - q(2) * p(2) - q(3) * p(3) + q(4) * p(4);

 %quat = validateq(quatr); 

return;