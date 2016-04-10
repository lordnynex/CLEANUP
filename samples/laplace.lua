#!/usr/bin/env lua
local cv=require('luacv')

local sigma = 3;
local smooth={cv.CV_GAUSSIAN,cv.CV_MEDIAN,cv.CV_BILATERAL}
local smoothType=smooth[1]

cv.NamedWindow( "Laplacian", 1 );
local frame=cv.LoadImage(arg[1] and arg[1] or "fruits.png")
if (not frame) then error ("Can't load source image "..arg[1]) end
local size=cv.GetSize(frame)

planes={}
for i=0,3 do
  planes[i] = cv.CreateImage( size, cv.IPL_DEPTH_8U, 1 );
end
laplace = cv.CreateImage( size, cv.IPL_DEPTH_16S, 1 );
colorlaplace = cv.CreateImage( size, cv.IPL_DEPTH_8U, 3 );


cv.Smooth( frame, colorlaplace, smoothType, 5*sigma, 5*sigma, sigma, sigma );
cv.Split( colorlaplace, planes[0], planes[1], planes[2], nil );
for i=0,3 do
  cv.Laplace( planes[i], laplace, 5 );
  cv.ConvertScaleAbs( laplace, planes[i], (sigma+1)*0.25, 0 );
end

cv.Merge( planes[0], planes[1], planes[2], nil, colorlaplace );
colorlaplace.origin = frame.origin;

cv.ShowImage("Laplacian", colorlaplace );

cv.WaitKey()
cv.DestroyAllWindows()
