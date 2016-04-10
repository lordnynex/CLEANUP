#!/usr/bin/env lua
cv=require('luacv')

ch={true,true,true}

nframes = 0
nframesToLearnBG = 30

model = cv.CreateBGCodeBookModel()

model.modMin = {3,3,3}
model.modMax = {10,10,10}
model.cbBounds ={10,10,10}

pause = false
singlestep = false

capture = cv.CreateFileCapture( arg[1] and arg[1] or 'tree.avi' ) or error("Can not initialize video capturing")
rawImage = cv.QueryFrame( capture )

yuvImage = cv.CloneImage(rawImage)
ImaskCodeBook = cv.CreateImage( cv.GetSize(rawImage), cv.IPL_DEPTH_8U, 1 )
ImaskCodeBookCC = cv.CreateImage( cv.GetSize(rawImage), cv.IPL_DEPTH_8U, 1 )
cv.Set(ImaskCodeBook,cv.Scalar(255))

cv.NamedWindow( "Raw", 1 )
cv.NamedWindow( "ForegroundCodeBook",1)
cv.NamedWindow( "CodeBook_ConnectComp",1)


while true do
  rawImage = cv.QueryFrame( capture )
  nframes=nframes+1
  if not rawImage then break end

  if rawImage then 
    cv.CvtColor( rawImage, yuvImage, cv.CV_BGR2YCrCb )

    if nframes-1 < nframesToLearnBG then
      cv.BGCodeBookUpdate( model, yuvImage )
    end

    if nframes-1 == nframesToLearnBG  then
      cv.BGCodeBookClearStale( model, model.t/2 )
    end

    if nframes-1 >= nframesToLearnBG  then
      cv.BGCodeBookDiff( model, yuvImage, ImaskCodeBook )
      cv.Copy(ImaskCodeBook,ImaskCodeBookCC)	
      cv.SegmentFGMask( ImaskCodeBookCC )
    end

    cv.ShowImage( "Raw", rawImage )
    cv.ShowImage( "ForegroundCodeBook",ImaskCodeBook)
    cv.ShowImage( "CodeBook_ConnectComp",ImaskCodeBookCC)
  end
  c=cv.WaitKey(10)
  if c == 27 then 
    break 
  elseif c==string.byte(' ') then
    cv.BGCodeBookClearStale(model,0)
    nframes = 0
  end
end 

cv.ReleaseCapture(capture )
cv.DestroyAllWindows()
