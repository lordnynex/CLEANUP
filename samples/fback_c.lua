#!/usr/bin/env lua
cv=require('luacv')

function drawOptFlowMap(flow,cflowmap,step,scale,color)
  for  y =0,cflowmap.rows-1,step do
    for  x = 0,cflowmap.cols-1,step do
      local scal=cv.Get2D(flow,y,x)
      fxy=cv.Point2D32f(scal.val[1],scal.val[2])
      cv.Line(cflowmap, cv.Point(x,y), cv.Point(cv.Round(x+fxy.x),cv.Round(y+fxy.y)),color, 1, 8, 0)
      cv.Circle(cflowmap, cv.Point(x,y), 2, color, -1, 8, 0)
    end
  end
end

  capture = cv.CaptureFromFile(arg[1] and arg[1] or 'tree.avi') or error("can't load video") 

  cv.NamedWindow("flow", 1)

  frame=cv.QueryFrame(capture)

  gray = cv.CreateMat(frame.height, frame.width, cv.CV_8UC1)
  prevgray = cv.CreateMat(gray.rows, gray.cols, cv.CV_8UC1)
  flow = cv.CreateMat(gray.rows, gray.cols, cv.CV_32FC2)
  cflow = cv.CreateMat(gray.rows, gray.cols, cv.CV_8UC3)
  while frame  do
    frame=cv.QueryFrame(capture)
    if not frame then break end
    cv.CvtColor(frame, gray, cv.CV_BGR2GRAY)

    cv.CalcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0)
    cv.CvtColor(prevgray, cflow, cv.CV_GRAY2BGR)
    drawOptFlowMap(flow, cflow, 16, 1.5,cv.CV_RGB(0, 255, 0))
    cv.ShowImage("flow", cflow)

    if cv.WaitKey(30)>=0 then break end

    prevgray,gray=gray,prevgray
  end

  cv.ReleaseMat(gray)
  cv.ReleaseMat(prevgray)
  cv.ReleaseMat(flow)
  cv.ReleaseMat(cflow)
  cv.ReleaseCapture(capture)
  cv.DestroyAllWindows()
