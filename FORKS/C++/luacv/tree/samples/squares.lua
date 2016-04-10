#!/usr/bin/env lua

cv=require('luacv')

wname="Square detection"
thresh=50
N=11

function angle(pt1,pt2,pt0)
  dx1 = pt1.x - pt0.x
  dy1 = pt1.y - pt0.y
  dx2 = pt2.x - pt0.x
  dy2 = pt2.y - pt0.y
  return (dx1*dx2 + dy1*dy2)/cv.Sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
end


function findSquares(img,storage)
  sz=cv.Size(img.width-2,img.height-2)
  timg=cv.CloneImage(img)
  gray=cv.CreateImage(sz,cv.IPL_DEPTH_8U,1)
  pyr=cv.CreateImage(cv.Size(sz.width/2,sz.height/2),cv.IPL_DEPTH_8U,3)

  squares=cv.CreateSeq(0,cv.CvSeq["size"],cv.CvPoint["size"],storage)
  cv.SetImageROI(timg,cv.Rect(0,0,sz.width,sz.height))
  cv.PyrDown(timg,pyr,7)
  cv.PyrUp(pyr,timg,7)
  tgray=cv.CreateImage(sz,cv.IPL_DEPTH_8U,1) 
  for c=0,2 do
    cv.SetImageCOI(timg,c+1)
    cv.Copy(timg,tgray,nil)

    for l=0,N-1 do
      if (not l) then
        cv.Canny(tgray,gray,0,thresh,5)
        cv.Dilate(gray,gray,0,1)
      else
        cv.Threshold(tgray,gray,(l+1)*255/N,255,cv.CV_THRESH_BINARY)
      end

      a,contours=cv.FindContours(gray,storage,cv.CvContour["size"],cv.CV_RETR_LIST,cv.CV_CHAIN_APPROX_SIMPLE,cv.Point(0,0))

      while contours do
        result=cv.ApproxPoly(contours,cv.CvContour["size"],storage,cv.CV_POLY_APPROX_DP, cv.ContourPerimeter(contours)*0.02,0)
        if (result.total==4 and cv.ContourArea(result,cv.Slice(0,result.total),0)>1000 and cv.CheckContourConvexity(result)) then
          s=0
          i=0
          for i=0,4 do
            if (i>=2) then
              t=math.abs(angle(
              cv.GetSeqElem(result,i,cv.CvPoint["name"]),
              cv.GetSeqElem(result,i-2,cv.CvPoint["name"]),
              cv.GetSeqElem(result,i-1,cv.CvPoint["name"])))
              s=s>t and s or t
            end
          end

          if s<0.3 then
            for i=0,3 do
              cv.SeqPush(squares,cv.GetSeqElem(result,i,cv.CvPoint["name"]))    
            end
          end
        end
        contours=contours.h_next
      end
    end
  end
  return squares
end

function drawSquares(img,squares)
  for i=0,squares.total-1,4 do
    rect={cv.GetSeqElem(squares,i,cv.CvPoint["name"]),
          cv.GetSeqElem(squares,i+1,cv.CvPoint["name"]),
          cv.GetSeqElem(squares,i+2,cv.CvPoint["name"]),
          cv.GetSeqElem(squares,i+3,cv.CvPoint["name"])}
    
    count={#rect}
    cv.PolyLine(img,rect,count,1,1,cv.CV_RGB(255,255,0),3,cv.CV_AA,0)
  end
  cv.ShowImage(wname,img)
end

storage=cv.CreateMemStorage()
src_im=cv.LoadImage(not arg[1] and 'squares.png' or arg[1])
if not src_im then error("Couldn't load source image "..arg[1]) end
img=cv.CloneImage(src_im)
cv.NamedWindow(wname)

drawSquares(img,findSquares(img,storage))

cv.WaitKey()
cv.ClearMemStorage(storage)
cv.DestroyWindow(wname)
