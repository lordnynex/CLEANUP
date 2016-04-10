#!/usr/bin/env lua
local cv=require('luacv')

local img = cv.CreateImage( cv.Size( 500, 500 ),cv.IPL_DEPTH_8U, 3 );
local storage = cv.CreateMemStorage();

cv.NamedWindow( "hull", 1 );

local RNG=cv.RNG(-1);
while true do
  local count =cv.RandInt(RNG) % 100
  local ptseq = cv.CreateSeq(cv.CV_32SC2["value"],cv.CvContour["size"],cv.CvPoint["size"],storage)
  for i=0,count do
    cv.SeqPush(ptseq,cv.Point(cv.RandInt(RNG)%(img.width/2)+img.width/4,cv.RandInt(RNG)%(img.height/2)+img.height/4));
  end

  hull=cv.ConvexHull2(ptseq,nil,cv.CV_CLOCKWISE,0);

  cv.Zero(img);
  local pt0
  for i=0,count do
    pt0 = cv.GetSeqElem(ptseq,i,cv.CvPoint["name"])
    cv.Circle( img, pt0, 2, cv.CV_RGB(255,0,0),cv.CV_FILLED,cv.CV_AA,0)
  end

  pt0=cv.GetSeqElem(hull,hull.total-1,cv.CvPoint["name"]..'[]')
  for i=0,hull.total do
    pt=cv.GetSeqElem(hull,i,cv.CvPoint["name"]..'[]')
    cv.Line(img,pt0,pt,cv.CV_RGB(0,255,0),1,cv.CV_AA,0)
    pt0=pt
  end

  cv.ShowImage( "hull", img );
  if (cv.WaitKey()==27) then break end
end
cv.DestroyAllWindows()
