#!/usr/bin/env lua
cv=require('luacv')

win_size = 10
MAX_COUNT = 500
status = {}
need_to_init = 0
flags = 0
pt=cv.Point(0,0)
count=0

capture = cv.CaptureFromFile( arg[1] and arg[1] or 'tree.avi' )

if not capture then error("can't open video") end

cv.NamedWindow( "LkDemo", 0 )

points0={}
points1={}

for i=0,MAX_COUNT do 
  points1[i]=cv.Point2D32f(0,0)
  points0[i]=cv.Point2D32f(0,0)
end


flags = 0
frame=cv.QueryFrame(capture)
image = cv.CreateImage( cv.GetSize(frame), cv.IPL_DEPTH_8U, 3 )
image.origin = frame.origin
grey = cv.CreateImage( cv.GetSize(frame), cv.IPL_DEPTH_8U, 1 )
prev_grey = cv.CreateImage( cv.GetSize(frame), cv.IPL_DEPTH_8U, 1 )
pyramid = cv.CreateImage( cv.GetSize(frame), cv.IPL_DEPTH_8U, 1 )
prev_pyramid = cv.CreateImage( cv.GetSize(frame), cv.IPL_DEPTH_8U, 1 )
need_to_init=1
while true do

  frame = cv.QueryFrame( capture )
  if not frame then break end

  cv.Copy( frame, image, nil )
  cv.CvtColor( image, grey, cv.CV_BGR2GRAY )


  if need_to_init==1 then
    eig = cv.CreateImage( cv.GetSize(grey), cv.IPL_DEPTH_32F, 1 )
    temp = cv.CreateImage( cv.GetSize(grey), cv.IPL_DEPTH_32F, 1 )
    quality = 0.01
    min_distance = 10

    count = MAX_COUNT
    count=cv.GoodFeaturesToTrack( grey, eig, temp, points1,count, quality, min_distance, nil, 3, 0, 0.04 )

    cv.FindCornerSubPix( grey, points1, cv.Size(win_size,win_size), cv.Size(-1,-1),cv.TermCriteria(bit32.bor(cv.CV_TERMCRIT_ITER,cv.CV_TERMCRIT_EPS),20,0.03))
    cv.ReleaseImage(eig )
    cv.ReleaseImage(temp )

  elseif count > 0 then

    status=cv.CalcOpticalFlowPyrLK( prev_grey, grey, prev_pyramid, pyramid,points0, points1, count, cv.Size(win_size,win_size), 3, cv.TermCriteria(bit32.bor(cv.CV_TERMCRIT_ITER,cv.CV_TERMCRIT_EPS),20,0.03), flags )

    flags = bit32.bor(flags,cv.CV_LKFLOW_PYR_A_READY)
    k=0
    for i=k,count-1 do
      points1[k] = points1[i]
      k=k+1
      cv.Circle( image, cv.PointFrom32f(points1[i]), 3, cv.CV_RGB(0,255,0), -1, 8,0)
    end 
    count = k
  end

  prev_grey,grey=grey,prev_grey
  prev_pyramid,pyramid=pyramid,prev_pyramid
  points0,points1=points1,points0

  need_to_init = 0
  cv.ShowImage( "LkDemo", image )

  c = cv.WaitKey(50)
  if c == 27 then
    break
  elseif c== 32 then --space
    need_to_init = 1
  end
end 

cv.ReleaseCapture( capture )
cv.DestroyWindow("LkDemo")
