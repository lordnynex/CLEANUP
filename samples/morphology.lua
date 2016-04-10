#!/usr/bin/env lua
local cv=require('luacv')

function OpenClose(pos)
  local n = pos - max_iters;
  local an = n > 0 and n or -n;
  element = cv.CreateStructuringElementEx( an*2+1, an*2+1, an, an, element_shape, nil );
  if (n<0) then
    cv.Erode(src,dst,element,1);
    cv.Dilate(dst,dst,element,1);
  else
    cv.Dilate(src,dst,element,1);
    cv.Erode(dst,dst,element,1);
  end
  cv.ShowImage(wndname,dst);
end

function ErodeDilate(pos)
  local n = pos - max_iters;
  local an = n > 0 and n or -n;
  element = cv.CreateStructuringElementEx( an*2+1, an*2+1, an, an, element_shape);
  if (n<0) then
    cv.Erode(src,dst,element,1);
  else
    cv.Dilate(src,dst,element,1);
  end
  cv.ShowImage(wndname,dst);
end

element_shape = cv.CV_SHAPE_RECT;
max_iters = 20;
wndname="morph"

src = cv.LoadImage(arg[1] and arg[1] or "lena.png")
if (not src) then error("Can't load source image "..arg[1]) end
dst = cv.CloneImage(src);

cv.NamedWindow(wndname,1);

fnc=OpenClose
--fnc=ErodeDilate
--
function on_change(i)
  fnc(i)
end
function on_change2(i)
  if i==1 then fnc=OpenClose 
  else fnc=ErodeDilate
  end
end
cv.CreateTrackbar("pos",wndname,max_iters,on_change)
cv.CreateTrackbar("fnc",wndname,1,on_change2)

on_change(max_iters-2*max_iters)
on_change2(0)
cv.WaitKey()
cv.DestroyAllWindows()
