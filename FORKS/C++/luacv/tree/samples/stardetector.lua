#!/usr/bin/env lua
cv=require('luacv')

capture = cv.CaptureFromFile( arg[1] and arg[1] or 'tree.avi' )

if not capture then error("can't open video") end

cv.NamedWindow( "stardetect", 0 )

frame=cv.QueryFrame(capture)
image = cv.CreateImage( cv.GetSize(frame), cv.IPL_DEPTH_8U, 3 )
image.origin = frame.origin
storage = cv.CreateMemStorage()
while true do
  frame = cv.QueryFrame( capture )
  if not frame then break end
  cv.Copy( frame, image)
  seq=cv.GetStarKeypoints(image,storage)
  for i=0,seq.total-1 do
    elem=cv.GetSeqElem(seq,i,cv.CvStarKeypoint["name"])
    cv.Circle(image,elem.pt,3,cv.Scalar(255,0,0),1)
  end 
  cv.ShowImage("stardetect",image)
  cv.ClearSeq(seq)
if  cv.WaitKey(150)> 0 then break end
end 

cv.ReleaseCapture( capture )
cv.ReleaseMemStorage(storage)
cv.DestroyWindow("stardetect")
