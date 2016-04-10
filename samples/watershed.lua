#!/usr/bin/env lua
cv=require('luacv')

storage=cv.CreateMemStorage()
rng=cv.RNG(-1)
prev_pt=cv.Point(-1,-1)
wname={"marker area","watershed"}
img0=cv.LoadImage(not arg[1] and "fruits.png" or arg[1])
if (not img0) then error("Couldn't load image") end

cv.NamedWindow(wname[1])
cv.NamedWindow(wname[2])

img=cv.CloneImage(img0)
img_gray=cv.CloneImage(img0)
wshed=cv.CloneImage(img0)

marker_mask=cv.CreateImage(cv.GetSize(img),cv.IPL_DEPTH_8U,1)
markers=cv.CreateImage(cv.GetSize(img),cv.IPL_DEPTH_32S,1)

cv.CvtColor(img,marker_mask,cv.CV_BGR2GRAY)
cv.CvtColor(marker_mask,img_gray,cv.CV_GRAY2BGR)

cv.Zero(marker_mask)
cv.Zero(wshed)
cv.ShowImage(wname[1],img)
cv.ShowImage(wname[2],wshed)

cv.SetMouseCallback(wname[1],
  function (event,x,y,flags)
    --flags==33 --- flags & CV_EVENT_FLAG_LBUTTON
    if ((event==cv.CV_EVENT_LBUTTONUP) or not (flags==33)) then
      prev_pt=cv.Point(-1,-1)
    elseif(event==cv.CV_EVENT_LBUTTONDOWN) then
      prev_pt=cv.Point(x,y)
    elseif(event==cv.CV_EVENT_MOUSEMOVE and flags==33) then
      pt=cv.Point(x,y)
      if (prev_pt.x<0) then prev_pt=pt end
      cv.Line(marker_mask, prev_pt,pt,cv.ScalarAll(255),5,8,0)
      cv.Line(img,prev_pt,pt,cv.ScalarAll(255),5,8,0)
      prev_pt=pt
      cv.ShowImage(wname[1],img)
    end
  end)

while (true) do
  if (cv.WaitKey()==27) then break end
  cv.ClearMemStorage(storage)
  comp_count=0
  a,contours=cv.FindContours(marker_mask,storage)
  cv.Zero(markers)

  while(contours) do
    cv.DrawContours(markers,contours,cv.ScalarAll(comp_count+1),
                cv.ScalarAll(comp_count+1),-1,-1,8,cv.Point(0,0))
    comp_count=comp_count+1
    contours=contours.h_next
  end

  if (comp_count~=0) then

    color_tab=cv.CreateMat(1,comp_count,cv.CV_8UC3)
    for i=0,comp_count-1 do
      cv.Set1D(color_tab,i,cv.Scalar(cv.RandInt(rng)%180 + 50,
                                     cv.RandInt(rng)%180 + 50,
                                     cv.RandInt(rng)%180 + 50,
                                     0))
    end

    cv.Watershed(img0,markers)

    for i=0,markers.height-1 do
      for j=0,markers.width-1 do
        tmp=cv.Get2D(markers,i,j)
        idx=tmp.val[1]
        if (idx==-1) then
          cv.Set2D(wshed,i,j,cv.ScalarAll(255))
        elseif (idx<=0 or idx>comp_count) then
          cv.Set2D(wshed,i,j,cv.ScalarAll(0))
        else
          cv.Set2D(wshed,i,j,cv.Get1D(color_tab,(idx-1)))
        end
      end
    end
    cv.AddWeighted(wshed,0.5,img_gray,0.5,0,wshed)
    cv.ShowImage(wname[2],wshed)
  end
end

cv.DestroyAllWindows()
