#!/usr/bin/env lua
cv=require('luacv')

src_im=cv.LoadImage(not arg[1] and "lena.png" or arg[1])
if not src_im then error("Couldn't load image") end

--transformation=cv.LogPolar
transformation=cv.LinearPolar

polar_img=cv.CloneImage(src_im)
recovered_img=cv.CloneImage(src_im)
diff_img=cv.CloneImage(src_im)

center=cv.Point2D32f(src_im.width/2,src_im.height/2)

names={"transformation","recovered","diff"}
imgs={polar_img,recovered_img,diff_img}

transformation(src_im,polar_img,center,src_im.width,cv.CV_INTER_LINEAR+cv.CV_WARP_FILL_OUTLIERS)
transformation(polar_img,recovered_img,center,src_im.width,cv.CV_WARP_INVERSE_MAP+cv.CV_INTER_LINEAR+cv.CV_WARP_FILL_OUTLIERS)

cv.Sub(recovered_img,src_im,diff_img)

for i=1,#names do
  cv.NamedWindow(names[i])
  cv.ShowImage(names[i],imgs[i])
end

cv.WaitKey()
cv.DestroyAllWindows()
