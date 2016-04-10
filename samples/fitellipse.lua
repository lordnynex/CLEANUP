#!/usr/bin/env lua
local cv=require('luacv')

image03=cv.LoadImage(not arg[1] and'squares.png' or arg[1],cv.CV_LOAD_IMAGE_GRAYSCALE) or error("can't load image")

storage=cv.CreateMemStorage(0)
image02=cv.CloneImage(image03)
image04=cv.CreateImage(cv.GetSize(image03),cv.IPL_DEPTH_8U,3)

cv.NamedWindow("Result",1)

cv.CreateTrackbar("Threshold","Result",255, function (slider_pos)
  local contour=cv.CreateSeq(cv.CV_SEQ_ELTYPE_POINT,cv.CvSeq["size"],cv.CvPoint["size"],storage)

  cv.Threshold(image03,image02,slider_pos,255,cv.CV_THRESH_BINARY)

  local _,contour=cv.FindContours(image02,storage,cv.CvContour["size"],cv.CV_RETR_LIST,cv.CV_CHAIN_APPROX_NONE,cv.Point(0,0))

  cv.CvtColor(image03,image04,cv.CV_GRAY2BGR)
  while contour do
    local count=contour.total
    if count>4 then
      local points=cv.CreateMat(1,count,cv.CV_32FC2)
      local elems={}
      cv.CvtSeqToArray(contour,elems,cv.Slice(0,count),'CvPoint')

      for i,j in pairs(elems) do 
        cv.Set1D(points,i-1,cv.Scalar(j.x,j.y)) 
      end
      local box=cv.FitEllipse2(points)
      cv.DrawContours(image04,contour,cv.CV_RGB(255,0,0),cv.CV_RGB(0,255,255),0,2,8,cv.Point(0,0))

      local center=cv.PointFrom32f(box.center)
      local size=cv.Size(cv.Round(box.size.width*0.5),cv.Round(box.size.height*0.5))
      if size.width>10 and size.height>10 then
        cv.Ellipse(image04,center,size,box.angle,0,360,cv.CV_RGB(128,128,0),1,cv.CV_AA,0)
        cv.Circle(image04,center,3,cv.CV_RGB(0,255,0),-1)
      end
      cv.ReleaseMat(points)
    end
    collectgarbage('collect')
    contour=contour.h_next
  end
  cv.ShowImage("Result",image04)
end)

cv.Callbacks[1](0)
cv.WaitKey(0)
cv.ReleaseImage(image02)
cv.ReleaseImage(image03)
cv.ReleaseImage(image04)
cv.ReleaseMemStorage(storage)
cv.DestroyWindow("Result")
