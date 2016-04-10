#!/usr/bin/env lua
cv=require('luacv')

MHI_DURATION = 1
MAX_TIME_DELTA = 0.5
MIN_TIME_DELTA = 0.05
N = 4
last=0
buf={}

function update_mhi( img, dst, diff_threshold )
  timestamp = os.clock()
  idx1=last
  size = cv.GetSize(img)
  if( not mhi or mhi.width ~= size.width or mhi.height ~= size.height ) then
    for  j=0,N-1 do
      if buf[j] then
        cv.ReleaseImage(buf[j]) 
      end
      buf[j] = cv.CreateImage( size, cv.IPL_DEPTH_8U, 1 )
      cv.Zero(buf[j])
    end

    mhi = cv.CreateImage( size, cv.IPL_DEPTH_32F, 1 )
    cv.Zero( mhi )
    orient = cv.CreateImage( size, cv.IPL_DEPTH_32F, 1 )
    segmask = cv.CreateImage( size, cv.IPL_DEPTH_32F, 1 )
    mask = cv.CreateImage( size, cv.IPL_DEPTH_8U, 1 )
  end

  cv.CvtColor( img, buf[last], cv.CV_BGR2GRAY )

  idx2 = (last + 1) % N
  last = idx2

  silh = buf[idx2]
  cv.AbsDiff( buf[idx1], buf[idx2], silh )

  cv.Threshold( silh, silh, diff_threshold, 1, cv.CV_THRESH_BINARY )
  cv.UpdateMotionHistory( silh, mhi, timestamp, MHI_DURATION )

  cv.CvtScale( mhi, mask, 255/MHI_DURATION, (MHI_DURATION - timestamp)*255/MHI_DURATION )
  cv.Zero( dst )
  cv.Merge( mask, nil, nil, nil, dst )

  cv.CalcMotionGradient( mhi, mask, orient, MAX_TIME_DELTA, MIN_TIME_DELTA, 3 )

  cv.ClearMemStorage(storage)

  seq = cv.SegmentMotion( mhi, segmask, storage, timestamp, MAX_TIME_DELTA )
  
  for  i=-1,seq.total-1 do 
    if i<0 then
      comp_rect = cv.Rect( 0, 0, size.width, size.height )
      color = cv.CV_RGB(255,255,255)
      magnitude = 100
    else
      comp=cv.GetSeqElem(seq, i ,cv.CvConnectedComp["name"])
      comp_rect = comp.rect
      color = cv.CV_RGB(255,0,0)
      magnitude = 30
    end
    if  (comp_rect.width + comp_rect.height) >= 100 then

    cv.SetImageROI( silh, comp_rect )
    cv.SetImageROI( mhi, comp_rect )
    cv.SetImageROI( orient, comp_rect )
    cv.SetImageROI( mask, comp_rect )

    angle = 360 - cv.CalcGlobalOrientation( orient, mask, mhi, timestamp, MHI_DURATION)
    count = cv.Norm( silh, nil, cv.CV_L1, nil )

    cv.ResetImageROI( mhi )
    cv.ResetImageROI( orient )
    cv.ResetImageROI( mask )
    cv.ResetImageROI( silh )

    if count >= (comp_rect.width*comp_rect.height * 0.05) then
      center = cv.Point( (comp_rect.x + comp_rect.width/2), (comp_rect.y + comp_rect.height/2) )
      cv.Circle( dst, center, cv.Round(magnitude*1.2), color, 3, cv.CV_AA, 0 )
      cv.Line( dst, center, cv.Point( cv.Round( center.x + magnitude*math.cos(angle*cv.CV_PI/180)),cv.Round( center.y - magnitude*math.sin(angle*cv.CV_PI/180))), color, 3, cv.CV_AA, 0 )

      end
    end
  end 
end


capture = cv.CaptureFromFile(arg[1] and arg[1] or 'tree.avi') or error("can't load file")


cv.NamedWindow( "Motion", 1 )
storage = cv.CreateMemStorage(0)
while true do
  image = cv.QueryFrame( capture )
  if not image then break end

  if not motion then
    motion = cv.CreateImage( cv.GetSize(image), cv.IPL_DEPTH_8U, 3 )
    cv.Zero(motion)
    motion.origin = image.origin
  end

  update_mhi( image, motion, 30 )
  cv.ShowImage( "Motion", motion )

  if cv.WaitKey(10) >= 0 then
    break
  end
end

cv.ReleaseCapture( capture )
cv.DestroyWindow( "Motion" )
