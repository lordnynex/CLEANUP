#!/usr/bin/env lua
cv=require('luacv')

img=cv.LoadImage(not arg[1] and "lena.png" or arg[1])
if (not arg[2]) and (not arg[1]) then
  tmpl=cv.CreateMatHeader(img.width,img.height,cv.IPL_DEPTH_8U)
  cv.GetSubRect(img,tmpl,cv.Rect(317,250,32,32))
else
  tmpl=cv.LoadImage(arg[2])
end

if (not img) or (not tmpl) then error("Cannot load src image or template image") end 

wname={"located","template"}
imgs={img,tmpl}

res=cv.CreateImage(cv.Size(img.width - tmpl.width +1,img.height - tmpl.height +1),
                   cv.IPL_DEPTH_32F,1)

cv.MatchTemplate(img,tmpl,res,cv.CV_TM_SQDIFF)
minloc=cv.Point(0,0)
maxloc=cv.Point(0,0)

min,max=cv.MinMaxLoc(res,minloc,maxloc)

cv.Rectangle(img,minloc,cv.Point(minloc.x+tmpl.width,minloc.y+tmpl.height),
             cv.CV_RGB(255,255,0),1,0,0)

for i=1,#wname do 
  cv.NamedWindow(wname[i])
  cv.ShowImage(wname[i],imgs[i])
end

cv.WaitKey()
cv.DestroyAllWindows()
