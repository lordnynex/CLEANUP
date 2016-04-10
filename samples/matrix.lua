#!/usr/bin/env lua
cv=require("luacv")

local orig_data={1,2,3.3,4,5,6,7,8}
local mat_type=cv.CV_32SC2
local mat=cv.Mat(2,2,mat_type,orig_data)
function printMat(mat,mat_type)
  for i=0,mat.rows -1 do 
    for j=0,mat.cols -1 do
      for k,l in pairs(cv.mGet(mat,mat_type,i,j)) do
        print("mat["..i..","..j..","..k.."]="..l)
      end
    end
  end
end

print("original matrix")
printMat(mat,mat_type)
print("--------------")
print("average value")
average=0
for i=0,mat.rows-1 do 
  for j=0,mat.cols-1 do
    data=cv.mGet(mat,mat_type,i,j)
    for k=1,#data do
      average=average+data[k]
    end
  end
end

print(average/(mat.rows*mat.cols*#data))
print("----------------------")

for i=0,mat.rows-1 do 
  for j=0,mat.cols-1 do
    data=cv.mGet(mat,mat_type,i,j)
    for k=1,#data do
      data[k]=data[k]^2
    end
     cv.mSet(mat,mat_type,i,j,data)
  end
end

print("power")
printMat(mat,mat_type)
print("--------------")
cv.Zero(mat)
print("Zero matrix")
printMat(mat,mat_type)
print(mat)

--cv.ReleaseMat(mat)
print("-------------")
