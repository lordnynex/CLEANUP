#!/usr/bin/env lua

local cv=require('luacv')

local image=cv.LoadImage(arg[1] and arg[1] or "lena.png",1)
if (not image) then error("Can't load image "..arg[1]) end

local cascade=cv.Load("haarcascade_frontalface_alt.xml",cv.CvHaarClassifierCascade["name"])
local storage=cv.CreateMemStorage()
local faces=cv.HaarDetectObjects(image,cascade,storage,1.1,3,0,cv.Size(40,40))

for i=0,faces.total do
 local r=cv.GetSeqElem(faces,i,cv.CvRect["name"])
  cv.Rectangle(image,cv.Point(r.x,r.y),cv.Point(r.x+r.width,r.y+r.height),cv.CV_RGB(255,0,0),1,8.0)
end
cv.ReleaseHaarClassifierCascade(cascade)
cv.NamedWindow("face",1)
cv.ShowImage("face",image)
cv.WaitKey()
cv.DestroyAllWindows()
