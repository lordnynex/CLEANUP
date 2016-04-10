#include "opencv2/opencv.hpp"
#include "luacvaux.h"
#include "lua_core.h"

#include "CvMoments.h"
#include "CvHuMoments.h"
#include "CvConnectedComp.h"
#include "CvSubdiv2D.h"
#include "CvQuadEdge2D.h"
#include "CvSubdiv2DPoint.h"
#include "CvContourScanner.h"
#include "CvChainPtReader.h"
#include "CvFeatureTree.h"
#include "CvLSH.h"
#include "CvLSHOperations.h"
#include "CvSubdiv2DEdge.h"

#define imgprocReg()\
  objectReg(CVMOMENTS_NAME,CvMoments_m);\
  objectReg(CVHUMOMENTS_NAME,CvHuMoments_m);\
  objectReg(CVCONNECTEDCOMP_NAME,CvConnectedComp_m);\
  objectReg(CVSUBDIV2D_NAME,CvSubdiv2D_m);\
  objectReg(CVQUADEDGE2D_NAME,CvQuadEdge2D_m);\
  objectReg(CVSUBDIV2DPOINT_NAME,CvSubdiv2DPoint_m);\
  objectReg(CVCONTOURSCANNER_NAME,CvContourScanner_m);\
  objectReg(CVCHAINPTREADER_NAME,CvChainPtReader_m);\
  objectReg(CVFEATURETREE_NAME,CvFeatureTree_m);\
  objectReg(CVLSH_NAME,CvLSH_m);\
  objectReg(CVLSHOPERATIONS_NAME,CvLSHOperations_m);\
  objectReg(CVSUBDIV2DEDGE_NAME,CvSubdiv2DEdge_m);

extern const luaL_Reg imgproc[];
extern const luacv_var imgproc_var[];
extern const luacv_var imgproc_object[];
