#!/usr/bin/env lua
cv=require('luacv')

img=cv.CreateImage(cv.Size(500,500),cv.IPL_DEPTH_8U,3)
wname="rect"

cv.NamedWindow(wname)
rng=cv.RNG(-1)
storage=cv.CreateMemStorage()
center=cv.Point2D32f(0,0)
while(true) do
  count=cv.RandInt(rng)%100 +1
  seq=cv.CreateSeq(cv.CV_32SC2["value"],cv.CvContour["size"],cv.CvPoint["size"],storage)
  for i=0,count-1 do
     cv.SeqInsert(seq,0,cv.Point(cv.RandInt(rng)%(img.width/2) +img.width/4,
                                 cv.RandInt(rng)%(img.height/2) + img.height/4))
  end
  box=cv.MinAreaRect2(seq)
  ret,radius=cv.MinEnclosingCircle(seq,center)
  box_vtx={}
  cv.BoxPoints(box,box_vtx)
  cv.Zero(img)
  for i=0,count-1 do
    pt0=cv.GetSeqElem(seq,i,cv.CvPoint["name"])
    cv.Circle(img,pt0,2,cv.CV_RGB(255,0,0),cv.CV_FILLED,cv.CV_AA,0)
  end
  pt0.x=cv.Round(box_vtx[4].x)
  pt0.y=cv.Round(box_vtx[4].y)
  for i=1,4 do
    pt=cv.Point(cv.Round(box_vtx[i].x),cv.Round(box_vtx[i].y))  
    cv.Line(img,pt0,pt,cv.CV_RGB(0,255,0),1,cv.CV_AA,0)
    pt0=pt;
  end
  cv.Circle(img,cv.Point(cv.Round(center.x),cv.Round(center.y)),cv.Round(radius),cv.CV_RGB(255,255,0),1,cv.CV_AA,0)
  cv.ShowImage(wname,img)
  if(cv.WaitKey()==27) then break end
  cv.ClearMemStorage(storage)
end
cv.DestroyWindow(wname)
