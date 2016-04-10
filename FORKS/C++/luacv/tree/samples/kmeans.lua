#!/usr/bin/env lua

cv=require('luacv')

color_tab={ cv.CV_RGB(255,0,0),
            cv.CV_RGB(0,255,0),
            cv.CV_RGB(100,100,255),
            cv.CV_RGB(255,0,255),
            cv.CV_RGB(255,255,0)}

local wname="clusters"

cv.NamedWindow(wname)
rng=cv.RNG(-1);
img=cv.CreateImage(cv.Size(500,500),cv.IPL_DEPTH_8U,3)

while(true) do
  cluster_count=cv.RandInt(rng)%5 +1
  sample_count=cv.RandInt(rng)%1000 +1
  points=cv.CreateMat(sample_count,1,cv.CV_32FC1)
  clusters=cv.CreateMat(sample_count,1,cv.CV_32SC1)
  cluster_count=(cluster_count>sample_count and sample_count or cluster_count)

  for k=0,cluster_count-1 do 
    center=cv.Point(cv.RandInt(rng)%img.width,cv.RandInt(rng)%img.height)
    point_chunk=cv.CreateMatHeader(1,1,cv.CV_32SC1)

    cv.GetRows(points,point_chunk,k*sample_count/cluster_count,
    k==cluster_count-1 and sample_count or (k+1)*sample_count/cluster_count,1)

    cv.RandArr(rng,point_chunk,cv.CV_RAND_NORMAL,
               cv.Scalar(center.x,center.y,0,0),
               cv.Scalar(img.width*0.1,img.height*0.1,0,0))
  end

  for i=0,(sample_count/2)-1 do
    idx1=cv.RandInt(rng)%sample_count;
    idx2=cv.RandInt(rng)%sample_count;
    pt1=cv.GetReal1D(points,idx1) 
    pt2=cv.GetReal1D(points,idx2) 
    cv.SetReal1D(points,idx2,pt1)
    cv.SetReal1D(points,idx1,pt2)
  end

  cv.KMeans2(points,cluster_count,clusters,cv.TermCriteria(cv.CV_TERMCRIT_EPS+cv.CV_TERMCRIT_ITER,10,1),5,rng,0)
  cv.Zero(img)
  for i=0,sample_count-2 do
    cluster_idx=cv.Get1D(clusters,i)
    cluster_idx=cluster_idx.val
    cluster_idx=cluster_idx[1]
    cv.Circle(img,cv.Point(cv.GetReal1D(points,i),cv.GetReal1D(points,i+1)),2,color_tab[cluster_idx+1],cv.CV_FILLED,cv.CV_AA,0)
  end

  points=nil
  clusters=nil
  collectgarbage('collect')

  cv.ShowImage(wname,img)
  if (cv.WaitKey()==27) then break end
end
cv.DestroyWindow(wname)
