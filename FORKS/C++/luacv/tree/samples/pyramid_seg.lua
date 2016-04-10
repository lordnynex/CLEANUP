#!/usr/bin/env lua

local cv=require("luacv")

local image=cv.LoadImage(arg[1] and arg[1] or "fruits.png",1)
if (not image) then error("Can't load source image "..arg[1]) end
local out_im=cv.CloneImage(image)

local storage=cv.CreateMemStorage(1000)
wname="segmentation"
cv.NamedWindow(wname)

function trackbar(pos)
  cv.PyrSegmentation(cv.CloneImage(image),out_im,storage,cv.CreateSeq(0,cv.CvSeq["size"],cv.CvPoint["size"],storage),4,255,pos)
  cv.ShowImage(wname,out_im)
end
range=100
cv.CreateTrackbar("track",wname,range,trackbar)
trackbar(range/2)
cv.WaitKey()

cv.DestroyWindow(wname)
