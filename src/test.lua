cv=require('luacv')

src=cv.LoadImage("/home/adit/luacv/src/test.png")
if (not src) then error("Can't load source image "..arg[1]) end

mat1 = cv.CreateMat(250, 183, cv.CV_8UC3)
cv.Resize(src, mat1, cv.CV_INTER_NN)
cv.SaveImage("/home/adit/luacv/src/mat1.png", mat1)

-- mat2 = cv.CreateMat(250, 183, cv.CV_8UC3)
-- cv.Resize(src, mat2, cv.CV_INTER_LINEAR)
-- cv.SaveImage("/home/adit/luacv/src/mat2.jpeg", mat2)
-- 
-- mat3 = cv.CreateMat(250, 183, cv.CV_8UC3)
-- cv.Resize(src, mat3, cv.CV_INTER_AREA)
-- cv.SaveImage("/home/adit/luacv/src/mat3.jpeg", mat3)
-- 
-- mat4 = cv.CreateMat(250, 183, cv.CV_8UC3)
-- cv.Resize(src, mat4, cv.CV_INTER_CUBIC)
-- cv.SaveImage("/home/adit/luacv/src/mat4.jpeg", mat4)
-- 
