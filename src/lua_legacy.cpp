#include "lua_legacy.h"

static int luacv_cvSegmentImage(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" SegmentImage("CVARR_NAME" srcarr, "CVARR_NAME" dstarr, num canny_thresh, num ffill_thresh, "CVMEMSTORAGE_NAME" storage)";  
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  pushCvSeq(L,cvSegmentImage(checkCvArr(L,1),checkCvArr(L,2),checknumber(L,3),checknumber(L,4),checkCvMemStorage(L,5)));
  return 1;
}
static int luacv_cvCalcCovarMatrixEx(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvCalcEigenObjects(lua_State *L)
{
UNIMPL(L);
return 1;
}

static int luacv_cvCalcDecompCoeff(lua_State *L)
{
  const char f_msg[]="num CalcDecompCoeff("IPLIMAGE_NAME" obj, "IPLIMAGE_NAME" eigObj, "IPLIMAGE_NAME" avg)";  
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvCalcDecompCoeff(checkIplImage(L,1),checkIplImage(L,2),checkIplImage(L,3)));
  return 1;
}

static int luacv_cvEigenDecomposite(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvEigenProjection(lua_State *L)
{
UNIMPL(L);
return 1;
}

static int luacv_cvCreate2DHMM(lua_State *L)
{
  const char f_msg[]=CVEHMM_NAME" Create2DHMM(int[] stateNumber, int[] numMix, int obsSize)";  
  int top;
  if (((top=lua_gettop(L))!=3)||(!lua_istable(L,1))||(!lua_istable(L,2))) luaL_error(L,f_msg);
  int count1=lua_objlen(L,1);
  int count2=lua_objlen(L,2);

  int *state=(int *)luacv_alloc(sizeof(int)*count1);
  int *mix=(int *)luacv_alloc(sizeof(int)*count2);
   
  for (int i=0;i<count1;i++)
  {
    lua_rawgeti(L,1,i+1); 
    state[i]=checkint(L,top+1);
    lua_pop(L,1);
  }

  for (int i=0;i<count2;i++)
  {
    lua_rawgeti(L,1,i+1); 
    mix[i]=checkint(L,top+1);
    lua_pop(L,1);
  }

  pushCvEHMM(L,cvCreate2DHMM(state,mix,checkint(L,3)));

  luacv_free(&state);
  luacv_free(&mix);
  return 1;
}

static int luacv_cvRelease2DHMM(lua_State *L)
{
  const char f_msg[]="Release2DHMM("CVEHMM_NAME" hmm)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvEHMM(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cvCreateObsInfo(lua_State *L)
{
  const char f_msg[]=CVIMGOBSINFO_NAME" CreateObsInfo("CVSIZE_NAME" numObs, int obsSize)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  pushCvImgObsInfo(L,cvCreateObsInfo(*checkCvSize(L,1),checkint(L,2)));
  return 1;
}

static int luacv_cvReleaseObsInfo(lua_State *L)
{
  const char f_msg[]="ReleaseObsInfo("CVIMGOBSINFO_NAME" obs_info)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvImgObsInfo(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cvUniformImgSegm(lua_State *L)
{
  const char f_msg[]="UniformImgSegm("CVIMGOBSINFO_NAME" obs_info, "CVEHMM_NAME" ehmm)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvUniformImgSegm(checkCvImgObsInfo(L,1),checkCvEHMM(L,2));
  return 0;
}

static int luacv_cvInitMixSegm(lua_State *L)
{
  const char f_msg[]="InitMixSegm("CVIMGOBSINFO_NAME"[] obs_info_array, int num_img, "CVEHMM_NAME" hmm)";  
  int top;
  if (((top=lua_gettop(L))!=3)||(!lua_istable(L,1))) luaL_error(L,f_msg);
  int  num_img=checkint(L,2);
  CvImgObsInfo **obs=(CvImgObsInfo **)luacv_alloc(sizeof(CvImgObsInfo*)*num_img);
   
  for (int i=0;i<num_img;i++)
  {
    lua_rawgeti(L,1,i+1); 
    obs[i]=checkCvImgObsInfo(L,top+1);
    lua_pop(L,1);
  }
  
  cvInitMixSegm(obs,num_img,checkCvEHMM(L,3));
  return 0;
}

static int luacv_cvEstimateHMMStateParams(lua_State *L)
{
  const char f_msg[]="EstimateHMMStateParams("CVIMGOBSINFO_NAME"[] obs_info_array, int num_img, "CVEHMM_NAME" hmm)";  
  int top;
  if (((top=lua_gettop(L))!=3)||(!lua_istable(L,1))) luaL_error(L,f_msg);
  int  num_img=checkint(L,2);
  CvImgObsInfo **obs=(CvImgObsInfo **)luacv_alloc(sizeof(CvImgObsInfo*)*num_img);
   
  for (int i=0;i<num_img;i++)
  {
    lua_rawgeti(L,1,i+1); 
    obs[i]=checkCvImgObsInfo(L,top+1);
    lua_pop(L,1);
  }
  
  cvEstimateHMMStateParams(obs,num_img,checkCvEHMM(L,3));
  return 0;
}

static int luacv_cvEstimateTransProb(lua_State *L)
{
  const char f_msg[]="EstimateTransProb("CVIMGOBSINFO_NAME"[] obs_info_array, int num_img, "CVEHMM_NAME" hmm)";  
  int top;
  if (((top=lua_gettop(L))!=3)||(!lua_istable(L,1))) luaL_error(L,f_msg);
  int  num_img=checkint(L,2);
  CvImgObsInfo **obs=(CvImgObsInfo **)luacv_alloc(sizeof(CvImgObsInfo*)*num_img);
   
  for (int i=0;i<num_img;i++)
  {
    lua_rawgeti(L,1,i+1); 
    obs[i]=checkCvImgObsInfo(L,top+1);
    lua_pop(L,1);
  }
  
  cvEstimateTransProb(obs,num_img,checkCvEHMM(L,3));

  return 1;
}

static int luacv_cvEstimateObsProb(lua_State *L)
{
  const char f_msg[]="EstimateObsProb("CVIMGOBSINFO_NAME" obs_info, "CVEHMM_NAME" hmm)";  
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvEstimateObsProb(checkCvImgObsInfo(L,1),checkCvEHMM(L,2));
  return 0;
}

static int luacv_cvEViterbi(lua_State *L)
{
  const char f_msg[]="num EViterbi("CVIMGOBSINFO_NAME" obs_info, "CVEHMM_NAME" hmm)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvEViterbi(checkCvImgObsInfo(L,1),checkCvEHMM(L,2)));
  return 1;
}

static int luacv_cvMixSegmL2(lua_State *L)
{
  const char f_msg[]="MixSegmL2("CVIMGOBSINFO_NAME"[] obs_info_array, int num_img, "CVEHMM_NAME" hmm)";  
  int top;
  if (((top=lua_gettop(L))!=3)||(!lua_istable(L,1))) luaL_error(L,f_msg);
  int  num_img=checkint(L,2);
  CvImgObsInfo **obs=(CvImgObsInfo **)luacv_alloc(sizeof(CvImgObsInfo*)*num_img);
   
  for (int i=0;i<num_img;i++)
  {
    lua_rawgeti(L,1,i+1); 
    obs[i]=checkCvImgObsInfo(L,top+1);
    lua_pop(L,1);
  }
  
  cvMixSegmL2(obs,num_img,checkCvEHMM(L,3));
  return 0;
}

static int luacv_cvCreateHandMask(lua_State *L)
{
  const char f_msg[]="CreateHandMask("CVSEQ_NAME" hand_points, "IPLIMAGE_NAME" img_mask, "CVRECT_NAME" roi)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvCreateHandMask(checkCvSeq(L,1),checkIplImage(L,2),checkCvRect(L,3));
  return 0;
}

static int luacv_cvFindHandRegion(lua_State *L)
{//zmena
  const char f_msg[]=CVSEQ_NAME" FindHandRegion("CVPOINT3D32F_NAME"[] points, int count, "CVSEQ_NAME" indexs, num[6] line, "CVSIZE2D32F_NAME" size, int flag, "CVPOINT3D32F_NAME" center, "CVMEMSTORAGE_NAME" storage)";  
  int top;
  if (((top=lua_gettop(L))!=3)||(!lua_istable(L,1))||(!lua_istable(L,4))) luaL_error(L,f_msg);
  CvSeq *numbers=NULL;
  int  count=checkint(L,2);
  CvPoint3D32f *points=(CvPoint3D32f *)luacv_alloc(sizeof(CvPoint3D32f)*count);
   
  for (int i=0;i<count;i++)
  {
    lua_rawgeti(L,1,i+1); 
    points[i]=*checkCvPoint3D32f(L,top+1);
    lua_pop(L,1);
  }
  
  float line[6];

  for (int i=0;i<6;i++)
  {
    lua_rawgeti(L,4,i+1); 
    line[i]=(float)checknumber(L,top+1);
    lua_pop(L,1);
  }
 
  cvFindHandRegion(points,count,checkCvSeq(L,3),line,*checkCvSize2D32f(L,5),checkint(L,6),checkCvPoint3D32f(L,7),checkCvMemStorage(L,8),&numbers);
  pushCvSeq(L,numbers);
  return 1;
}

static int luacv_cvFindHandRegionA(lua_State *L)
{//zmena
  const char f_msg[]=CVSEQ_NAME" FindHandRegionA("CVPOINT3D32F_NAME"[] points, int count, "CVSEQ_NAME" indexs, num[6] line, "CVSIZE2D32F_NAME" size, int jc, "CVPOINT3D32F_NAME" center, "CVMEMSTORAGE_NAME" storage)";  
  int top;
  if (((top=lua_gettop(L))!=3)||(!lua_istable(L,1))||(!lua_istable(L,4))) luaL_error(L,f_msg);
  CvSeq *numbers=NULL;
  int  count=checkint(L,2);
  CvPoint3D32f *points=(CvPoint3D32f *)luacv_alloc(sizeof(CvPoint3D32f)*count);
   
  for (int i=0;i<count;i++)
  {
    lua_rawgeti(L,1,i+1); 
    points[i]=*checkCvPoint3D32f(L,top+1);
    lua_pop(L,1);
  }
  
  float line[6];

  for (int i=0;i<6;i++)
  {
    lua_rawgeti(L,4,i+1); 
    line[i]=(float)checknumber(L,top+1);
    lua_pop(L,1);
  }
 
  cvFindHandRegionA(points,count,checkCvSeq(L,3),line,*checkCvSize2D32f(L,5),checkint(L,6),checkCvPoint3D32f(L,7),checkCvMemStorage(L,8),&numbers);
  pushCvSeq(L,numbers);
  return 1;
}

static int luacv_cvCalcImageHomography(lua_State *L)
{
UNIMPL(L);
return 1;
}


static int luacv_cvCalcPGH(lua_State *L)
{
  const char f_msg[]="CalcPGH("CVSEQ_NAME" contour, "CVHISTOGRAM_NAME" hist)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvCalcPGH(checkCvSeq(L,1),checkCvHistogram(L,2));
  return 0;
}

static int luacv_cvFindDominantPoints(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" FindDominantPoints("CVSEQ_NAME" contour, "CVMEMSTORAGE_NAME" storage, int method=CV_DOMINANT_IPAN, num parameter1=0, num parameter2=0, num parameter3=0, num parameter4=0)";
  lua_Number par1=0,par2=0,par3=0,par4=0;
  lua_Integer method=CV_DOMINANT_IPAN;
  switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      method=checkint(L,3);
      break;
    case 4:
      method=checkint(L,3);
      par1=checknumber(L,4);
      break;
    case 5:
      method=checkint(L,3);
      par1=checknumber(L,4);
      par2=checknumber(L,5);
      break;
    case 6:
      method=checkint(L,3);
      par1=checknumber(L,4);
      par2=checknumber(L,5);
      par3=checknumber(L,6);
      break;
    case 7:
      method=checkint(L,3);
      par1=checknumber(L,4);
      par2=checknumber(L,5);
      par3=checknumber(L,6);
      par4=checknumber(L,7);
      break;

    default:
      luaL_error(L,f_msg);
  }
  pushCvSeq(L,cvFindDominantPoints(checkCvSeq(L,1),checkCvMemStorage(L,2),method,par1,par2,par3,par4));
  return 1;
}

static int luacv_cvDeInterlace(lua_State *L)
{
  const char f_msg[]="DeInterlace("CVARR_NAME" frame, "CVARR_NAME" fieldEven, "CVARR_NAME" field0dd)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvDeInterlace(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3));
  return 0;
}

static int luacv_cvCreateContourTree(lua_State *L)
{
  const char f_msg[]=CVCONTOURTREE_NAME" CreateContourTree("CVSEQ_NAME" contour, "CVMEMSTORAGE_NAME" storage, num threshold)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  pushCvContourTree(L,cvCreateContourTree(checkCvSeq(L,1),checkCvMemStorage(L,2),checknumber(L,3)));
  return 1;
}

static int luacv_cvContourFromContourTree(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" ContourFromContourTree("CVCONTOURTREE_NAME" tree, "CVMEMSTORAGE_NAME" storage, "CVTERMCRITERIA_NAME" criteria)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  pushCvSeq(L,cvContourFromContourTree(checkCvContourTree(L,1),checkCvMemStorage(L,2),*checkCvTermCriteria(L,3)));
  return 1;
}

static int luacv_cvMatchContourTrees(lua_State *L)
{
  const char f_msg[]="num MatchContourTrees("CVCONTOURTREE_NAME" tree1, "CVCONTOURTREE_NAME" tree2, int method, num threshold)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  lua_pushnumber(L,cvMatchContourTrees(checkCvContourTree(L,1),checkCvContourTree(L,2),checkint(L,3),checknumber(L,4)));
  return 1;
}
/*
static int luacv_cvCalcContoursCorrespondence(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" CalcContoursCorrespondence("CVSEQ_NAME" contour1, "CVSEQ_NAME" contour2, "CVMEMSTORAGE_NAME" storage)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  pushCvSeq(L,cvCalcContoursCorrespondence(checkCvSeq(L,1),checkCvSeq(L,2),checkCvMemStorage(L,3)));
  return 1;
}

static int luacv_cvMorphContours(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" MorphContours("CVSEQ_NAME" contour1, "CVSEQ_NAME" contour2, "CVSEQ_NAME" corr, num alpha,"CVMEMSTORAGE_NAME" storage)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  pushCvSeq(L,cvMorphContours(checkCvSeq(L,1),checkCvSeq(L,2),checkCvSeq(L,3),checknumber(L,4),checkCvMemStorage(L,5)));
  return 1;
}
*/
static int luacv_cvSnakeImage(lua_State *L)
{//zmena
  const char f_msg[]="num,num,num SnakeImage("IPLIMAGE_NAME" image, "CVPOINT_NAME"[] points, int lenght, num alpha, num beta, num gamma, int coeff_usage, "CVSIZE_NAME" win, "CVTERMCRITERIA_NAME" criteria, int calc_gradient=1)";
  float alpha=checknumber(L,4),beta=checknumber(L,5),gamma=checknumber(L,6);
  lua_Integer len=checkint(L,3),top=lua_gettop(L),calc=1;
  switch(top)
  {
    case 9:
      break;
    case 10:
      calc=checkint(L,10);
      break;
    default:
      luaL_error(L,f_msg);
  }

  CvPoint *points=(CvPoint*)luacv_alloc(sizeof(CvPoint)*len);
  CvPoint **tmp=(CvPoint**)luacv_alloc(sizeof(CvPoint*)*len);


  for (int i=0;i<len;i++)
  {
    lua_rawgeti(L,2,i+1); 
    tmp[i]=checkCvPoint(L,top+1);
    points[i]=*(tmp[i]);
    lua_pop(L,1);
  }

  cvSnakeImage(checkIplImage(L,1),points,len,&alpha,&beta,&gamma,checkint(L,7),*checkCvSize(L,8),*checkCvTermCriteria(L,9),calc);

  for (int i=0;i<len;i++)
   *(tmp[i])=points[i];


  lua_pushnumber(L,alpha);
  lua_pushnumber(L,beta);
  lua_pushnumber(L,gamma);
  luacv_free(&points);
  return 3;
}

static int luacv_cvCreateGLCM(lua_State *L)
{
  const char f_msg[]=CVGLCM_NAME" CreateGLCM("IPLIMAGE_NAME" srcimage, int stepMagnitude, int[] stepDirections=nil, int numStepDirections=0, int optimizationType=CV_GLCM_OPTIMIZATION_NONE)";

  lua_Integer num=0,opt=CV_GLCM_OPTIMIZATION_NONE;
  int *directions=NULL;
  int top;
  switch((top=lua_gettop(L)))
  {
    case 2:
      break;
    case 3:
      break;
    case 4:
      num=checkint(L,4);
      break;
    case 5:
      num=checkint(L,4);
      opt=checkint(L,5);
      break;

    default:
      luaL_error(L,f_msg);
  }

  if (top>2)
    if ((lua_istable(L,3))&&(lua_objlen(L,3)>0))
    {
      int len=lua_objlen(L,3);
      directions=(int *)luacv_alloc(sizeof(int)*len); 
      for (int i=0;i<len;i++)
      {
        lua_rawgeti(L,3,i+1); 
        directions[i]=checkint(L,top+1);
        lua_pop(L,1);
      }
    }

  pushCvGLCM(L,cvCreateGLCM(checkIplImage(L,1),checkint(L,2),directions,num,opt));
  return 1;
}

static int luacv_cvReleaseGLCM(lua_State *L)
{
  const char f_msg[]="ReleaseGLCM("CVGLCM_NAME" glcm)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvGLCM(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cvCreateGLCMDescriptors(lua_State *L)
{
  const char f_msg[]="CreateGLCMDescriptors("CVGLCM_NAME" destGLCM, int descriptorOptimizationType=CV_GLCMDESC_OPTIMIZATION_ALLOWDOUBLENEST)";
  lua_Integer opt=CV_GLCMDESC_OPTIMIZATION_ALLOWDOUBLENEST;
  switch(lua_gettop(L))
  {
    case 1:
      break;
    case 2:
        opt=checkint(L,2);
      break;
    default:
      luaL_error(L,f_msg);
  }

  cvCreateGLCMDescriptors(checkCvGLCM(L,1),opt);
  return 0;
}

static int luacv_cvGetGLCMDescriptor(lua_State *L)
{
  const char f_msg[]="num GetGLCMDescriptor("CVGLCM_NAME" glcm, int step, int descriptor)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetGLCMDescriptor(checkCvGLCM(L,1),checkint(L,2),checkint(L,3)));
  return 1;
}

static int luacv_cvGetGLCMDescriptorStatistics(lua_State *L)
{//zmena
  const char f_msg[]="num,num GetGLCMDescriptorStatistics("CVGLCM_NAME" glcm, int descriptor)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_Number average, standard;

  cvGetGLCMDescriptorStatistics(checkCvGLCM(L,1),checkint(L,2),&average,&standard);
  lua_pushnumber(L,average);
  lua_pushnumber(L,standard);
  return 2;
}

static int luacv_cvCreateGLCMImage(lua_State *L)
{
  const char f_msg[]=IPLIMAGE_NAME" CreateGLCMImage("CVGLCM_NAME" glcm, int step)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  pushIplImage(L,cvCreateGLCMImage(checkCvGLCM(L,1),checkint(L,2)));
  return 1;
}

static int luacv_cvInitFaceTracker(lua_State *L)
{  
  const char f_msg[]=CVFACETRACKER_NAME" InitFaceTracker("CVFACETRACKER_NAME" pFaceTracking, "IPLIMAGE_NAME" imgGray, "CVRECT_NAME"[] pRects, int nRects)";
  int top;
  if (((top=lua_gettop(L))!=4)||(!lua_istable(L,3))) luaL_error(L,f_msg);
  lua_Integer num=checkint(L,4);
  int len=lua_objlen(L,3); 
  CvRect *rects=(CvRect*)luacv_alloc(sizeof(CvRect)*len);
  CvRect **temp=(CvRect**)luacv_alloc(sizeof(CvRect*)*len);

  for (int i=0;i<len;i++)
  {
    lua_rawgeti(L,1,i+1); 
    temp[i]=checkCvRect(L,top+1);
    rects[i]=*(temp[i]);
    lua_pop(L,1);
  }

  pushCvFaceTracker(L,cvInitFaceTracker(checkCvFaceTracker(L,1),checkIplImage(L,2),rects,num));
  for (int i=0;i<len;i++)
    *(temp[i])=rects[i];

  return 1;
}

static int luacv_cvTrackFace(lua_State *L)
{//zmena
  const char f_msg[]="int,num TrackFace("CVFACETRACKER_NAME" pFaceTracking, "IPLIMAGE_NAME" imgGray, "CVRECT_NAME"[] pRects, int nRects, "CVPOINT_NAME" ptRotate)";
  int top;
  if (((top=lua_gettop(L))!=4)||(!lua_istable(L,3))) luaL_error(L,f_msg);
  lua_Integer num=checkint(L,4);
  lua_Number angle;
  int len=lua_objlen(L,3); 
  CvRect *rects=(CvRect*)luacv_alloc(sizeof(CvRect)*len);
  CvRect **temp=(CvRect**)luacv_alloc(sizeof(CvRect*)*len);

  for (int i=0;i<len;i++)
  {
    lua_rawgeti(L,1,i+1); 
    temp[i]=checkCvRect(L,top+1);
    rects[i]=*(temp[i]);
    lua_pop(L,1);
  }

  lua_pushnumber(L,cvTrackFace(checkCvFaceTracker(L,1),checkIplImage(L,2),rects,num,checkCvPoint(L,5),&angle));
  for (int i=0;i<len;i++)
    *(temp[i])=rects[i];
  
  lua_pushnumber(L,angle);

  return 2;
}

static int luacv_cvReleaseFaceTracker(lua_State *L)
{
  const char f_msg[]="ReleaseFaceTracker("CVFACETRACKER_NAME" ppFaceTracker)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvFaceTracker(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cv3dTracker2dTrackedObject(lua_State *L)
{
  const char f_msg[]="3dTracker2dTrackedObject(int id, "CVPOINT2D32F_NAME" p)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  Cv3dTracker2dTrackedObject obj=cv3dTracker2dTrackedObject(checkint(L,1),*checkCvPoint2D32f(L,2));
  pushCv3dTracker2dTrackedObject(L,&obj);
  return 1;
}


static int luacv_cv3dTrackerTrackedObject(lua_State *L)
{
  const char f_msg[]="3dTracker3dTrackedObject(int id, "CVPOINT3D32F_NAME" p)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  Cv3dTrackerTrackedObject obj=cv3dTrackerTrackedObject(checkint(L,1),*checkCvPoint3D32f(L,2));
  pushCv3dTrackerTrackedObject(L,&obj);
  return 1;
}

static int luacv_cv3dTrackerCalibrateCameras(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cv3dTrackerLocateObjects(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvVoronoiDiagramFromContour(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvVoronoiDiagramFromImage(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvReleaseVoronoiStorage(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvLinearContorModelFromVoronoiDiagram(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvReleaseLinearContorModelStorage(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvInitPerspectiveTransform(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvInitStereoRectification(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvMakeScanlines(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvPreWarpImage(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvFindRuns(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvDynamicCorrespondMulti(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvMakeAlphaScanlines(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvMorphEpilinesMulti(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvPostWarpImage(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvDeleteMoire(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvCreateConDensation(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvReleaseConDensation(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvConDensUpdateByTime(lua_State *L)
{
UNIMPL(L);
return 1;
}
static int luacv_cvConDensInitSampleSet(lua_State *L)
{
UNIMPL(L);
return 1;
}
const luaL_Reg legacy[]=
{
  funcReg(SegmentImage),                    funcReg(CalcCovarMatrixEx),
  funcReg(CalcEigenObjects),                funcReg(CalcDecompCoeff),
  funcReg(EigenDecomposite),                funcReg(EigenProjection),
  funcReg(Create2DHMM),                     funcReg(Release2DHMM),
  funcReg(CreateObsInfo),                   funcReg(ReleaseObsInfo),
  funcReg(UniformImgSegm),                  funcReg(InitMixSegm),
  funcReg(EstimateHMMStateParams),          funcReg(EstimateTransProb),
  funcReg(EstimateObsProb),                 funcReg(EViterbi),
  funcReg(MixSegmL2),                       funcReg(CreateHandMask),
  funcReg(FindHandRegionA),                 funcReg(CalcImageHomography),
  funcReg(CalcPGH),                         funcReg(FindDominantPoints),
  /*funcReg(StartFindCliques),                funcReg(FindNextMaximalClique),
  funcReg(EndFindCliques),                  funcReg(BronKerbosch),
  funcReg(SubgraphWeight),                  funcReg(FindCliqueEx),
  funcReg(ComputePerspectiveMap),*/           funcReg(DeInterlace),
  funcReg(CreateContourTree),               funcReg(ContourFromContourTree),
  funcReg(MatchContourTrees),               funcReg(SnakeImage),
  funcReg(CreateGLCM),                      funcReg(ReleaseGLCM),
  funcReg(CreateGLCMDescriptors),           funcReg(GetGLCMDescriptor),
  funcReg(GetGLCMDescriptorStatistics),     funcReg(CreateGLCMImage),
  funcReg(InitFaceTracker),                 funcReg(TrackFace),
  funcReg(ReleaseFaceTracker),              funcReg(3dTrackerCalibrateCameras),
  funcReg(3dTrackerLocateObjects),          funcReg(VoronoiDiagramFromContour),
  funcReg(VoronoiDiagramFromImage),         funcReg(ReleaseVoronoiStorage),
  funcReg(LinearContorModelFromVoronoiDiagram),funcReg(ReleaseLinearContorModelStorage),
  funcReg(InitPerspectiveTransform),        funcReg(InitStereoRectification),
  funcReg(MakeScanlines),                   funcReg(PreWarpImage),
  funcReg(FindRuns),                        funcReg(DynamicCorrespondMulti),
  funcReg(MakeAlphaScanlines),              funcReg(MorphEpilinesMulti),
  funcReg(PostWarpImage),                   funcReg(DeleteMoire),
  funcReg(CreateConDensation),              funcReg(ReleaseConDensation),
  funcReg(ConDensUpdateByTime),             funcReg(ConDensInitSampleSet),
  funcReg(FindHandRegion),/*                  funcReg(CalcContoursCorrespondence),
  funcReg(MorphContours),*/                   funcReg(3dTracker2dTrackedObject),
  funcReg(3dTrackerTrackedObject),
  {NULL,NULL}
};

const luacv_var legacy_object[]=
{
  {CVIMGOBSINFO_NAME,sizeof(CvImgObsInfo)},
  {CVEHMM_NAME,sizeof(CvEHMM)},
  {CVCONTOURTREE_NAME,sizeof(CvContourTree)},
  {NULL,0}
};


const luacv_var legacy_var[]=
{
  varReg(CV_DOMINANT_IPAN),             varReg(CV_CONTOUR_TREES_MATCH_I1),
  varReg(CV_ARRAY),                     varReg(CV_UNDEF_SC_PARAM),
  varReg(CV_IDP_BIRCHFIELD_PARAM1),     varReg(CV_IDP_BIRCHFIELD_PARAM2),
  varReg(CV_IDP_BIRCHFIELD_PARAM3),     varReg(CV_IDP_BIRCHFIELD_PARAM4),
  varReg(CV_IDP_BIRCHFIELD_PARAM5),     varReg(CV_DISPARITY_BIRCHFIELD),    
  varReg(CV_CAMERA_TO_WARP),            varReg(CV_WARP_TO_CAMERA),
  varReg(CV_VALUE),                     varReg(CV_ARRAY),
  varReg(CV_GLCM_OPTIMIZATION_NONE),    varReg(CV_GLCM_OPTIMIZATION_LUT),
  varReg(CV_GLCM_OPTIMIZATION_HISTOGRAM),
  varReg(CV_GLCMDESC_OPTIMIZATION_ALLOWDOUBLENEST),
  varReg(CV_GLCMDESC_OPTIMIZATION_ALLOWTRIPLENEST),
  varReg(CV_GLCMDESC_OPTIMIZATION_HISTOGRAM),
  varReg(CV_GLCMDESC_ENTROPY),          varReg(CV_GLCMDESC_ENERGY),
  varReg(CV_GLCMDESC_HOMOGENITY),       varReg(CV_GLCMDESC_CONTRAST),
  varReg(CV_GLCMDESC_CLUSTERTENDENCY),  varReg(CV_GLCMDESC_CLUSTERSHADE),
  varReg(CV_GLCMDESC_CORRELATION),      varReg(CV_GLCMDESC_CORRELATIONINFO1),
  varReg(CV_GLCMDESC_CORRELATIONINFO2), varReg(CV_GLCMDESC_MAXIMUMPROBABILITY),
  varReg(CV_GLCM_ALL),                  varReg(CV_GLCM_GLCM),
  varReg(CV_GLCM_DESC),                 varReg(CV_NUM_FACE_ELEMENTS),
  {NULL,0}
};
