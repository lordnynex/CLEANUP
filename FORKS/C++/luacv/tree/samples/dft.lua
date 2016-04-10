#!/usr/bin/env lua
local cv=require("luacv")

function ShiftDFT(src_im)
    local center=cv.Point(src_im.height/2,src_im.width/2)
    local qstub={} 
    for i=1,4 do
        qstub[i]=cv.CreateMatHeader(src_im.height,src_im.width,cv.CV_64FC1)
    end

    local q1=cv.GetSubRect(src_im,qstub[1],cv.Rect(0,0,center.x,center.y))
    local q2=cv.GetSubRect(src_im,qstub[2],cv.Rect(center.x,0,center.x,center.y))
    local q3=cv.GetSubRect(src_im,qstub[3],cv.Rect(center.x,center.y,center.x,center.y))
    local q4=cv.GetSubRect(src_im,qstub[4],cv.Rect(0,center.y,center.x,center.y))
    
    local tmp=cv.CreateMat(src_im.height/2,src_im.width/2,cv.CV_64FC1)

    cv.Copy(q3,tmp)
    cv.Copy(q1,q3)
    cv.Copy(tmp,q1)
    cv.Copy(q4,tmp)
    cv.Copy(q2,q4)
    cv.Copy(tmp,q2)
end

filename=not arg[1] and "lena.png" or arg[1]

im = cv.LoadImage( filename, cv.CV_LOAD_IMAGE_GRAYSCALE );
if not im then error("Couldn't load image") end

im_size=cv.GetSize(im)
realInput = cv.CreateImage( im_size,cv.IPL_DEPTH_64F, 1);
imaginaryInput = cv.CreateImage( im_size, cv.IPL_DEPTH_64F, 1);
complexInput = cv.CreateImage( im_size, cv.IPL_DEPTH_64F, 2);

cv.Scale(im, realInput, 1.0, 0.0);
cv.Zero(imaginaryInput);
cv.Merge(realInput, imaginaryInput, nil, nil, complexInput);

dft_M = cv.GetOptimalDFTSize( im.height - 1 );
dft_N = cv.GetOptimalDFTSize( im.width - 1 );

dft_A = cv.CreateMat( dft_M, dft_N, cv.CV_64FC2 );
dft_size=cv.Size(dft_N, dft_M)
image_Re = cv.CreateImage( dft_size, cv.IPL_DEPTH_64F, 1);
image_Im = cv.CreateImage( dft_size, cv.IPL_DEPTH_64F, 1);
tmp=cv.CreateMatHeader(dft_M,dft_N,cv.CV_64FC2)

-- copy A to dft_A and pad dft_A with zeros
cv.GetSubRect( dft_A, tmp, cv.Rect(0,0, im.width, im.height));
cv.Copy( complexInput, tmp, nil );
if dft_A.cols > im.width then
  cv.GetSubRect( dft_A, tmp, cvRect(im.width,0, dft_A.cols - im.width, im.height));
  cv.Zero(tmp);
end

-- no need to pad bottom part of dft_A with zeros because of
-- use nonzero_rows parameter in cvDFT() call below

cv.DFT( dft_A, dft_A, cv.CV_DXT_FORWARD, complexInput.height );

cv.NamedWindow("win", 0);
cv.NamedWindow("magnitude", 0);
cv.ShowImage("win", im);

-- Split Fourier in real and imaginary parts
cv.Split( dft_A, image_Re, image_Im, nil, nil );

-- Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
cv.Pow( image_Re, image_Re, 2.0);
cv.Pow( image_Im, image_Im, 2.0);
cv.Add( image_Re, image_Im, image_Re, nil);
cv.Pow( image_Re, image_Re, 0.5 );

-- Compute log(1 + Mag)
cv.AddS( image_Re, cv.ScalarAll(1.0), image_Re, nil );
cv.Log( image_Re, image_Re ); 


-- Rearrange the quadrants of Fourier image so that the origin is at
-- the image center
ShiftDFT( image_Re);

m,M=cv.MinMaxLoc(image_Re)
cv.Scale(image_Re, image_Re, 1.0/(M-m), 1.0*(-m)/(M-m));
cv.ShowImage("magnitude", image_Re);

cv.WaitKey(-1);
cv.DestroyAllWindows()
