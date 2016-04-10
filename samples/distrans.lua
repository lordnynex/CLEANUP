#!/usr/bin/env lua

cv=require('luacv')

wndname="distance transform"
mask_size=cv.CV_DIST_MASK_5
edge_thresh=100
dist_type=cv.CV_DIST_L2

gray=cv.LoadImage(not arg[1] and 'lena.png' or arg[1],0)
if (not gray) then error("Couldn't load source image ") end

dist=cv.CreateImage(cv.GetSize(gray),cv.IPL_DEPTH_32F,1)
dist8u1=cv.CloneImage(gray)
dist8u2=cv.CloneImage(gray)
dist8u=cv.CreateImage(cv.GetSize(gray),cv.IPL_DEPTH_8U,3)
dist32s=cv.CreateImage(cv.GetSize(gray),cv.IPL_DEPTH_32S,1)

edge=cv.CloneImage(gray)
labels=cv.CreateImage(cv.GetSize(gray),cv.IPL_DEPTH_32S,1)

cv.NamedWindow(wndname)
colors={{0,0,0},{255,0,0},{255,128,0},{255,255,0},{0,255,0},
        {0,128,255},{0,255,255},{0,0,255},{255,0,255}}

function on_change(pos)
  edge_thresh=pos
  cv.Threshold(gray,edge,edge_thresh,edge_thresh,cv.CV_THRESH_BINARY)  
  cv.DistTransform(edge,dist,dist_type,mask_size,{},labels)
  cv.ConvertScale(dist,dist,5000,0)
  cv.Pow(dist,dist,0.5)
  
  cv.ConvertScale(dist,dist32s,1,0.5)
  cv.AndS(dist32s,cv.ScalarAll(255),dist32s,nil)
  cv.ConvertScale(dist32s,dist8u1,1,0)
  cv.ConvertScale(dist32s,dist32s,-1,0)
  cv.AddS(dist32s,cv.ScalarAll(255),dist32s,nil)
  cv.ConvertScale(dist32s,dist8u2,1,0)
  cv.Merge(dist8u1,dist8u2,dist8u2,nil,dist8u)
  cv.ShowImage(wndname,dist8u)
  end

range=200
cv.CreateTrackbar("track",wndname,range,on_change)

on_change(range/2)
cv.WaitKey()

cv.DestroyAllWindows()
