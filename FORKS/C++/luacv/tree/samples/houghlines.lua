#!/usr/bin/env lua

cv=require('luacv')

src=cv.LoadImage(arg[1] and arg[1] or "lena.png",0)
if (not src) then error("Can't load source image "..arg[1]) end

storage=cv.CreateMemStorage(0)
dst=cv.CreateImage(cv.GetSize(src),cv.IPL_DEPTH_8U,1)
color_dst=cv.CreateImage(cv.GetSize(src),cv.IPL_DEPTH_8U,3)

cv.NamedWindow("hough")
thresh=70
cv.Canny(src,dst,thresh,thresh*3,3)
cv.CvtColor(dst,color_dst,cv.CV_GRAY2BGR)
local lines=cv.HoughLines2(dst,storage,cv.CV_HOUGH_PROBABILISTIC,1,cv.CV_PI/180,50,50,10)

for i=0,lines.total do
  pts=cv.GetSeqElem(lines,i,cv.CvPoint["name"].."[2]")
  cv.Line(color_dst,pts[1],pts[2],cv.CV_RGB(255,80,0),1,cv.CV_AA,0)
end

cv.ShowImage("hough",color_dst)
cv.WaitKey()
cv.DestroyAllWindows()

