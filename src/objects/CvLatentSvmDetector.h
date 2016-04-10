#ifndef CVLATENTSVMDETECTOR_NAME
#include "opencv2/opencv.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/legacy/compat.hpp"
#include "luacvaux.h"

#include "CvLSVMFilterObject.h"

#define CVLATENTSVMDETECTOR_NAME "CvLatentSvmDetector"

#define checkCvLatentSvmDetector(L,i) luacv_checkObject<CvLatentSvmDetector>(L,i,CVLATENTSVMDETECTOR_NAME)
#define pushCvLatentSvmDetector(L,data) luacv_pushObject<CvLatentSvmDetector>(L,data,CVLATENTSVMDETECTOR_NAME,false)

extern const struct luaL_Reg CvLatentSvmDetector_m[];
#endif
