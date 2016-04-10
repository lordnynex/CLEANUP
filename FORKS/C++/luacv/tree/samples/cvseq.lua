#!/usr/bin/env lua
local cv=require("luacv")
local el2={cv.Slice(1,2),cv.Slice(3,4)}
local el={cv.Slice(5,6),cv.Slice(7,8),cv.Slice(9,10)}
local seq=cv.CreateSeq(0,cv.CvSeq["size"],cv.CvSlice["size"],cv.CreateMemStorage())

print("Blank sequence","seq.total="..seq.total)

cv.SeqPushMulti(seq,el2,#el2)
cv.SeqInsert(seq,0,el[1])
cv.SeqInsert(seq,0,el[2])
cv.SeqInsert(seq,0,el[3])

print("Filled sequence","seq.total="..seq.total)

local elements={}
cv.SeqPopMulti(seq,elements,2,0,cv.CvSlice["name"])
for i,j in pairs(elements) do print(j) end

print(cv.GetSeqElem(seq,0,cv.CvSlice["name"]))
print("converted sequnce to array")
cv.CvtSeqToArray(seq,elements,cv.Slice(0,3),cv.CvSlice["name"])
for i,j in pairs(elements) do print(j) end
