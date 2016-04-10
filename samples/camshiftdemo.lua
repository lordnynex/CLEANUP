#!/usr/bin/env lua
cv=require('luacv')

backproject_mode = 0
select_object = 0
track_object = 0
show_hist = 1

origin=cv.Point(0,0)
track_window=cv.Rect(0,0,1,1)
track_box=cv.Box2D(cv.Point2D32f(0,0),cv.Size2D32f(0,0),0)
track_comp=cv.ConnectedComp(0,cv.Scalar(0),cv.Rect(0,0,1,1))
selection=cv.Rect(0,0,0,0)
hdims = {16}
hranges_arr = {0,180}
hranges = {hranges_arr}
vmin,vmax,smin= 10,256,30

function hsv2rgb(hue)
  rgb={}
  sector_data={{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}}
  hue = hue *0.033333333333333333333333333333333
  sector = math.floor(hue)
  p = cv.Round(255*(hue - sector))
  p = bit32.bxor(p,(bit32.band(sector,1) and  255 or 0))

  rgb[sector_data[sector+1][1]+1] = 255
  rgb[sector_data[sector+1][2]+1] = 0
  rgb[sector_data[sector+1][3]+1] = p
  return cv.Scalar(rgb[3], rgb[2], rgb[1])
end


capture = cv.CaptureFromFile( arg[1] and arg[1] or 'tree.avi' )

if( not capture ) then error("Could not initialize capturing") end

print("Hot keys: \n\tESC - quit the program\n\tc - stop the tracking\n\tb - switch to/from backprojection view\n\th - show/hide object histogram\nTo initialize tracking, select the object with mouse\n" )

cv.NamedWindow( "Histogram", 1 )
cv.NamedWindow( "CamShiftDemo", 1 )

cv.SetMouseCallback( "CamShiftDemo", 
function ( event, x, y,flags)
  if image.origin>0 then
    y = image.height - y
  end

  if select_object>0 then
    selection.x = x<origin.x and  x or origin.x
    selection.y =y<origin.y and y or origin.y
    selection.width= selection.x + math.abs(x - origin.x)
    selection.height =selection.y + math.abs(y - origin.y)

    selection.x = selection.x > 0 and selection.x or 0
    selection.y = selection.y > 0 and selection.y or 0
    selection.width = selection.width<image.width and selection.width or image.width
    selection.height = selection.height<image.height and selection.heigh or image.height
    selection.width = selection.width - selection.x
    selection.height = selection.height-selection.y
  end
  if (event ==cv.CV_EVENT_LBUTTONDOWN) then
    origin = cv.Point(x,y)
    selection = cv.Rect(x,y,0,0)
    select_object = 1
  elseif(event==cv.CV_EVENT_LBUTTONUP) then
    select_object = 0
    if selection.width > 0 and selection.height > 0 then
      track_object = -1
    end
  end
end)

cv.CreateTrackbar( "Vmin", "CamShiftDemo", 256, function(pos) vmin=pos end )
cv.CreateTrackbar( "Vmax", "CamShiftDemo", 256, function(pos) vmax=pos end )
cv.CreateTrackbar( "Smin", "CamShiftDemo", 256, function(pos) smin=pos end )
frame=cv.QueryFrame( capture )

image = cv.CreateImage( cv.GetSize(frame), cv.IPL_DEPTH_8U, 3 )
image.origin = frame.origin
hsv = cv.CreateImage( cv.GetSize(frame),cv.IPL_DEPTH_8U, 3 )
hue = cv.CreateImage( cv.GetSize(frame),cv.IPL_DEPTH_8U, 1 )
mask = cv.CreateImage( cv.GetSize(frame),cv.IPL_DEPTH_8U, 1 )
backproject = cv.CreateImage( cv.GetSize(frame),cv.IPL_DEPTH_8U, 1 )
hist = cv.CreateHist( 1, hdims, cv.CV_HIST_ARRAY, hranges, 1 )
histimg = cv.CreateImage( cv.Size(320,200),cv.IPL_DEPTH_8U, 3 )
cv.Zero( histimg )

while true do

  frame = cv.QueryFrame( capture )
  if not frame  then break end
  cv.Copy( frame, image)
  cv.CvtColor( image, hsv, cv.CV_BGR2HSV )
  if track_object then
    _vmin,_vmax = vmin,vmax

    cv.InRangeS( hsv, cv.Scalar(0,smin,_vmin<_vmax and _vmin or _vmax ,0),
    cv.Scalar(180,256,_vmin>_vmax and _vmin or _vmax,0), mask )
    cv.Split( hsv, hue, nil, nil, nil )

    if track_object < 0 then
      max_val = 0
      cv.SetImageROI( hue, selection )
      cv.SetImageROI( mask, selection )
      cv.CalcHist( hue, hist, 0, mask )
      min_val,max_val=cv.GetMinMaxHistValue( hist)
      cv.ConvertScale( hist.bins, hist.bins, max_val and 255/max_val or 0, 0 )
      cv.ResetImageROI( hue )
      cv.ResetImageROI( mask )
      track_window = selection
      track_object = 1

      cv.Zero( histimg )
      bin_w = histimg.width / hdims[1]
      for i=0,hdims[1]-1 do
        val = cv.Round( cv.GetReal1D(hist.bins,i)*histimg.height/255 )
        color = hsv2rgb(i*180/hdims[1])
        cv.Rectangle( histimg, cv.Point(i*bin_w,histimg.height),
        cv.Point((i+1)*bin_w,histimg.height - val),
        color, -1, 8, 0 )
      end
    end
    cv.CalcBackProject( hue, backproject, hist )
    cv.And(backproject, mask, backproject )

    cv.CamShift( backproject, track_window, cv.TermCriteria( bit32.bor(cv.CV_TERMCRIT_EPS,cv.CV_TERMCRIT_ITER), 10, 1 ), track_comp, track_box )
    track_window = track_comp.rect


    if backproject_mode~=0 then
      cv.CvtColor( backproject, image, cv.CV_GRAY2BGR )
    end

    if not image.origin then
      track_box.angle = -track_box.angle
    end

    cv.EllipseBox( image, track_box, cv.CV_RGB(255,0,0), 3, cv.CV_AA, 0 )
  end
  if select_object>0 and selection.width > 0 and selection.height > 0 then
    cv.SetImageROI( image, selection )
    cv.XorS( image, cv.ScalarAll(255), image)
    cv.ResetImageROI( image )
  end

  cv.ShowImage( "CamShiftDemo", image )
  cv.ShowImage( "Histogram", histimg )

  c = cv.WaitKey(100)

  if c == 27 then break end

  if c==string.byte('b') then
    backproject_mode = bit32.bxor(backproject_mode,1)
  elseif c== string.byte('c') then
    track_object = 0
    cv.Zero( histimg )

  elseif c== string.byte('h') then
    show_hist =bit32.bxor(show_hist,1)
    if show_hist==0 then
      cv.DestroyWindow( "Histogram" )
    else
      cv.NamedWindow( "Histogram", 1 )
    end
  end
end

cv.ReleaseCapture(capture)
cv.DestroyAllWindows()
