#!/usr/bin/env lua
cv=require('luacv')

src_image=cv.LoadImage(not arg[1] and "lena.png" or arg[1],cv.CV_LOAD_IMAGE_GRAYSCALE)
if (not src_image) then error("Couldn't load image") end

dst_image=cv.CloneImage(src_image)
hist_width=256
hist_image=cv.CreateImage(cv.Size(300,300),cv.IPL_DEPTH_8U,1)
hist_size=255
hist=cv.CreateHist(1,{hist_size},cv.CV_HIST_ARRAY,{{0,hist_width}})
wname="image"

lut_mat=cv.CreateMat(1,hist_width,cv.CV_8UC1)
_brightness=100
_contrast=100

function update_hist()
  local brightness=_brightness -100
  local contrast=_contrast -100
  if (contrast>0) then
    delta=127*contrast/100
    a=255/(255-delta*2)
    b=a*(brightness - delta)
  else
    delta=-128*contrast/100
    a=(hist_width-delta*2)/255
    b=a*brightness +  delta
  end

  for i=0,hist_width-1 do
    local v=cv.Round(a*i+b)
    if (v<0) then v=0 
    elseif (v>255) then v=255
    end
    cv.Set1D(lut_mat,i,cv.ScalarAll(v))
  end

  cv.LUT(src_image,dst_image,lut_mat)
  cv.ShowImage(wname,dst_image)

  cv.CalcHist(dst_image,hist)
  cv.Zero(dst_image)
  minval,maxval=cv.GetMinMaxHistValue(hist)
  cv.Scale(hist.bins,hist.bins,hist_image.height/maxval,0)
  cv.Set(hist_image,cv.ScalarAll(255))
  bin_w=cv.Round(hist_image.width/hist_size)
  local str=''
  for i=0,hist_size-1 do
    local val=cv.Round(cv.GetReal1D(hist.bins,i))
    cv.Rectangle(hist_image,cv.Point(i*bin_w,hist_image.height),
    cv.Point((i+1)*bin_w,hist_image.height-val),cv.ScalarAll(0),-1,8,0)
    str=str..i..' 0 '..val..'\n'
  end

  local command=[[
    set xrange [0:]]..hist_size..[[]
    set yrange [0:]
    set style fill solid 0.5
    set xlabel "grayscale"
    set ylabel "count"
    set multiplot
    set size 1, 0.5
    set origin 0.0, 0.5
    plot '-' u 1:2:3 w filledcu  t 'histogram'
    ]]..str..[[
    e
    set origin 0.0,0.0
    plot '-' u 3 s cumulative  t 'cumulative histogram'
    ]]..str..[[
    e
    unset multiplot
    ]]

  gnuplot:write(command)
  gnuplot:flush()
end


cv.NamedWindow(wname)

cv.CreateTrackbar("brightness",wname,250,function(pos) _brightness=pos update_hist() end)
cv.CreateTrackbar("contrast",wname,250,function(pos) _contrast=pos update_hist() end)

gnuplot=io.popen('gnuplot','w')
update_hist()
cv.WaitKey()
gnuplot:close()
cv.DestroyAllWindows()

