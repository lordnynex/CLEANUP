#!/usr/bin/env lua
cv=require("luacv")
local filename=arg[1] and arg[1] or "lena.png"
image=cv.LoadImage(filename,1)
if (not image) then error("Can't load image ") end
im_size=cv.GetSize(image)
cedge=cv.CreateImage(im_size,cv.IPL_DEPTH_8U,3)
edge=cv.CreateImage(im_size,cv.IPL_DEPTH_8U,1)
gray=cv.CreateImage(im_size,cv.IPL_DEPTH_8U,1)
cv.CvtColor(image,gray,cv.CV_BGR2GRAY)
cv.Smooth(gray,edge,cv.CV_BLUR,3,3,0,0)
cv.Not(gray,edge)

cv.NamedWindow("edge",0)

for thresh=300,0,-5 do
  cv.Canny(gray,edge,thresh,thresh*2,3)
  cv.Zero(cedge)
  cv.Copy(image,cedge,edge)
  cv.ShowImage("edge",cedge)
  if cv.WaitKey(2)==27 then break end
end
cv.DestroyAllWindows()
