#!/usr/bin/env lua

cv=require('luacv')
local NUMBER=100
local DELAY=5
local wndname="Drawing Demo"

function random_color(rng)
  return cv.CV_RGB(cv.RandInt(rng)%255,cv.RandInt(rng)%255,cv.RandInt(rng)%255);
end

local line_type = cv.CV_AA
local angle;
local width=1000
local height=700
local width3 = width*3
local height3 = height*3
local ymin = 0;

image=cv.CreateImage( cv.Size(width,height), cv.IPL_DEPTH_8U, 3 );

cv.NamedWindow(wndname, 1 );
cv.Zero( image );
cv.ShowImage(wndname,image);
cv.WaitKey(DELAY);

local rng = cv.RNG(-1);
pt1=cv.Point(0,0)
pt2=cv.Point(0,0)

for i=0,NUMBER-1 do
  pt1.x=cv.RandInt(rng) % width3 - width;
  pt1.y=cv.RandInt(rng) % height3 - height;
  pt2.x=cv.RandInt(rng) % width3 - width;
  pt2.y=cv.RandInt(rng) % height3 - height;

  cv.Line( image, pt1, pt2, random_color(rng), cv.RandInt(rng)%10, line_type, 0 );
  cv.ShowImage(wndname,image);
  if (cv.WaitKey(DELAY) >= 0) then os.exit(0) end;
end

for i=0,NUMBER-1 do
  pt1.x=cv.RandInt(rng) % width3 - width;
  pt1.y=cv.RandInt(rng) % height3 - height;
  pt2.x=cv.RandInt(rng) % width3 - width;
  pt2.y=cv.RandInt(rng) % height3 - height;

  cv.Rectangle( image,pt1, pt2, random_color(rng), cv.RandInt(rng)%10-1, line_type, 0 );
  cv.ShowImage(wndname,image);
  if(cv.WaitKey(DELAY) >= 0) then os.exit(0) end
end

local sz=cv.Size(0,0)
for i=0,NUMBER-1 do
  pt1.x=cv.RandInt(rng) % width3 - width;
  pt1.y=cv.RandInt(rng) % height3 - height;
  sz.width =cv.RandInt(rng)%200;
  sz.height=cv.RandInt(rng)%200;
  angle = (cv.RandInt(rng)%1000)*0.180;

  cv.Ellipse( image, pt1, sz, angle, angle - 100, angle + 200,
  random_color(rng), cv.RandInt(rng)%10-1, line_type, 0 );
  cv.ShowImage(wndname,image);
  if (cv.WaitKey(DELAY) >= 0) then os.exit(0) end
end
local pt={{cv.Point(0,0),cv.Point(0,0),cv.Point(0,0)},{cv.Point(0,0),cv.Point(0,0),cv.Point(0,0)}}

local arr={2,3}
for i=0,NUMBER-1 do
  for j=1,arr[1] do
    for k=1,arr[2] do
      pt[j][k].x=cv.RandInt(rng) % width3 - width;
      pt[j][k].y=cv.RandInt(rng) % height3 - height;
    end
  end
  cv.PolyLine( image, pt, arr, 2, 1, random_color(rng), cv.RandInt(rng)%10, line_type, 0 );
  cv.ShowImage(wndname,image);
  if(cv.WaitKey(DELAY) >= 0) then os.exit(0); end
end

for i=0,NUMBER-1 do
  for j=1,arr[1] do
    for k=1,arr[2] do
      pt[j][k].x=cv.RandInt(rng) % width3 - width;
      pt[j][k].y=cv.RandInt(rng) % height3 - height;
    end
  end

  cv.FillPoly( image, pt, arr, 2, random_color(rng), line_type, 0 );
  cv.ShowImage(wndname,image);
  if(cv.WaitKey(DELAY) >= 0) then os.exit(0) end
end

for i=0,NUMBER-1 do
  pt1.x=cv.RandInt(rng) % width3 - width;
  pt1.y=cv.RandInt(rng) % height3 - height;

  cv.Circle( image, pt1, cv.RandInt(rng)%300, random_color(rng),
  cv.RandInt(rng)%10-1, line_type, 0 );
  cv.ShowImage(wndname,image);
  if (cv.WaitKey(DELAY) >= 0) then os.exit(0) end
end

local font=cv.Font(1)
for i=1,NUMBER-1 do
  pt1.x=cv.RandInt(rng) % width3 - width;
  pt1.y=cv.RandInt(rng) % height3 - height;
  cv.InitFont( font, cv.RandInt(rng) % 8,
  (cv.RandInt(rng)%100)*0.05+0.1, (cv.RandInt(rng)%100)*0.05+0.1,
  (cv.RandInt(rng)%5)*0.1, cv.Round(cv.RandInt(rng)%10), line_type );

  cv.PutText( image, "Testing text rendering!", pt1, font, random_color(rng));
  cv.ShowImage(wndname,image);
  if(cv.WaitKey(DELAY) >= 0)then os.exit(0) end
end

cv.InitFont(font,cv.CV_FONT_HERSHEY_COMPLEX, 3, 3, 0.0, 5, line_type );
local text_size=cv.Size(0,0)
cv.GetTextSize( "OpenCV forever!", font, text_size, ymin );

pt1.x = (width - text_size.width)/2;
pt1.y = (height + text_size.height)/2;
image2 = cv.CloneImage(image);

for i=0,254 do
  cv.SubS( image2, cv.ScalarAll(i), image, nil );
  cv.PutText( image, "OpenCV forever!", pt1, font, cv.CV_RGB(255,i,i));
  cv.ShowImage(wndname,image);

  if(cv.WaitKey(DELAY) >= 0) then os.exit(0) end
end
cv.WaitKey();
cv.DestroyAllWindows()
