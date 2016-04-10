#!/usr/bin/env lua
cv=require('luacv')

DETECTION = 0
CAPTURING = 1
CALIBRATED = 2

board_size = cv.Size(8,6)
square_size = 1
aspect_ratio = 1
image_count = 10
write_extrinsics = 0
write_points = 0
flags = 0

flip_vertical = 0
delay = 1000
prev_timestamp = 0
font = cv.Font( 1, 1 )
avg_reproj_err = 0
mode = DETECTION
undistort_image = 0

image_points_buf ={}
for i=1,board_size.width*board_size.height do
  image_points_buf[i]=cv.Point2D32f(0,0)
end


capture = cv.CaptureFromFile('cam.avi') or error("The input file could not be opened")
mode = DETECTION
function MAX(i,j) return  end

elem_size = board_size.width*board_size.height*cv.CvPoint2D32f["size"]
storage = cv.CreateMemStorage( elem_size*4> bit32.lshift(1, 16)and elem_size*4 or bit32.lshift(1, 16))

image_points_seq = cv.CreateSeq( 0, cv.CvSeq["size"], elem_size, storage )

cv.NamedWindow( "Image View", 1 )

while true do
  text_origin=cv.Point(0,0)
  text_size = cv.Size(0,0)
  base_line = 0
  blink=0

  view0 = cv.QueryFrame( capture ) 
  mode=CAPTURING
  if not view0 then
    break
  end


  if view0  then
    view = cv.CreateImage( cv.GetSize(view0), cv.IPL_DEPTH_8U, view0.nChannels )
    if view0.origin == cv.IPL_ORIGIN_BL then
      cv.Flip( view0, view )
    else
      cv.Copy( view0, view )
    end
  end

  if flip_vertical >0 then
    cv.Flip( view, view)
  end


  found,count = cv.FindChessboardCorners( view, board_size,image_points_buf, cv.CV_CALIB_CB_ADAPTIVE_THRESH)

  view_gray = cv.CreateImage( cv.GetSize(view), cv.IPL_DEPTH_8U, 1 )
  cv.CvtColor( view, view_gray, cv.CV_BGR2GRAY )
  cv.FindCornerSubPix( view_gray, image_points_buf, cv.Size(11,11),cv.Size(-1,-1), cv.TermCriteria( cv.CV_TERMCRIT_EPS+cv.CV_TERMCRIT_ITER, 30, 0.1 ))

  cv.ReleaseImage( view_gray )
  if ( mode==CAPTURING and found>0 and os.clock()-prev_timestamp > delay*0.001) then
    prev_timestamp=os.clock()
    blink=1
  end
    
  cv.DrawChessboardCorners( view, board_size, image_points_buf, count,found)

  s=count..'/'..found
  cv.GetTextSize( s, font,text_size, base_line )
  text_origin.x = view.width - text_size.width - 10
  text_origin.y = view.height - base_line - 10

  cv.PutText( view, s, text_origin, font, mode ~= CALIBRATED and cv.CV_RGB(255,0,0) or cv.CV_RGB(0,255,0))
  
  if (blink>0) then
    cv.Not(view,view)
  end


  cv.ShowImage( "Image View", view )
  cv.ReleaseImage( view )
  key = cv.WaitKey(50)

  if key == 27 then
    break
  end
end
cv.ReleaseCapture( capture )
cv.ReleaseMemStorage( storage )
