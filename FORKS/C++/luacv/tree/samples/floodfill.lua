#!/usr/bin/env lua
cv=require('luacv')
color_img=cv.LoadImage(not arg[1] and 'fruits.png' or arg[1])
if (not color_img) then error("Couldn't load image") end
mask=cv.CreateImage(cv.Size(color_img.width+2,color_img.height+2),cv.IPL_DEPTH_8U,1)

wname={"image","mask"}
imgs={color_img,mask}
rng=cv.RNG(-1)
new_mask_val=255*(2^8) -- 255<<8

for i=1,#wname do
  cv.NamedWindow(wname[i])
end
comp=cv.ConnectedComp(0,cv.Scalar(0),cv.Rect(0,0,0,0))
function mouse_event(event,x,y,flags)
  if (event==cv.CV_EVENT_FLAG_LBUTTON) then
    seed=cv.Point(x,y)
    cv.Threshold(mask,mask,1,255,cv.CV_THRESH_BINARY)
    cv.FloodFill(color_img,seed,cv.CV_RGB(255-cv.RandInt(rng)%255,255-cv.RandInt(rng)%255,255-cv.RandInt(rng)%255),cv.CV_RGB(20,20,20),
    cv.CV_RGB(20,20,20),comp,4+new_mask_val+cv.CV_FLOODFILL_FIXED_RANGE,mask)
    for i=1,#wname do
      cv.ShowImage(wname[i],imgs[i])
    end
  end
end

cv.SetMouseCallback(wname[1],mouse_event);

for i=1,#wname do
  cv.ShowImage(wname[i],imgs[i])
end

cv.WaitKey()
cv.DestroyAllWindows()
