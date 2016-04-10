#!/usr/bin/env lua
local cv=require('luacv')

img=cv.LoadImage(arg[1] and arg[1] or 'squares.png' ,cv.CV_LOAD_IMAGE_GRAYSCALE) or error("couldn't load image")

cv.NamedWindow("image",1)
cv.ShowImage("image",img)

cv.NamedWindow("connected",1)
storage=cv.CreateMemStorage()
rng=cv.RNG(-1)
function update(pos)
  bw=cv.CloneImage(img)
  cv.Threshold(img,bw,pos,pos,cv.CV_THRESH_BINARY)  
  _,contours=cv.FindContours(bw,storage,cv.CvContour["size"],cv.CV_RETR_CCOMP,cv.CV_CHAIN_APPROX_SIMPLE)

  dst=cv.CreateImage(cv.GetSize(bw),cv.IPL_DEPTH_8U,3)
  cv.Zero(dst)
  if contours then
    while contours do
      color=cv.CV_RGB(cv.RandInt(rng)%255,cv.RandInt(rng)%255,cv.RandInt(rng)%255)
      cv.DrawContours(dst,contours,color,cv.CV_RGB(cv.RandInt(rng)%255,cv.RandInt(rng)%255,cv.RandInt(rng)%255),contours.total,3,cv.CV_AA,cv.Point(0,0))
      contours=contours.h_next
    end
  end
  cv.ShowImage("connected",dst)
end
cv.CreateTrackbar("threshold","connected",255,update)
update(128)
cv.WaitKey()
cv.DestroyAllWindows()
