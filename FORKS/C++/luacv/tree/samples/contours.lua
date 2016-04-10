#!/usr/bin/env lua

local cv=require("luacv")

function on_trackbar(pos)
  local cnt_img=cv.CreateImage(cv.Size(500,500),cv.IPL_DEPTH_8U,3)
  local _contours=contours
  cv.Zero(cnt_img)
  cv.DrawContours(cnt_img,_contours,cv.CV_RGB(255,0,0),cv.CV_RGB(0,255,0),pos,3,cv.CV_AA,cv.Point(0,0))
  cv.ShowImage(wname[2],cnt_img)
end


storage=cv.CreateMemStorage()
img=cv.CreateImage(cv.Size(500,500),cv.IPL_DEPTH_8U,1)
cv.Zero(img)

for i=0,5 do
  dx=(i%2)*250 -30
  dy=(i/2)*150
  white=cv.RealScalar(255)
  black=cv.RealScalar(0)

 if i==0 then
    for j=0,10 do
      angle=(j+5)*cv.CV_PI/21
      cv.Line(img,cv.Point(cv.Round(dx+100+j*10-80*math.cos(angle)),
                           cv.Round(dy+100-90*math.sin(angle))),
                  cv.Point(cv.Round(dx+100+j*10-30*math.cos(angle)),
                           cv.Round(dy+100-30*math.sin(angle))),white,1,8,0)
    end
  end

  cv.Ellipse(img,cv.Point(dx+150,dy+100),cv.Size(100,70),0,0,360,white,-1,8,0);
  cv.Ellipse(img,cv.Point(dx+115,dy+70),cv.Size(30,20),0,0,360,black,-1,8,0);
  cv.Ellipse(img,cv.Point(dx+185,dy+70),cv.Size(30,20),0,0,360,black,-1,8,0);
  cv.Ellipse(img,cv.Point(dx+115,dy+70),cv.Size(15,15),0,0,360,white,-1,8,0);
  cv.Ellipse(img,cv.Point(dx+185,dy+70),cv.Size(15,15),0,0,360,white,-1,8,0);
  cv.Ellipse(img,cv.Point(dx+115,dy+70),cv.Size(5,5),0,0,360,black,-1,8,0);
  cv.Ellipse(img,cv.Point(dx+185,dy+70),cv.Size(5,5),0,0,360,black,-1,8,0);
  cv.Ellipse(img,cv.Point(dx+150,dy+100),cv.Size(10,5),0,0,360,black,-1,8,0);
  cv.Ellipse(img,cv.Point(dx+150,dy+150),cv.Size(40,10),0,0,360,black,-1,8,0);
  cv.Ellipse(img,cv.Point(dx+27,dy+100),cv.Size(20,35),0,0,360,white,-1,8,0);
  cv.Ellipse(img,cv.Point(dx+273,dy+100),cv.Size(20,35),0,0,360,white,-1,8,0);
end
wname={"image","contours"}
cv.NamedWindow(wname[1],1)
cv.ShowImage(wname[1],img)
a,contours=cv.FindContours(img,storage,cv.CvContour["size"],cv.CV_RETR_TREE,cv.CV_CHAIN_APPROX_SIMPLE,cv.Point(0,0))
contours=cv.ApproxPoly(contours,cv.CvContour["size"],storage,cv.CV_POLY_APPROX_DP,3,1)

cv.NamedWindow(wname[2],1)

cv.CreateTrackbar("levels",wname[2],4,on_trackbar)
on_trackbar(0)
cv.WaitKey()
cv.DestroyAllWindows()
