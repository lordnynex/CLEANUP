#!/usr/bin/env lua
cv=require('luacv')

cap = cv.CaptureFromFile(arg[1] and arg[1] or 'tree.avi') or error("can not open camera or video file")

tmp_frame = cv.QueryFrame(cap)

if not tmp_frame then
  error("can not read data from the video source")
end

cv.NamedWindow("BG", 1)
cv.NamedWindow("FG", 1)

bg_model = cv.CreateFGDStatModel( tmp_frame)

while true do
  tmp_frame = cv.QueryFrame(cap)
  if not tmp_frame then break end

  t = cv.GetTickCount()
  cv.UpdateBGStatModel( tmp_frame, bg_model,-1)
  t = cv.GetTickCount() - t
  print( "time", t/(cv.GetTickFrequency()*1000.) )
  cv.ShowImage("BG", bg_model.background)
  cv.ShowImage("FG", bg_model.foreground)
  k = cv.WaitKey(5)
  if k == 27 then  break end
end

cv.ReleaseBGStatModel(bg_model )
cv.ReleaseCapture(cap)
