%to validate a quaternion

function qt = validateq(quat)

if (quat(4) < 0)
   for i = 1 : 4
      qt(i) = -quat(i);
   end
else
   for i = 1 : 4
      qt(i) = quat(i);
   end
end

return;