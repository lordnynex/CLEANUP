#!/usr/bin/env lua
cv=require('luacv')

board_sz=cv.Size(8,5)
corners={}

for i=0,8*5 do 
  corners[i]=cv.Point2D32f(0,0)
end

image=cv.LoadImage(arg[1] and arg[1] or "chess2.png") or error("can't load image")

gray=cv.CreateImage(cv.GetSize(image),cv.IPL_DEPTH_8U,1)

found,count=cv.FindChessboardCorners(image,board_sz,corners,bit32.bor(cv.CV_CALIB_CB_ADAPTIVE_THRESH,cv.CV_CALIB_CB_FILTER_QUADS))

cv.CvtColor(image,gray,cv.CV_BGR2GRAY)

cv.FindCornerSubPix(gray,corners,cv.Size( 11, 11 ),cv.Size(-1,-1 ),cv.TermCriteria(cv.CV_TERMCRIT_EPS+cv.CV_TERMCRIT_ITER,30,0.1))

cv.DrawChessboardCorners(image,board_sz,corners,count,found)

cv.ShowImage("calib",image)
cv.WaitKey()
cv.ReleaseImage(image)
cv.ReleaseImage(gray)
