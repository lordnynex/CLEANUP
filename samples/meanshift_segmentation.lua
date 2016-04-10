#!/usr/bin/env lua
cv=require('luacv')


comp=cv.ConnectedComp(0,cv.Scalar(0),cv.Rect(0,0,0,0))
function floodFillPostprocess( img, color )
  rng = cv.RNG(-1);
  mask=cv.CreateMat(img.height+2,img.width+2, cv.CV_8UC1);
  cv.Zero(mask)

  for y=0,img.width-1 do
    for x=0,img.height-1 do
      pt=cv.Get2D(mask,x+1,y+1)
      if pt.val[1] == 0 then
        newVal=cv.Scalar(cv.RandInt(rng)%180 + 50,cv.RandInt(rng)%180 + 50,cv.RandInt(rng)%180 + 50)
        cv.FloodFill(img,cv.Point(y,x),newVal,color,color,comp,4,mask)
      end
    end
  end
end

winName = "meanshift";

function meanShiftSegmentation(pos)
  cv.PyrMeanShiftFiltering( img, res, spatialRad, colorRad, maxPyrLevel );
  floodFillPostprocess( res, cv.ScalarAll(2));
  cv.ShowImage(winName, res );
end

img = cv.LoadImage( arg[1] and arg[1] or 'squares.png') or error("can't load image")
res=cv.CreateImage(cv.GetSize(img),cv.IPL_DEPTH_8U,3)
spatialRad = 10;
colorRad = 10;
maxPyrLevel = 1;

cv.NamedWindow( winName, cv.CV_WINDOW_AUTOSIZE );

meanShiftSegmentation();
cv.WaitKey();
