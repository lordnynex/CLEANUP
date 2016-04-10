#include "lua_objdetect.h"

static int luacv_cvHaarDetectObjects(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" HaarDetectObjects("CVARR_NAME" image,"CVHAARCLASSIFIERCASCADE_NAME" cascade, "CVMEMSTORAGE_NAME" storage, num scale_factor=1.1, int min_neighbours=3, int flags=0, "CVSIZE_NAME" min_size=Size(0,0))";
  double scale_factor=1.1;
  int min_neighbours=3,flags=0;
  CvSize min_size=cvSize(0,0),*s=&min_size;
  switch(lua_gettop(L))
  {

    case 3:
      break;
    case 4:
      scale_factor=checknumber(L,4);
      break;
    case 5:
      scale_factor=checknumber(L,4);
      min_neighbours=checkint(L,5);
      break;
    case 6:
      scale_factor=checknumber(L,4);
      min_neighbours=checkint(L,5);
      flags=checkint(L,6);
      break;
    case 7:
      scale_factor=checknumber(L,4);
      min_neighbours=checkint(L,5);
      flags=checkint(L,6);
      s=checkCvSize(L,7);
      break;

    default:
      luaL_error(L,f_msg);
  }

  pushCvSeq(L,cvHaarDetectObjects(checkCvArr(L,1),checkCvHaarClassifierCascade(L,2),checkCvMemStorage(L,3),scale_factor,min_neighbours,flags,*s));
  return 1;
}

static int luacv_cvReleaseHaarClassifierCascade(lua_State *L)
{
  const char f_msg[]="ReleaseHaarClassifierCascade("CVHAARCLASSIFIERCASCADE_NAME" cascade)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvHaarClassifierCascade(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cvSetImagesForHaarClassifierCascade(lua_State *L)
{
  const char f_msg[]="SetImagesHaarClassifierCascade("CVHAARCLASSIFIERCASCADE_NAME" cascade, "CVARR_NAME" sum, "CVARR_NAME" sqsum, "CVARR_NAME" tilted_sum, num scale)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  cvSetImagesForHaarClassifierCascade(checkCvHaarClassifierCascade(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4),checknumber(L,5));
  return 0;
}

static int luacv_cvRunHaarClassifierCascade(lua_State *L)
{
  const char f_msg[]="num RunHaarClassifierCascade("CVHAARCLASSIFIERCASCADE_NAME" cascade, "CVPOINT_NAME" pt, int start_stage=0)";
  lua_Integer stage=0;
  switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      stage=checkint(L,3);
      break;
    default:
      luaL_error(L,f_msg);
  }

  lua_pushnumber(L,cvRunHaarClassifierCascade(checkCvHaarClassifierCascade(L,1),*checkCvPoint(L,2),stage));
  return 1;
}

static int luacv_cvLoadLatentSvmDetector(lua_State *L)
{
  const char f_msg[]=CVLATENTSVMDETECTOR_NAME" LoadLatentSvmDetector(string filename)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvLatentSvmDetector(L,cvLoadLatentSvmDetector(checkstring(L,1)));
  return 1;
}

static int luacv_cvReleaseLatentSvmDetector(lua_State *L)
{
  const char f_msg[]="ReleaseLatentSvmDetector("CVLATENTSVMDETECTOR_NAME" detector)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvLatentSvmDetector(L,1);
  luacv_releaseObject(L,1);
  return 0;
}


static int luacv_cvLatentSvmDetectObjects(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" LatentSvmDetectObjects("IPLIMAGE_NAME" image, "CVLATENTSVMDETECTOR_NAME" detector, "CVMEMSTORAGE_NAME" storage, num overlap_threshold=0.5, int numThreads=-1)";
  lua_Integer numThreads=0;
  lua_Number overlap_threshold=0.5;
  switch(lua_gettop(L))
  {
    case 3:
      break;
    case 4:
      overlap_threshold=checknumber(L,4);
      break;
    case 5:
      overlap_threshold=checknumber(L,4);
      numThreads=checkint(L,5);
      break;
    default:
      luaL_error(L,f_msg);
  }

  pushCvSeq(L,cvLatentSvmDetectObjects(checkIplImage(L,1),checkCvLatentSvmDetector(L,2),checkCvMemStorage(L,3),overlap_threshold,numThreads));
  return 1;
}

const luacv_var objdetect_object[]=
{
  {CVHAARFEATURE_NAME,sizeof(CvHaarFeature)},
  {CVHAARCLASSIFIER_NAME,sizeof(CvHaarClassifier)},
  {CVHAARSTAGECLASSIFIER_NAME,sizeof(CvHaarStageClassifier)},
  {CVHAARCLASSIFIERCASCADE_NAME,sizeof(CvHaarClassifierCascade)},
  {CVLSVMFILTERPOSITION_NAME,sizeof(CvLSVMFilterPosition)},
  {CVLSVMFILTEROBJECT_NAME,sizeof(CvLSVMFilterObject)},
  {CVLATENTSVMDETECTOR_NAME,sizeof(CvLatentSvmDetector)},
  {CVOBJECTDETECTION_NAME,sizeof(CvObjectDetection)},
  {NULL,0}
};



const luaL_Reg objdetect[]=
{
  // **** Haar classifier
  funcReg(ReleaseHaarClassifierCascade),        funcReg(HaarDetectObjects),
  funcReg(SetImagesForHaarClassifierCascade),   funcReg(RunHaarClassifierCascade),
  // **** latent SVM detector
  funcReg(LoadLatentSvmDetector),               funcReg(ReleaseLatentSvmDetector),
  funcReg(LatentSvmDetectObjects),
  {NULL,NULL}
};

const luacv_var objdetect_var[]=
{
  varReg(CV_HAAR_DO_CANNY_PRUNING),             varReg(CV_HAAR_SCALE_IMAGE),
  varReg(CV_HAAR_FIND_BIGGEST_OBJECT),          varReg(CV_HAAR_DO_ROUGH_SEARCH),
  {NULL,0}
};
