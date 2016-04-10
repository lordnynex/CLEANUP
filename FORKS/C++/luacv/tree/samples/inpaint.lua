#!/usr/bin/env lua
cv=require('luacv')

img=cv.LoadImage(arg[1] and arg[1] or "fruits.png")
if (not img) then error("Couldn't load image") end
inpainted=cv.CloneImage(img)
inpaint_mask=cv.CreateImage(cv.GetSize(img),cv.IPL_DEPTH_8U,1)
cv.Zero(inpaint_mask)
cv.Zero(inpainted)
cv.NamedWindow("image")
cv.NamedWindow("inpainted")

prev_pt=cv.Point(-1,-1)
cv.ShowImage("image",img)
cv.SetMouseCallback("image",
  function (event,x,y,flags)
    --flag==33 --- flags & CV_EVENT_FLAG_LBUTTON
    if ((event==cv.CV_EVENT_LBUTTONUP) or (not flags==33)) then 
      prev_pt=cv.Point(-1,-1)
    elseif (event==cv.CV_EVENT_LBUTTONUP) then
      prev_pt=cv.Point(x,y)
    elseif (event==cv.CV_EVENT_MOUSEMOVE and (flags==33)) then
      pt=cv.Point(x,y)
      if (prev_pt.x<0) then
        prev_pt=pt
      end
      cv.Line(inpaint_mask,prev_pt,pt,cv.ScalarAll(255),5,8,0)
      cv.Line(img,prev_pt,pt,cv.ScalarAll(255),5,8,0)
      prev_pt=pt
      cv.ShowImage("image",img)
    end
  end
)

while(true) do 
  if (cv.WaitKey(1000)==27) then break; end
  cv.Inpaint(img,inpaint_mask,inpainted,3,cv.CV_INPAINT_TELEA) 
  cv.ShowImage("inpainted",inpainted)
end

cv.DestroyAllWindows()
