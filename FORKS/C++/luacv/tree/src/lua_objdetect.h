#include "opencv2/opencv.hpp"
#include "luacvaux.h"
#include "lua_core.h"

#include "CvHaarFeature.h"
#include "CvHaarClassifier.h"
#include "CvHaarStageClassifier.h"
#include "CvHaarClassifierCascade.h"
#include "CvLSVMFilterPosition.h"
#include "CvLSVMFilterObject.h"
#include "CvLatentSvmDetector.h"
#include "CvObjectDetection.h"

#define objdetectReg()\
  objectReg(CVHAARFEATURE_NAME,CvHaarFeature_m);\
  objectReg(CVHAARCLASSIFIER_NAME,CvHaarClassifier_m);\
  objectReg(CVHAARSTAGECLASSIFIER_NAME,CvHaarStageClassifier_m);\
  objectReg(CVHAARCLASSIFIERCASCADE_NAME,CvHaarClassifierCascade_m);\
  objectReg(CVLSVMFILTERPOSITION_NAME,CvLSVMFilterPosition_m);\
  objectReg(CVLSVMFILTEROBJECT_NAME,CvLSVMFilterObject_m);\
  objectReg(CVLATENTSVMDETECTOR_NAME,CvLatentSvmDetector_m);\
  objectReg(CVOBJECTDETECTION_NAME,CvObjectDetection_m);\


extern const luaL_Reg objdetect[];
extern const luacv_var objdetect_var[];
extern const luacv_var objdetect_object[];

