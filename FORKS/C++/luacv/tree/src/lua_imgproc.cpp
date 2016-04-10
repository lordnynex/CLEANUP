#include "lua_imgproc.h"

static int luacv_cvInpaint(lua_State *L)
{
  const char f_msg[]="Inpaint("CVARR_NAME" src, "CVARR_NAME" inpaint_mask, "CVARR_NAME" dst, num inpaintRange, int flags)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  cvInpaint(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checknumber(L,4),checkint(L,5));
  return 0;
}



static int luacv_cvApproxPoly(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" ApproxPoly(userdata src_seq, int header_size, "CVMEMSTORAGE_NAME" storage, int method, num parameter, int parameter2=0)";
  int param2=0;
  switch(lua_gettop(L))
  {
    case 5:
      break;
    case 6:
      param2=checkint(L,6);
      break;
    default:
      luaL_error(L,f_msg);
  }
  pushCvSeq(L,cvApproxPoly(checkCvSeq(L,1),checkint(L,2),checkCvMemStorage(L,3),checkint(L,4),checknumber(L,5),param2));
  return 1;
}

static int luacv_cvAcc(lua_State *L)
{
  const char f_msg[]="Acc("CVARR_NAME" image, "CVARR_NAME" sum, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      if (!lua_isnil(L,3))
        mask=checkCvArr(L,3);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvAcc(checkCvArr(L,1),checkCvArr(L,2),mask);
  return 0;
}

static int luacv_cvSmooth(lua_State *L)
{
  const char f_msg[]="Smooth("CVARR_NAME" src, "CVARR_NAME" dst, int smoothtype=CV_GAUSIAN, int size1=3, int size2=0, num sigma1=0, num sigma2=0)";
  int type=CV_GAUSSIAN,size1=3,size2=0;
  double sigma1=0,sigma2=0;
  switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      type=checkint(L,3);
      break;
    case 4:
      type=checkint(L,3);
      size1=checkint(L,4);
      break;
    case 5:
      type=checkint(L,3);
      size1=checkint(L,4);
      size2=checkint(L,5);
      break;
    case 6:
      type=checkint(L,3);
      size1=checkint(L,4);
      size2=checkint(L,5);
      sigma1=checknumber(L,6);
      break;
    case 7:
      type=checkint(L,3);
      size1=checkint(L,4);
      size2=checkint(L,5);
      sigma1=checknumber(L,6);
      sigma2=checknumber(L,7);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvSmooth(checkCvArr(L,1),checkCvArr(L,2),type,size1,size2,sigma1,sigma2);
  return 0;
}

static int luacv_cvReleasePyramid(lua_State *L)
{
  const char f_msg[]="ReleasePyramid("CVMAT_NAME" pyramid, int extra_layers)";
  CvMat ***mat=(CvMat ***)checkldata(L,1);
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvReleasePyramid(mat,checkint(L,2));
  return 0;
}

static int luacv_cvPyrSegmentation(lua_State *L)
{
  const char f_msg[]="PyrSegmentation("IPLIMAGE_NAME" src, "IPLIMAGE_NAME" dst, "CVMEMSTORAGE_NAME" storage, "CVSEQ_NAME" comp, int level, num threshold1, num threshold2)";
  if (lua_gettop(L)!=7) luaL_error(L,f_msg);
  CvSeq *comp=checkCvSeq(L,4);
  cvPyrSegmentation(checkIplImage(L,1),checkIplImage(L,2),checkCvMemStorage(L,3),&comp,checkint(L,5),checknumber(L,6),checknumber(L,7));
  return 0;
}
static int luacv_cvWatershed(lua_State *L)
{
  const char f_msg[]="Watershed("CVARR_NAME" image, "CVARR_NAME" markers)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvWatershed(checkCvArr(L,1),checkCvArr(L,2));
  return 0;
}

static int luacv_cvCvtColor(lua_State *L)
{
  const char f_msg[]="CvtColor("CVARR_NAME" src, "CVARR_NAME" dst, int code)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvCvtColor(checkCvArr(L,1),checkCvArr(L,2),checkint(L,3));
  return 0;
}


static int luacv_cvWarpAffine(lua_State *L)
{
  const char f_msg[]="WarpAffine("CVARR_NAME" src, "CVARR_NAME" dst, "CVMAT_NAME" map_matrix, int flags=(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS), "CVSCALAR_NAME" fillval=ScalarAll(0))";
  int flags=(CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS);
  CvScalar s=cvScalarAll(0),*fillval=&s;
  switch(lua_gettop(L))
  {
    case 3:
      break;
    case 4:
      flags=checkint(L,4);
      break;
    case 5:
      flags=checkint(L,4);
      fillval=checkCvScalar(L,5);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvWarpAffine(checkCvArr(L,1),checkCvArr(L,2),checkCvMat(L,3),flags,*fillval);
  return 0;
}

static int luacv_cvGetAffineTransform(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" GetAffineTransform("CVPOINT2D32F_NAME" src,"CVPOINT2D32F_NAME" dst , "CVMAT_NAME" map_matrix)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  pushCvMat(L,cvGetAffineTransform(checkCvPoint2D32f(L,1),checkCvPoint2D32f(L,2),checkCvMat(L,3)));
  return 1;
}

static int luacv_cv2DRotationMatrix(lua_State *L)
{//zmena
  const char f_msg[]=CVMAT_NAME" RotationMatrix("CVPOINT2D32F_NAME" center, num angle, num scale, "CVMAT_NAME" map_matrix)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  pushUserData(L,cv2DRotationMatrix(*checkCvPoint2D32f(L,1),checknumber(L,2),checknumber(L,3),checkCvMat(L,4)));
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
  return 1;
}

static int luacv_cvWarpPerspective(lua_State *L)
{
  const char f_msg[]="WarpPerspective("CVARR_NAME" src, "CVARR_NAME" dst, "CVMAT_NAME" map_matrix, int flags=CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, "CVSCALAR_NAME" fillval=ScalarAll(0))";
  int flags=CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS;
  CvScalar val=cvScalarAll(0),*s=&val;
  switch(lua_gettop(L))
  {
    case 3:
      break;
    case 4:
      flags=checkint(L,4);
      break;
    case 5:
      flags=checkint(L,4);
      s=checkCvScalar(L,5);
      break;
    default:
      luaL_error(L,f_msg);
  }

  cvWarpPerspective(checkCvArr(L,1),checkCvArr(L,2),checkCvMat(L,3),flags,*s);
  return 0;
}


static int luacv_cvReleaseStructuringElement(lua_State *L)
{
  const char f_msg[]="ReleaseStructuringElement("IPLCONVKERNEL_NAME" kernel)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkIplConvKernel(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cvMoments(lua_State *L)
{
  const char f_msg[]="Moments("CVARR_NAME" arr, "CVMOMENTS_NAME" moments, int binary=0)";
  int binary=0;
  switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      binary=checkint(L,3);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvMoments(checkCvArr(L,1),checkCvMoments(L,2),binary);
  return 0;
}

static int luacv_cvGetSpatialMoment(lua_State *L)
{
  const char f_msg[]="num GetSpatialMoment("CVMOMENTS_NAME" moments, int x_order, int y_order)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetSpatialMoment(checkCvMoments(L,1),checkint(L,2),checkint(L,3)));
  return 1;
}

static int luacv_cvGetCentralMoment(lua_State *L)
{
  const char f_msg[]="num GetCentralMoment("CVMOMENTS_NAME" moments, int x_order, int y_order)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetCentralMoment(checkCvMoments(L,1),checkint(L,2),checkint(L,3)));

  return 1;
}
static int luacv_cvGetHuMoments(lua_State *L)
{
  const char f_msg[]="GetHuMoments("CVMOMENTS_NAME" moments,"CVHUMOMENTS_NAME" hu_moments)";
  if ((lua_gettop(L)!=2)) luaL_error(L,f_msg); 
  cvGetHuMoments(checkCvMoments(L,1),checkCvHuMoments(L,2));
  return 0;
}

static int luacv_cvGetRectSubPix(lua_State *L)
{
  const char f_msg[]="GetRectSubPix("CVARR_NAME" src, "CVARR_NAME" dst, "CVPOINT2D32F_NAME" center)";
  if ((lua_gettop(L)!=3)) luaL_error(L,f_msg); 
  cvGetRectSubPix(checkCvArr(L,1),checkCvArr(L,2),*checkCvPoint2D32f(L,3));
  return 0;
}

static int luacv_cvFindContours(lua_State *L)
{//zmena
  const char f_msg[]="int,"CVSEQ_NAME" FindContours("CVARR_NAME" image, "CVMEMSTORAGE_NAME" storage, int header_size="CVCONTOUR_NAME"[\"size\"], int mode=CV_RETR_LISTS, int method=CV_CHAIN_APPROX_SIMPLE, "CVPOINT_NAME" offset=Point(0,0))";
  int header_size=sizeof(CvContour),mode=CV_RETR_LIST,method=CV_CHAIN_APPROX_SIMPLE;
  CvPoint offset=cvPoint(0,0);
  switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      header_size=checkint(L,3);
      break;
    case 4:
      header_size=checkint(L,3);
      mode=checkint(L,4);
      break;
    case 5:
      header_size=checkint(L,3);
      mode=checkint(L,4);
      method=checkint(L,5);
      break;
    case 6:
      header_size=checkint(L,3);
      mode=checkint(L,4);
      method=checkint(L,5);
      offset=*checkCvPoint(L,6);
      break;

    default:
      luaL_error(L,f_msg);
  }
  CvSeq *seq=NULL;
  lua_pushnumber(L,cvFindContours(checkCvArr(L,1),checkCvMemStorage(L,2),&seq,header_size,mode,method,offset));
  pushCvSeq(L,seq);
  return 2;
}
static int luacv_cvStartFindContours(lua_State *L)
{
  const char f_msg[]=CVCONTOURSCANNER_NAME" StartFindContours("CVARR_NAME" image, "CVMEMSTORAGE_NAME" storage, int header_size="CVCONTOUR_NAME"[\"size\"], int mode=CV_RETR_LISTS, int method=CV_CHAIN_APPROX_SIMPLE, "CVPOINT_NAME" offset=Point(0,0))";
  int header_size=sizeof(CvContour),mode=CV_RETR_LIST,method=CV_CHAIN_APPROX_SIMPLE;
  CvPoint offset=cvPoint(0,0);
  switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      header_size=checkint(L,3);
      break;
    case 4:
      header_size=checkint(L,3);
      mode=checkint(L,4);
      break;
    case 5:
      header_size=checkint(L,3);
      mode=checkint(L,4);
      method=checkint(L,5);
      break;
    case 6:
      header_size=checkint(L,3);
      mode=checkint(L,4);
      method=checkint(L,5);
      offset=*checkCvPoint(L,6);
      break;

    default:
      luaL_error(L,f_msg);
  }
  CvContourScanner scan=cvStartFindContours(checkCvArr(L,1),checkCvMemStorage(L,2),header_size,mode,method,offset);
  pushCvContourScanner(L,&scan);
  return 1;
}

static int luacv_cvFindNextContour(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" FindNextContour("CVCONTOURSCANNER_NAME" scanner)";
  if ((lua_gettop(L)!=1)) luaL_error(L,f_msg); 
  pushCvSeq(L,cvFindNextContour(*checkCvContourScanner(L,1)));
  return 1;
}

static int luacv_cvSubstituteContour(lua_State *L)
{
  const char f_msg[]="SubstituteContour("CVCONTOURSCANNER_NAME" scanner, "CVSEQ_NAME" new_contour)";
  if ((lua_gettop(L)!=2)) luaL_error(L,f_msg); 
  cvSubstituteContour(*checkCvContourScanner(L,1),checkCvSeq(L,2));
  return 0;
}
static int luacv_cvEndFindContours(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" EndFindContours("CVCONTOURSCANNER_NAME" scanner)";
  if ((lua_gettop(L)!=1)) luaL_error(L,f_msg); 
  pushCvSeq(L,cvEndFindContours(checkCvContourScanner(L,1)));
  return 1;
}

static int luacv_cvStartReadChainPoints(lua_State *L)
{
  const char f_msg[]="StartReadChainPoints("CVCHAIN_NAME" chain, "CVCHAINPTREADER_NAME" reader)";
  if ((lua_gettop(L)!=2)) luaL_error(L,f_msg); 
  cvStartReadChainPoints(checkCvChain(L,1),checkCvChainPtReader(L,2)); 
  return 0;
}
static int luacv_cvReadChainPoint(lua_State *L)
{
  const char f_msg[]=CVPOINT_NAME" ReadChainPoint("CVCHAINPTREADER_NAME" reader)";
  if ((lua_gettop(L)!=1)) luaL_error(L,f_msg); 
  CvPoint p=cvReadChainPoint(checkCvChainPtReader(L,1));
  pushCvPoint(L,&p);
  return 1;
}

static int luacv_cvCheckContourConvexity(lua_State *L)
{
  const char f_msg[]="int CheckContourConvexity("CVARR_NAME" contour)";
  if ((lua_gettop(L)!=1)) luaL_error(L,f_msg); 
  lua_pushnumber(L,cvCheckContourConvexity(checkCvArr(L,1)));
  return 1;
}

static int luacv_cvMaxRect(lua_State *L)
{
  const char f_msg[]=CVRECT_NAME" MaxRect("CVRECT_NAME" rect1, "CVRECT_NAME" rect2)";
  if ((lua_gettop(L)!=2)) luaL_error(L,f_msg); 
  CvRect r=cvMaxRect(checkCvRect(L,1),checkCvRect(L,2));
  pushCvRect(L,&r);
  return 1;
}
static int luacv_cvBoxPoints(lua_State *L)
{
  const char f_msg[]="BoxPoints("CVBOX2D_NAME" box, "CVPOINT2D32F_NAME"[] pt)";
  if (((lua_gettop(L)!=2))||(!lua_istable(L,2))) luaL_error(L,f_msg); 
  CvPoint2D32f pt[4];
  cvBoxPoints(*checkCvBox2D(L,1),pt);
  for(int i=0;i<4;i++)
  {
   pushCvPoint2D32f(L,&pt[i]); 
   lua_rawseti(L,2,i+1);
  }
  return 0;
}
static int luacv_cvReleaseHist(lua_State *L)
{
  const char f_msg[]="ReleaseHist("CVHISTOGRAM_NAME" hist)";
  if ((lua_gettop(L)!=1)) luaL_error(L,f_msg); 
  checkCvHistogram(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cvClearHist(lua_State *L)
{
  const char f_msg[]="ClearHist("CVHISTOGRAM_NAME" hist)";
  if ((lua_gettop(L)!=1)) luaL_error(L,f_msg); 
  cvClearHist(checkCvHistogram(L,1));
  return 0;
}

static int luacv_cvThreshHist(lua_State *L)
{
  const char f_msg[]="ThreshHist("CVHISTOGRAM_NAME" hist, num factor)";
  if ((lua_gettop(L)!=2)) luaL_error(L,f_msg); 
  cvThreshHist(checkCvHistogram(L,1),checknumber(L,2));
  return 0;
}

static int luacv_cvFilter2D(lua_State *L)
{
  const char f_msg[]="Filter2D("CVARR_NAME" src, "CVARR_NAME" dst, "CVMAT_NAME" kernel, "CVPOINT_NAME" anchor=Point(-1,-1))";
  CvPoint anchor=cvPoint(-1,-1),*an=&anchor;
  switch(lua_gettop(L))
  {
    case 3:
      break;
    case 4:
      an=checkCvPoint(L,4);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvFilter2D(checkCvArr(L,1),checkCvArr(L,2),checkCvMat(L,3),*an);
  return 0;
}

static int luacv_cvSobel(lua_State *L)
{
  const char f_msg[]="Sobel("CVARR_NAME" src, "CVARR_NAME" dst, int xorder, int yorder, int aperature_size=3)";
  int aperature_size=3; 
  switch(lua_gettop(L))
  {
    case 4:
      break;
    case 5:
      aperature_size=checkint(L,5);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvSobel(checkCvArr(L,1),checkCvArr(L,2),checkint(L,3),checkint(L,4),aperature_size);
  return 0;
}


static int luacv_cvLaplace(lua_State *L)
{
  const char f_msg[]="Laplace("CVARR_NAME" src, "CVARR_NAME" dst, int aperature_size=3)";
  int aperature_size=3; 
  switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      aperature_size=checkint(L,3);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvLaplace(checkCvArr(L,1),checkCvArr(L,2),aperature_size);
  return 0;
}


static int luacv_cvDilate(lua_State *L)
{
  const char f_msg[]="Dilate("CVARR_NAME" src, "CVARR_NAME" dst, "IPLCONVKERNEL_NAME" element=nil, int iterations=1)";
  int iterations=1; 
  IplConvKernel *element=NULL;
  switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      element=checkIplConvKernel(L,3); 
      break;
    case 4:
      element=checkIplConvKernel(L,3); 
      iterations=checkint(L,4);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvDilate(checkCvArr(L,1),checkCvArr(L,2),element,iterations);
  return 0;
}


static int luacv_cvErode(lua_State *L)
{
  const char f_msg[]="Erode("CVARR_NAME" src, "CVARR_NAME" dst, "IPLCONVKERNEL_NAME" element=nil, int iterations=1)";
  int iterations=1; 
  IplConvKernel *element=NULL;
  switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      if (!lua_isnil(L,3)) 
        element=checkIplConvKernel(L,3); 
      break;
    case 4:
      if (!lua_isnil(L,3)) 
        element=checkIplConvKernel(L,3); 
      iterations=checkint(L,4);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvErode(checkCvArr(L,1),checkCvArr(L,2),element,iterations);
  return 0;
}

static int luacv_cvConvexHull2(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" ConvexHull2("CVARR_NAME" name, "CVMEMSTORAGE_NAME" hull_storage=nil,  int orientation=cv.CV_CLOCKWISE, int return_points=0)";
  int orientation=CV_CLOCKWISE,return_points=0; 
  CvMemStorage *storage=NULL;
  switch(lua_gettop(L))
  {
    case 1:
      break;
    case 2:
      if (!lua_isnil(L,2)) 
        storage=checkCvMemStorage(L,2); 
      break;
    case 3:
      if (!lua_isnil(L,2)) 
        storage=checkCvMemStorage(L,2); 
      orientation=checkint(L,3);
      break;
    case 4:
      if (!lua_isnil(L,2)) 
        storage=checkCvMemStorage(L,2); 
      orientation=checkint(L,3);
      return_points=checkint(L,4);
      break;
    default:
      luaL_error(L,f_msg);
  }
  pushCvSeq(L,cvConvexHull2(checkCvArr(L,1),storage,orientation,return_points));
  return 1;
}



static int luacv_cvCopyHist(lua_State *L)
{
  const char f_msg[]="CopyHist("CVHISTOGRAM_NAME" src, "CVHISTOGRAM_NAME" dst)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvHistogram *dst=checkCvHistogram(L,2);
  cvCopyHist(checkCvHistogram(L,1),&dst);
  return 0;
}

static int luacv_cvThreshold(lua_State *L)
{
  const char f_msg[]="num Threshold("CVARR_NAME" src, "CVARR_NAME" dst, num threshold, num max_value, int threshold_type)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  lua_pushnumber(L,cvThreshold(checkCvArr(L,1),checkCvArr(L,2),checknumber(L,3),checknumber(L,4),checkint(L,5)));
  return 1;
}

static int luacv_cvHoughLines2(lua_State *L)
{
  const char f_msg[]="HoughLines2("CVARR_NAME" image, userdata line_storage, int method, num rho, num theta, int threshold, num param1=0, param2=0)";
  lua_Number param1=0,param2=0;
  switch(lua_gettop(L))
  {
    case 6:
      break;
    case 7:
      param1=checknumber(L,7);
      break;
    case 8:
      param1=checknumber(L,7);
      param2=checknumber(L,8);
      break;
    default:
      luaL_error(L,f_msg);
  }
  pushCvSeq(L,cvHoughLines2(checkCvArr(L,1),checkldata(L,2),checkint(L,3),checknumber(L,4),checknumber(L,5),checkint(L,6),param1,param2));
  return 1;
}


static int luacv_cvCanny(lua_State *L)
{
  const char f_msg[]="Canny("CVARR_NAME" image, "CVARR_NAME" edges, num threshold1, num threshold2, int aperture_size=3)";
  int size=3;
  switch(lua_gettop(L))
  {
    case 4:
      break;
    case 5:
      size=checkint(L,5);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvCanny(checkCvArr(L,1),checkCvArr(L,2),checknumber(L,3),checknumber(L,4),size);
  return 0;
}

static int luacv_cvReleaseFeatureTree(lua_State *L)
{
  const char f_msg[]="ReleaseFeatureTree("CVFEATURETREE_NAME" tr)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvFeatureTree(L,1);
  luacv_releaseObject(L,1);
  return 0;
}
static int luacv_cvReleaseLSH(lua_State *L)
{
  const char f_msg[]="ReleaseLSH("CVLSH_NAME" lsh)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvLSH(L,1);
  luacv_releaseObject(L,1);

  return 0;
}

static int luacv_cvLSHRemove(lua_State *L)
{
  const char f_msg[]="LSHRemove("CVLSH_NAME" lsh, "CVMAT_NAME" indices)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvLSHRemove(checkCvLSH(L,1),checkCvMat(L,2)); 
  return 0;
}

static int luacv_cvFindCornerSubPix(lua_State *L)
{
  const char f_msg[]="FindCornerSubPix("CVARR_NAME" image, "CVPOINT2D32F_NAME"[] corners, "CVSIZE_NAME" win, "CVSIZE_NAME" zero_none, "CVTERMCRITERIA_NAME" criteria)";
  int count=0;
  if ((!lua_istable(L,2))||(lua_gettop(L)!=5)||(!(count=lua_objlen(L,2)))) luaL_error(L,f_msg);
  CvPoint2D32f *corners=(CvPoint2D32f*)luacv_alloc(sizeof(CvPoint2D32f)*count);
  for(int i=0;i<count;i++)
  {
    lua_rawgeti(L,2,i+1); 
    corners[i]=*checkCvPoint2D32f(L,6);
    lua_pop(L,1);
  }
  cvFindCornerSubPix(checkCvArr(L,1),corners,count,*checkCvSize(L,3),*checkCvSize(L,4),*checkCvTermCriteria(L,5));
  luacv_free(&corners);
  return 0;
}

static int luacv_cvCreateStructuringElementEx(lua_State *L)
{
  const char f_msg[]=IPLCONVKERNEL_NAME" CreateStructuringElementEx(int cols, int rows, int anchor_x, int anchor_y, int shape, int[] values=nil)";
  int *values=NULL;
  switch(lua_gettop(L))
  {
    case 5: 
      break;
    case 6:
      {
        if (lua_isnil(L,6)) break;
        if (!lua_istable(L,6)) luaL_error(L,"Values has to be a numerical table indexed from 1.");
        int len=lua_objlen(L,6);
        values=(int*)luacv_alloc(sizeof(int)*len);
        for(int i=1;i<=len;i++)
        {
          lua_rawgeti(L,6,i);
          values[i-1]=checkint(L,7);
          lua_pop(L,1);
        }
        break;
      }
    default:
      luaL_error(L,f_msg);
  }
  pushIplConvKernel(L,cvCreateStructuringElementEx(checkint(L,1),checkint(L,2),checkint(L,3),checkint(L,4),checkint(L,5),values));
  if (values!=NULL) luacv_free(&values);
  return 1;
}

static int luacv_cvBoundingRect(lua_State *L)
{
  const char f_msg[]=CVRECT_NAME" BoundingRect("CVARR_NAME" points, int update=0)";
   int update=0;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            update=checkint(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvRect r=cvBoundingRect(checkCvArr(L,1),update);
  pushCvRect(L,&r);
  return 1;
}

static int luacv_cvResize(lua_State *L)
{
  const char f_msg[]="Resize("CVARR_NAME" src, "CVARR_NAME" dst, int interpolation=CV_INTER_LINEAR )";
  int inter=CV_INTER_LINEAR;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            inter=checkint(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvResize(checkCvArr(L,1),checkCvArr(L,2),inter);
  return 0;
}

static int luacv_cvSquareAcc(lua_State *L)
{
  const char f_msg[]="SquareAcc("CVARR_NAME" image, "CVARR_NAME" sqsum, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            mask=checkCvArr(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvSquareAcc(checkCvArr(L,1),checkCvArr(L,2),mask); 
  return 0;
}

static int luacv_cvMultiplyAcc(lua_State *L)
{
  const char f_msg[]="MultiplyAcc("CVARR_NAME" image1, "CVARR_NAME" image2, "CVARR_NAME" acc, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvMultiplyAcc(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),mask); 
  return 0;
}

static int luacv_cvRunningAvg(lua_State *L)
{
  const char f_msg[]="RunningAvg("CVARR_NAME" image, "CVARR_NAME" acc, num alpha, "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            mask=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvRunningAvg(checkCvArr(L,1),checkCvArr(L,2),checknumber(L,3),mask); 
  return 0;
}

static int luacv_cvCopyMakeBorder(lua_State *L)
{
  const char f_msg[]="CopyMakeBorder("CVARR_NAME" src, "CVARR_NAME" dst, "CVPOINT_NAME" offset, int bordertyp, "CVSCALAR_NAME" value=ScallarAll(0))";
  CvScalar value=cvScalarAll(0);
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            value=*checkCvScalar(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvCopyMakeBorder(checkCvArr(L,1),checkCvArr(L,2),*checkCvPoint(L,3),checkint(L,4),value);
  return 0;
}

static int luacv_cvIntegral(lua_State *L)
{
  const char f_msg[]="Integral("CVARR_NAME" image, "CVARR_NAME" sum, "CVARR_NAME" sqsum=nil, "CVARR_NAME" tilted_sum=nil)";
  CvArr *sqsum=NULL,*tilted_sum=NULL;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            sqsum=checkCvArr(L,3);
            break;
    case 4:
            sqsum=checkCvArr(L,3);
            tilted_sum=checkCvArr(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvIntegral(checkCvArr(L,1),checkCvArr(L,2),sqsum,tilted_sum);
  return 0;
}

static int luacv_cvPyrDown(lua_State *L)
{
  const char f_msg[]="PyrDown("CVARR_NAME" src, "CVARR_NAME" dst, int filter=CV_GAUSSIAN_5x5)";
  int filter=CV_GAUSSIAN_5x5;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            filter=checkint(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvPyrDown(checkCvArr(L,1),checkCvArr(L,2),filter);
  return 0;
}

static int luacv_cvPyrUp(lua_State *L)
{
  const char f_msg[]="PyrUp("CVARR_NAME" src, "CVARR_NAME" dst, int filter=CV_GAUSSIAN_5x5)";
  int filter=CV_GAUSSIAN_5x5;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            filter=checkint(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvPyrUp(checkCvArr(L,1),checkCvArr(L,2),filter);
  return 0;
}

static int luacv_cvCreatePyramid(lua_State *L)
{
  const char f_msg[]="userdata CreatePyramid("CVARR_NAME" img, int extra_layers, num rate, "CVSIZE_NAME"[] layer_sizes=nil, "CVARR_NAME" bufarr=nil, int calc=0, int filter=CV_GAUSSIAN_5x5)";
  int filter=CV_GAUSSIAN_5x5,calc=1, len=0;
  CvSize **layer_sizes=NULL;
  CvArr *bufarr=NULL;
  if (!lua_istable(L,4)) luaL_error(L,"Argument 4 must be table of "CVSIZE_NAME" objects.");
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            len=lua_objlen(L,4); 
            for(int i=1;i<=len;i++)
            {
              lua_rawgeti(L,4,i); 
              layer_sizes[i-1]=checkCvSize(L,8);
              lua_pop(L,1);
            }
            break;
    case 5:
            len=lua_objlen(L,4); 
            for(int i=1;i<=len;i++)
            {
              lua_rawgeti(L,4,i); 
              layer_sizes[i-1]=checkCvSize(L,8);
              lua_pop(L,1);
            }
            bufarr=checkCvArr(L,5);
            break;
    case 6:
            len=lua_objlen(L,4); 
            for(int i=1;i<=len;i++)
            {
              lua_rawgeti(L,4,i); 
              layer_sizes[i-1]=checkCvSize(L,8);
              lua_pop(L,1);
            }
            bufarr=checkCvArr(L,5);
            calc=checkint(L,6);
            break;
    case 7:
            len=lua_objlen(L,4); 
            for(int i=1;i<=len;i++)
            {
              lua_rawgeti(L,4,i); 
              layer_sizes[i-1]=checkCvSize(L,8);
              lua_pop(L,1);
            }
            bufarr=checkCvArr(L,5);
            calc=checkint(L,6);
            filter=checkint(L,7);
            break;

    default:
            luaL_error(L,f_msg);
  }
  pushUserData(L,(void*)cvCreatePyramid(checkCvArr(L,1),checkint(L,2),checkint(L,3),*layer_sizes,bufarr,calc,filter));
  return 1;
}
static int luacv_cvPyrMeanShiftFiltering(lua_State *L)
{
  const char f_msg[]="PyrMeanShiftFiltering("CVARR_NAME" src, "CVARR_NAME" dst, num sp, num sr, int max_level=1, "CVTERMCRITERIA_NAME" termcrit=TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,5,1))";
  int max_level=1;
  CvTermCriteria termcrit=cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,5,1);
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            max_level=checkint(L,5);
            break;
    case 6:
            max_level=checkint(L,5);
            termcrit=*checkCvTermCriteria(L,6);
            break;

    default:
            luaL_error(L,f_msg);
  }
  cvPyrMeanShiftFiltering(checkCvArr(L,1),checkCvArr(L,2),checknumber(L,3),checknumber(L,4),max_level,termcrit);
  return 0;
}

static int luacv_cvRemap(lua_State *L)
{
  const char f_msg[]="Remap("CVARR_NAME" src, "CVARR_NAME" dst, "CVARR_NAME" mapx, "CVARR_NAME" mapy, int flags=CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, "CVSCALAR_NAME" fillval=ScalarAll(0))";
  int flags=CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS;
  CvScalar fillval=cvScalarAll(0);

  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            flags=checkint(L,5);
            break;
    case 6:
            flags=checkint(L,5);
            fillval=*checkCvScalar(L,6);
            break;

    default:
            luaL_error(L,f_msg);
  }
  cvRemap(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4),flags,fillval);
  return 0;
}

static int luacv_cvConvertMaps(lua_State *L)
{
  const char f_msg[]="ConvertMaps("CVARR_NAME" mapx, "CVARR_NAME" mapy, "CVARR_NAME" mapxy, "CVARR_NAME" mapalpha)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvConvertMaps(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4));

  return 0;
}

static int luacv_cvLogPolar(lua_State *L)
{
  const char f_msg[]="LogPolar("CVARR_NAME" src, "CVARR_NAME" dst, "CVPOINT2D32F_NAME" center, num M, int flags=CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS)";
  int flags=CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS;

  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            flags=checkint(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvLogPolar(checkCvArr(L,1),checkCvArr(L,2),*checkCvPoint2D32f(L,3),checknumber(L,4),flags);

  return 0;
}

static int luacv_cvLinearPolar(lua_State *L)
{
  const char f_msg[]="LinearPolar("CVARR_NAME" src, "CVARR_NAME" dst, "CVPOINT2D32F_NAME" center, num maxRadius, int flags=CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS)";
  int flags=CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS;

  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            flags=checkint(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvLinearPolar(checkCvArr(L,1),checkCvArr(L,2),*checkCvPoint2D32f(L,3),checknumber(L,4),flags);

  return 0;
}

static int luacv_cvUndistort2(lua_State *L)
{
  const char f_msg[]="Undistort2("CVARR_NAME" src, "CVARR_NAME" dst, "CVMAT_NAME" camera_matrix, "CVMAT_NAME" distortion_coeffs, "CVMAT_NAME" new_camera_matrix=nil)";
  CvMat *mat=NULL;
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            mat=checkCvMat(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvUndistort2(checkCvArr(L,1),checkCvArr(L,2),checkCvMat(L,3),checkCvMat(L,4),mat);
  return 0;
}

static int luacv_cvInitUndistortMap(lua_State *L)
{
  const char f_msg[]="InitUndistortMap("CVMAT_NAME" camera_matrix, "CVMAT_NAME" distortion_coeffs, "CVARR_NAME" mapx, "CVARR_NAME" mapy)";
  if(lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvInitUndistortMap(checkCvMat(L,1),checkCvMat(L,2),checkCvArr(L,3),checkCvArr(L,4));
    return 0;
}

static int luacv_cvInitUndistortRectifyMap(lua_State *L)
{
  const char f_msg[]="InitUndistortRectifyMap("CVMAT_NAME" camera_matrix, "CVMAT_NAME" distortion_coeffs, "CVMAT_NAME" R, "CVMAT_NAME" new_camera_matrix, "CVARR_NAME" mapx, "CVARR_NAME" mapy)";
  if(lua_gettop(L)!=6) luaL_error(L,f_msg);
  cvInitUndistortRectifyMap(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),checkCvArr(L,5),checkCvArr(L,6));
  return 0;
}

static int luacv_cvUndistortPoints(lua_State *L)
{
  const char f_msg[]="UndistortPoints("CVMAT_NAME" src, "CVMAT_NAME" dst, "CVMAT_NAME" camera_matrix, "CVMAT_NAME" distortion_coeffs, "CVMAT_NAME" R=nil, "CVMAT_NAME" P=nil)";
  CvMat *R=NULL,*P=NULL;
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            R=checkCvMat(L,5);
            break;
    case 6:
            R=checkCvMat(L,5);
            P=checkCvMat(L,6);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvUndistortPoints(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),R,P);

  return 0;
}

static int luacv_cvMorphologyEx(lua_State *L)
{
  const char f_msg[]="MorphologyEx("CVMAT_NAME" src, "CVMAT_NAME" dst, "CVARR_NAME" temp, "IPLCONVKERNEL_NAME" element, int  operation, int iterations=1)";
  int iterations=1;
  switch(lua_gettop(L))
  {
    case 5:
            break;
    case 6:
            iterations=checkint(L,6);
            break;
    default:
            luaL_error(L,f_msg);
  }

  cvMorphologyEx(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkIplConvKernel(L,4),checkint(L,5),iterations);

  return 0;
}

static int luacv_cvGetNormalizedCentralMoment(lua_State *L)
{
  const char f_msg[]="num GetNormalizedCentralMoment("CVMOMENTS_NAME" moments, int x_order, int y_order)";
  if(lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvGetNormalizedCentralMoment(checkCvMoments(L,1),checkint(L,2),checkint(L,3)));
  return 1;
}

static int luacv_cvSampleLine(lua_State *L)
{//zmena
  const char f_msg[]="int,userdata SampleLine("CVMAT_NAME" image, "CVPOINT_NAME" pt1, "CVPOINT_NAME" pt2, int connectivity=8)";
  int connectivity=8;
  void*buffer=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            connectivity=checkint(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  
  lua_pushnumber(L,cvSampleLine(checkCvMat(L,1),*checkCvPoint(L,2),*checkCvPoint(L,3),buffer,connectivity));
  pushUserData(L,buffer);
  return 2;
}

static int luacv_cvGetQuadrangleSubPix(lua_State *L)
{
  const char f_msg[]="GetQuadrangleSubPix("CVARR_NAME" src, "CVARR_NAME" dst, "CVMAT_NAME" map_matrix)";
  if(lua_gettop(L)!=3) luaL_error(L,f_msg);
  cvGetQuadrangleSubPix(checkCvArr(L,1),checkCvArr(L,2),checkCvMat(L,3)); 
  return 0;
}

static int luacv_cvMatchTemplate(lua_State *L)
{
  const char f_msg[]="MatchTemplate("CVARR_NAME" image, "CVARR_NAME" templ, "CVARR_NAME" result, int method)";
  if(lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvMatchTemplate(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkint(L,4));
  return 0;
}

static int luacv_cvCalcEMD2(lua_State *L)
{
  UNIMPL(L);
  return 1;
}

static int luacv_cvApproxChains(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" ApproxChains("CVSEQ_NAME" src_seq, "CVMEMSTORAGE_NAME" storage, int method=CV_CHAIN_APPROX_SIMPLE, num parameter=0, int minimal_perimeter=0, int recursive=0)";
  int method=CV_CHAIN_APPROX_SIMPLE,minimal_perimeter=0,recursive=0;
  double parameter=0;

  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            method=checkint(L,3);
            break;
    case 4:
            method=checkint(L,3);
            parameter=checknumber(L,4);
            break;
    case 5:
            method=checkint(L,3);
            parameter=checknumber(L,4);
            minimal_perimeter=checkint(L,5);
            break;
    case 6:
            method=checkint(L,3);
            parameter=checknumber(L,4);
            minimal_perimeter=checkint(L,5);
            recursive=checkint(L,6);
            break;

    default:
            luaL_error(L,f_msg);
  }
  pushCvSeq(L,cvApproxChains(checkCvSeq(L,1),checkCvMemStorage(L,2),method,parameter,minimal_perimeter,recursive)); 
  return 1;
}

static int luacv_cvInitSubdivDelaunay2D(lua_State *L)
{
  const char f_msg[]="InitSubdivDelaunay2D("CVSUBDIV2D_NAME" subdiv, "CVRECT_NAME" rect)";
  if(lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvInitSubdivDelaunay2D(checkCvSubdiv2D(L,1),*checkCvRect(L,2));
  return 0;
}

static int luacv_cvCreateSubdiv2D(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2D_NAME" CreateSubdiv2D(int subdiv_type, int header_size, int vtx_size, int quadedge_size, "CVMEMSTORAGE_NAME" storage)";
  if(lua_gettop(L)!=5) luaL_error(L,f_msg);
  pushCvSubdiv2D(L,cvCreateSubdiv2D(checkint(L,1),checkint(L,2),checkint(L,3),checkint(L,4),checkCvMemStorage(L,5)));

  return 1;
}

static int luacv_cvCreateSubdivDelaunay2D(lua_State *L)
{
  const char f_msg[]="CreateSubdivDelaunay2D("CVRECT_NAME" rect, "CVMEMSTORAGE_NAME" storage)";
  if(lua_gettop(L)!=2) luaL_error(L,f_msg);
  pushCvSubdiv2D(L,cvCreateSubdivDelaunay2D(*checkCvRect(L,1),checkCvMemStorage(L,2)));
  return 1;
}

static int luacv_cvSubdivDelaunay2DInsert(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2DPOINT_NAME" SubdivDelaunay2DInsert("CVSUBDIV2D_NAME" subdiv, "CVPOINT2D32F_NAME" pt)";
  if(lua_gettop(L)!=2) luaL_error(L,f_msg);
  pushCvSubdiv2DPoint(L,cvSubdivDelaunay2DInsert(checkCvSubdiv2D(L,1),*checkCvPoint2D32f(L,2)));
  return 1;
}

static int luacv_cvSubdiv2DLocate(lua_State *L)
{//zmena
  const char f_msg[]="int,"CVSUBDIV2DEDGE_NAME","CVSUBDIV2DPOINT_NAME" Subdiv2DLocate("CVSUBDIV2D_NAME" subdiv, "CVPOINT2D32F_NAME" pt)";

  switch(lua_gettop(L))
  {
    case 2:
            break;
    /*case 3:
     
            break;
            */
    default:
            luaL_error(L,f_msg);
  }
  CvSubdiv2DEdge e=0;
  CvSubdiv2DPoint *p=NULL;
  lua_pushnumber(L,cvSubdiv2DLocate(checkCvSubdiv2D(L,1),*checkCvPoint2D32f(L,2),&e,&p));
  pushCvSubdiv2DEdge(L,&e);
  pushCvSubdiv2DPoint(L,p);

  return 3;
}

static int luacv_cvCalcSubdivVoronoi2D(lua_State *L)
{
  const char f_msg[]="CalcSubdivVoronoi2D("CVSUBDIV2D_NAME" subdiv)";
  if(lua_gettop(L)!=1) luaL_error(L,f_msg);
  cvCalcSubdivVoronoi2D(checkCvSubdiv2D(L,1));
  return 0;
}

static int luacv_cvClearSubdivVoronoi2D(lua_State *L)
{
  const char f_msg[]="ClearSubdivVoronoi2D("CVSUBDIV2D_NAME" subdiv)";
  if(lua_gettop(L)!=1) luaL_error(L,f_msg);
  cvClearSubdivVoronoi2D(checkCvSubdiv2D(L,1));
  return 0;
}

static int luacv_cvFindNearestPoint2D(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2DPOINT_NAME" FindNearestPoint2D("CVSUBDIV2D_NAME" subdiv, "CVPOINT2D32F_NAME" pt)";
  if(lua_gettop(L)!=2) luaL_error(L,f_msg);
  pushCvSubdiv2DPoint(L,cvFindNearestPoint2D(checkCvSubdiv2D(L,1),*checkCvPoint2D32f(L,2)));
  return 1;
}

static int luacv_cvSubdiv2DnextEdge(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2DEDGE_NAME" Subdiv2DNextEdge("CVSUBDIV2DEDGE_NAME" edge)";
  if(lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvSubdiv2DEdge edge=cvSubdiv2DNextEdge(*checkCvSubdiv2DEdge(L,1));
  pushCvSubdiv2DEdge(L,&edge);
  return 1;
}

static int luacv_cvSubdiv2DRotateEdge(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2DEDGE_NAME" Subdiv2DRotateEdge("CVSUBDIV2DEDGE_NAME" edge, int rotate)";
  if(lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvSubdiv2DEdge edge=cvSubdiv2DRotateEdge(*checkCvSubdiv2DEdge(L,1),checkint(L,2));
  pushCvSubdiv2DEdge(L,&edge);

  return 1;
}

static int luacv_cvSubdiv2DSymEdge(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2DEDGE_NAME" Subdiv2DSymEdge("CVSUBDIV2DEDGE_NAME" edge)";
  if(lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvSubdiv2DEdge edge=cvSubdiv2DSymEdge(*checkCvSubdiv2DEdge(L,1));
  pushCvSubdiv2DEdge(L,&edge);
  return 1;
}

static int luacv_cvSubdiv2DGetEdge(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2DEDGE_NAME" Subdiv2DGetEdge("CVSUBDIV2DEDGE_NAME" edge, int type)";
  if(lua_gettop(L)!=2) luaL_error(L,f_msg);
  CvSubdiv2DEdge edge=cvSubdiv2DGetEdge(*checkCvSubdiv2DEdge(L,1),(CvNextEdgeType)checkint(L,2));
  pushCvSubdiv2DEdge(L,&edge);
  return 1;
}

static int luacv_cvSubdiv2DEdgeOrg(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2DPOINT_NAME" Subdiv2DEdgeOrg("CVSUBDIV2DEDGE_NAME" edge)";
  if(lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvSubdiv2DPoint(L,cvSubdiv2DEdgeOrg(*checkCvSubdiv2DEdge(L,1)));
  return 1;
}
static int luacv_cvSubdiv2DEdgeDst(lua_State *L)
{
  const char f_msg[]=CVSUBDIV2DPOINT_NAME" Subdiv2DEdgeDst("CVSUBDIV2DEDGE_NAME" edge)";
  if(lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvSubdiv2DPoint(L,cvSubdiv2DEdgeDst(*checkCvSubdiv2DEdge(L,1)));
  return 1;
}
static int luacv_cvTriangleArea(lua_State *L)
{
  const char f_msg[]="num TriangleArea("CVPOINT2D32F_NAME" a,"CVPOINT2D32F_NAME" b,"CVPOINT2D32F_NAME" c)";
  if(lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvTriangleArea(*checkCvPoint2D32f(L,1),*checkCvPoint2D32f(L,2),*checkCvPoint2D32f(L,3)));
  return 1;
}

static int luacv_cvArcLength(lua_State *L)
{
  const char f_msg[]="num ArcLength("CVARR_NAME" contour, "CVSLICE_NAME" slice=CV_WHOLE_SEQ, int is_closed=-1)";
  CvSlice slice=CV_WHOLE_SEQ;
  int is_closed=-1;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            slice=*checkCvSlice(L,2);
            break;
    case 3:
            slice=*checkCvSlice(L,2);
            is_closed=checkint(L,3);
            break;

    default:
            luaL_error(L,f_msg);
  }

  lua_pushnumber(L,cvArcLength(checkCvArr(L,1),slice,is_closed));
  return 1;
}

static int luacv_cvContourPerimeter(lua_State *L)
{
  const char f_msg[]="num ContourPerimetere("CVARR_NAME" contour)";
  if(lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,cvContourPerimeter(checkCvArr(L,1)));
  return 1;
}

static int luacv_cvContourArea(lua_State *L)
{
  const char f_msg[]="num ContourArea("CVARR_NAME" contour, "CVSLICE_NAME" slice=CV_WHOLE_SEQ, int oriented=0)";
  CvSlice slice=CV_WHOLE_SEQ;
  int oriented=0;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            slice=*checkCvSlice(L,2);
            break;
    case 3:
            slice=*checkCvSlice(L,2);
            oriented=checkint(L,3);
            break;

    default:
            luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvContourArea(checkCvArr(L,1),slice,oriented));
  return 1;
}

static int luacv_cvMinAreaRect2(lua_State *L)
{
  const char f_msg[]=CVBOX2D_NAME" MinAreaRect2("CVARR_NAME" point, "CVMEMSTORAGE_NAME" storage=nil)";
  CvMemStorage *storage=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            storage=checkCvMemStorage(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }
  CvBox2D box=cvMinAreaRect2(checkCvArr(L,1),storage);
  pushCvBox2D(L,&box);
  return 1;
}

static int luacv_cvMinEnclosingCircle(lua_State *L)
{//zmena
  const char f_msg[]="int,num MinEnclosingCircle("CVARR_NAME" points, "CVPOINT2D32F_NAME" center)";
  if(lua_gettop(L)!=2) luaL_error(L,f_msg);
  float radius=0; 
  lua_pushnumber(L,cvMinEnclosingCircle(checkCvArr(L,1),checkCvPoint2D32f(L,2),&radius));
  lua_pushnumber(L,radius);
  return 2;
}

static int luacv_cvMatchShapes(lua_State *L)
{
  const char f_msg[]="num MatchShapes(userdata object1, userdata object2, int method, num parameter=0)";
  double parameter=0;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            parameter=checknumber(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvMatchShapes(checkldata(L,1),checkldata(L,2),checkint(L,3),parameter));
  return 1;
}

static int luacv_cvConvexityDefects(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" ConvexityDefects("CVARR_NAME" contour, "CVARR_NAME" convexhull, "CVMEMSTORAGE_NAME" storage=nil)";
  CvMemStorage*storage=NULL;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            storage=checkCvMemStorage(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  pushCvSeq(L,cvConvexityDefects(checkCvArr(L,1),checkCvArr(L,2),storage));
  return 1;
}

static int luacv_cvFitEllipse2(lua_State *L)
{
  const char f_msg[]=CVBOX2D_NAME" FitEllipse2("CVARR_NAME" points)";
  if(lua_gettop(L)!=1) luaL_error(L,f_msg);
  CvBox2D box=cvFitEllipse2(checkCvArr(L,1));
  pushCvBox2D(L,&box);
  return 1;
}

static int luacv_cvPointSeqFromMat(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" PointSeqFromMat(int seq_kind, "CVARR_NAME" mat, "CVCONTOUR_NAME" contour_header, "CVSEQBLOCK_NAME" block)";
  if(lua_gettop(L)!=4) luaL_error(L,f_msg);
  pushCvSeq(L,cvPointSeqFromMat(checkint(L,1),checkCvArr(L,2),checkCvContour(L,3),checkCvSeqBlock(L,4))); 
  return 1;
}

static int luacv_cvPointPolygonTest(lua_State *L)
{
  const char f_msg[]="num PointPolygonTest("CVARR_NAME" contour, "CVPOINT2D32F_NAME" pt, int measure_dist)";
  if(lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvPointPolygonTest(checkCvArr(L,1),*checkCvPoint2D32f(L,2),checkint(L,3)));
  return 1;
}

static int luacv_cvCreateHist(lua_State *L)
{
  const char f_msg[]=CVHISTOGRAM_NAME" CreateHist(int dims, int[] sizes, int type, num[][] ranges=nil,int uniform=1)";
  if (!lua_istable(L,2)) luaL_error(L,"%s\n\tSizes has to be a table of numbers",f_msg);
  int sizes_len=lua_objlen(L,2),dims=checkint(L,1),sizes[CV_MAX_DIM],uniform=1;
  if (sizes_len!=dims) luaL_error(L,"%s\n\tSizes table must have size as dims variable value",f_msg);
  float **ranges=NULL;
  int top=lua_gettop(L);
  for(int i=0;i<sizes_len;i++)
  {
    lua_rawgeti(L,2,i+1); 
    sizes[i]=checkint(L,top+1);
    lua_pop(L,1);
  }
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            if (!lua_istable(L,4)) luaL_error(L,"%s\n\tRanges variable has to be table.",f_msg);
            sizes_len=lua_objlen(L,4);
            ranges=(float**)luacv_alloc(sizeof(float*)*sizes_len); 
            for(int i=0;i<sizes_len;i++)
            {
              lua_rawgeti(L,4,i+1);
              if (!lua_istable(L,top+1)) luaL_error(L,"%s\n\tRanges table must have table elemenst.",f_msg);
              ranges[i]=(float*)luacv_alloc(sizeof(float)*2);
              lua_rawgeti(L,top+1,1);
              ranges[i][0]=(float)checknumber(L,top+2); 
              lua_pop(L,1);
              lua_rawgeti(L,top+1,2);
              ranges[i][1]=(float)checknumber(L,top+2); 
              lua_pop(L,1);
            }
            break;
    case 5:
            if (!lua_istable(L,4)) luaL_error(L,"%s\n\tRanges variable has to be table.",f_msg);
            sizes_len=lua_objlen(L,4);
            ranges=(float**)luacv_alloc(sizeof(float*)*sizes_len); 
            for(int i=0;i<sizes_len;i++)
            {
              lua_rawgeti(L,4,i+1);
              if (!lua_istable(L,top+1)) luaL_error(L,"%s\n\tRanges table must have table elemenst.",f_msg);
              ranges[i]=(float*)luacv_alloc(sizeof(float)*2);
              lua_rawgeti(L,top+1,1);
              ranges[i][0]=(float)checknumber(L,top+2); 
              lua_pop(L,1);
              lua_rawgeti(L,top+1,2);
              ranges[i][1]=(float)checknumber(L,top+2); 
              lua_pop(L,1);
              lua_pop(L,1);
            }
            uniform=checkint(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }
  pushCvHistogram(L,cvCreateHist(dims,sizes,checkint(L,3),ranges,uniform));
  if (ranges!=NULL)
  {
    for(int i=0;i<sizes_len;i++)
      luacv_free(&ranges[i]);
  }
  return 1;
}

static int luacv_cvSetHistBinRanges(lua_State *L)
{
  UNIMPL(L);
  return 1;
}

static int luacv_cvMakeHistHeaderForArray(lua_State *L)
{
  UNIMPL(L);
  return 1;
}
static int luacv_cvGetMinMaxHistValue(lua_State *L)
{//zmena
  const char f_msg[]="float,float,int int GetMinMaxHistValue("CVHISTOGRAM_NAME" hist)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  float min_value=0,max_value=0;
  int min_idx=0,max_idx=0;
  cvGetMinMaxHistValue(checkCvHistogram(L,1),&min_value,&max_value,&min_idx,&max_idx);
  lua_pushnumber(L,min_value);
  lua_pushnumber(L,max_value);
  lua_pushnumber(L,min_idx);
  lua_pushnumber(L,max_idx);
  return 4;
}

static int luacv_cvNormalizeHist(lua_State *L)
{
  const char f_msg[]="NormalizeHist("CVHISTOGRAM_NAME" hist, num factor)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvNormalizeHist(checkCvHistogram(L,1),checknumber(L,2));
  return 0;
}

static int luacv_cvCompareHist(lua_State *L)
{
  const char f_msg[]="num CompareHist("CVHISTOGRAM_NAME" hist1, "CVHISTOGRAM_NAME" hist2, int method)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvCompareHist(checkCvHistogram(L,1),checkCvHistogram(L,2),checkint(L,3)));
  return 1;
}

static int luacv_cvCalcBayesianProb(lua_State *L)
{
  const char f_msg[]="CalcBayesianProb("CVHISTOGRAM_NAME" src, int number, "CVHISTOGRAM_NAME" dst)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvHistogram *h1=checkCvHistogram(L,1),*h2=checkCvHistogram(L,3);
  cvCalcBayesianProb(&h1,checkint(L,2),&h2);
  return 0;
}

static int luacv_cvCalcArrHist(lua_State *L)
{
  const char f_msg[]="CalcArrHist("CVARR_NAME" arr, "CVHISTOGRAM_NAME" hist, int accumulate=0, "CVARR_NAME" mask=NULL)";
  int accumulate=0;
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            accumulate=checkint(L,3);
            break;
    case 4:
            accumulate=checkint(L,3);
            mask=checkCvArr(L,4);
            break;

    default:
            luaL_error(L,f_msg);
  }
  CvArr *arr=checkCvArr(L,1);
  cvCalcArrHist(&arr,checkCvHistogram(L,2),accumulate,mask);
  return 0;
}

static int luacv_cvCalcHist(lua_State *L)
{
  const char f_msg[]="CalcHist("CVARR_NAME" image, "CVHISTOGRAM_NAME" hist, int accumulate=0, "CVARR_NAME" mask=NULL)";
  int accumulate=0;
  CvArr *mask=NULL;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            accumulate=checkint(L,3);
            break;
    case 4:
            accumulate=checkint(L,3);
            mask=checkCvArr(L,4);
            break;

    default:
            luaL_error(L,f_msg);
  }
  IplImage *im=(IplImage*)checkCvArr(L,1);
  cvCalcHist(&im,checkCvHistogram(L,2),accumulate,mask);
  return 0;
}

static int luacv_cvCalcArrBackProject(lua_State *L)
{
  const char f_msg[]="CalcArrBackProject("IPLIMAGE_NAME" image, "CVARR_NAME" dst, "CVHISTOGRAM_NAME" hist)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  CvArr *im=checkCvArr(L,1);
  cvCalcArrBackProject(&im,checkCvArr(L,2),checkCvHistogram(L,3));

  return 0;
}
static int luacv_cvCalcArrBackProjectPatch(lua_State *L)
{
  const char f_msg[]="CalcArrBackProjectPatch("IPLIMAGE_NAME" image, "CVARR_NAME" dst, "CVSIZE_NAME" range, "CVHISTOGRAM_NAME" hist, int method, num factor)";
  if (lua_gettop(L)!=6) luaL_error(L,f_msg);
  CvArr *im=checkCvArr(L,1);
  cvCalcArrBackProjectPatch(&im,checkCvArr(L,2),*checkCvSize(L,3),checkCvHistogram(L,4),checkint(L,5),checknumber(L,6));

  return 0;
}

static int luacv_cvCalcProbDensity(lua_State *L)
{
  const char f_msg[]="CalcProbDensity("CVHISTOGRAM_NAME" hist1,"CVHISTOGRAM_NAME" hist2,"CVHISTOGRAM_NAME" dst_hist, num scale=255)";
  double scale=255;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            scale=checknumber(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }

  cvCalcProbDensity(checkCvHistogram(L,1),checkCvHistogram(L,2),checkCvHistogram(L,3),scale);
  return 0;
}

static int luacv_cvEqualizeHist(lua_State *L)
{
  const char f_msg[]="EqualizeHist("CVARR_NAME" src, "CVARR_NAME" dst)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvEqualizeHist(checkCvArr(L,1),checkCvArr(L,2));
  return 0;
}

static int luacv_cvDistTransform(lua_State *L)
{
  const char f_msg[]="DistTransfomr("CVARR_NAME" src, "CVARR_NAME" dst, int distance_type=CV_DIST_L2, int mask_size=3, num[] mask=nil, "CVARR_NAME" labels=0)";
  int distance_type=CV_DIST_L2,mask_size=3,len;
  float *mask=NULL,m[LUAI_MAXVARS];
  CvArr *labels=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            distance_type=checkint(L,3);
            break;
    case 4:
            distance_type=checkint(L,3);
            mask_size=checkint(L,4);
            break;
    case 5:
            distance_type=checkint(L,3);
            mask_size=checkint(L,4);
            if (lua_istable(L,5))
            {
              mask=m;
              len=lua_objlen(L,5);
              for(int i=0;i<len;i++)
              {
                lua_rawgeti(L,5,i+1);
                mask[i]=(float)checknumber(L,7);
                lua_pop(L,1);
              }
            }
            break;
    case 6:
            distance_type=checkint(L,3);
            mask_size=checkint(L,4);
            if (lua_istable(L,5))
            {
              mask=m;
              len=lua_objlen(L,5);
              for(int i=0;i<len;i++)
              {
                lua_rawgeti(L,5,i+1);
                mask[i]=(float)checknumber(L,7);
                lua_pop(L,1);
              }
            }
            labels=checkCvArr(L,6);
            break;

    default:
            luaL_error(L,f_msg);
  }
  cvDistTransform(checkCvArr(L,1),checkCvArr(L,2),distance_type,mask_size,mask,labels); 
  return 0;
}

static int luacv_cvAdaptiveThreshold(lua_State *L)
{
  const char f_msg[]="AdaptiveThreshold("CVARR_NAME" src, "CVARR_NAME" dst, num max_values, int adaptive_method=CV_ADAPTIVE_THRESH_MEAN_C, int threshold_type=CV_THRESH_BINARY, int block_size=3, num param1=5)";
  int adaptive_method=CV_ADAPTIVE_THRESH_MEAN_C,threshold_type=CV_THRESH_BINARY, block_size=3;
  double param1=5;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            adaptive_method=checkint(L,4);
            break;
    case 5:
            adaptive_method=checkint(L,4);
            threshold_type=checkint(L,5);
            break;
    case 6:
            adaptive_method=checkint(L,4);
            threshold_type=checkint(L,5);
            block_size=checkint(L,6);
            break;
   case 7:
            adaptive_method=checkint(L,4);
            threshold_type=checkint(L,5);
            block_size=checkint(L,6);
            param1=checknumber(L,7);
            break;

    default:
            luaL_error(L,f_msg);
  }

  cvAdaptiveThreshold(checkCvArr(L,1),checkCvArr(L,2),checknumber(L,3),adaptive_method,threshold_type,block_size,param1);
  return 0;
}

static int luacv_cvFloodFill(lua_State *L)
{
  const char f_msg[]="FloodFill("CVARR_NAME" image, "CVPOINT_NAME" seed_point, "CVSCALAR_NAME" new_val, "CVSCALAR_NAME" lo_diff=ScalarAll(0), "CVSCALAR_NAME" up_diff=ScalarAll(0), "CVCONNECTEDCOMP_NAME" comp=nil, int flags=4, "CVARR_NAME" mask=nil)";
  CvScalar lo_diff=cvScalarAll(0),up_diff=cvScalarAll(0);
  CvConnectedComp *comp=NULL;
  int flags=4;
  CvArr *mask=NULL;

  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            lo_diff=*checkCvScalar(L,4);
            break;
    case 5:
            lo_diff=*checkCvScalar(L,4);
            up_diff=*checkCvScalar(L,5);
            break;
    case 6:
            lo_diff=*checkCvScalar(L,4);
            up_diff=*checkCvScalar(L,5);
            comp=checkCvConnectedComp(L,6);
            break;
    case 7:
            lo_diff=*checkCvScalar(L,4);
            up_diff=*checkCvScalar(L,5);
            comp=checkCvConnectedComp(L,6);
            flags=checkint(L,7);
            break;
    case 8:
            lo_diff=*checkCvScalar(L,4);
            up_diff=*checkCvScalar(L,5);
            comp=checkCvConnectedComp(L,6);
            flags=checkint(L,7);
            mask=checkCvArr(L,8);
            break;
    default:
            luaL_error(L,f_msg);
  }

  cvFloodFill(checkCvArr(L,1),*checkCvPoint(L,2),*checkCvScalar(L,3),lo_diff,up_diff,comp,flags,mask);
  return 0;
}

static int luacv_cvPreCornerDetect(lua_State *L)
{
  const char f_msg[]="PreCornerDetect("CVARR_NAME" image, "CVARR_NAME" corners, int aperature_size=3)";
  int aperature_size=3;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            aperature_size=checkint(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvPreCornerDetect(checkCvArr(L,1),checkCvArr(L,2),aperature_size);
  return 0;
}

static int luacv_cvCornerEigenValsAndVecs(lua_State *L)
{
  const char f_msg[]="CorenerEigenValsAndVecs("CVARR_NAME" image, "CVARR_NAME" eigenvv, int block_size, int aperature_size=3)";
  int aperature_size=3;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            aperature_size=checkint(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvCornerEigenValsAndVecs(checkCvArr(L,1),checkCvArr(L,2),checkint(L,3),aperature_size);
  return 0;
}

static int luacv_cvCornerHarris(lua_State *L)
{
  const char f_msg[]="CorenerHarris("CVARR_NAME" image, "CVARR_NAME" harris_responce, int block_size, int aperature_size=3, num k=0.04)";
  int aperature_size=3;
  double k=0.04;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            aperature_size=checkint(L,3);
            break;
    case 5:
            aperature_size=checkint(L,3);
            k=checknumber(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvCornerHarris(checkCvArr(L,1),checkCvArr(L,2),checkint(L,3),aperature_size,k);
  return 0;
}

static int luacv_cvGoodFeaturesToTrack(lua_State *L)
{
  //zmena
  const char f_msg[]="int GoodFeaturesToTrack("CVARR_NAME" image, "CVARR_NAME" eigen_image, "CVARR_NAME" temp_image, "CVPOINT2D32F_NAME"[] corners, int count, num quality_level, num min_distance, "CVMAT_NAME" mask=nil, int block_size=3, int use_harris=0, num k=0.04)";
  const int top=lua_gettop(L);
  if (!lua_istable(L,4)) luaL_error(L,f_msg);
  int block_size=3;
  int use_harris=0;
  CvArr * mask=NULL;
  double k=0.04;
  switch(top)
  {
    case 7:
      break;
    case 8:
      mask=checkCvArr(L,8);
      break;
    case 9:
      mask=checkCvArr(L,8);
      block_size=checkint(L,9);
      break;
    case 10:
      mask=checkCvArr(L,8);
      block_size=checkint(L,9);
      use_harris=checkint(L,10);
      break;
    case 11:
      mask=checkCvArr(L,8);
      block_size=checkint(L,9);
      use_harris=checkint(L,10);
      k=checknumber(L,11);
      break;
    default:
      luaL_error(L,f_msg);
  }

  int count=checkint(L,5);
  CvPoint2D32f *corners=(CvPoint2D32f*)luacv_alloc(sizeof(CvPoint2D32f)*count);

  cvGoodFeaturesToTrack(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),corners,&count,checknumber(L,6),checknumber(L,7),mask,block_size,use_harris,k);
  for (int i=1;i<=count;i++)
  {
    pushCvPoint2D32f(L,&corners[i-1]);
    lua_rawseti(L,4,i); 
  }

  luacv_free(&corners);

  lua_pushnumber(L,count);
  return 1;
}

static int luacv_cvHoughCircles(lua_State *L)
{
  const char f_msg[]=CVSEQ_NAME" HoughCircles("CVARR_NAME" image, userdata circle_storage, int method, num dp, num min_dist, num param1=100, num param2=100, int min_radius=0, int max_radius=0)";
  double param1=100,param2=100;
  int min_radius=0,max_radius=0;
  switch(lua_gettop(L))
  {
    case 5:
            break;
    case 6:
            param1=checknumber(L,6); 
            break;
    case 7:
            param1=checknumber(L,6); 
            param1=checknumber(L,7); 
            break;
    case 8:
            param1=checknumber(L,6); 
            param1=checknumber(L,7); 
            min_radius=checkint(L,8);
            break;
    case 9:
            param1=checknumber(L,6); 
            param1=checknumber(L,7); 
            min_radius=checkint(L,8);
            max_radius=checkint(L,9);
            break;

    default:
            luaL_error(L,f_msg);
  }

  pushCvSeq(L,cvHoughCircles(checkCvArr(L,1),checkldata(L,2),checkint(L,3),checknumber(L,4),checknumber(L,5),param1,param2,min_radius,max_radius));
  return 1;
}

static int luacv_cvFitLine(lua_State *L)
{
  const char f_msg[]="FitLine("CVARR_NAME" points, int dist_type, num param, num reps, num aeps, num[] line)";
  UNIMPL(L);
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  
  return 1;
}
static int luacv_cvCreateKDTree(lua_State *L)
{
  const char f_msg[]=CVFEATURETREE_NAME" CreateKDTree("CVMAT_NAME" desc)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  pushCvFeatureTree(L,cvCreateKDTree(checkCvMat(L,1)));
  return 1;
}

static int luacv_cvCreateSpillTree(lua_State *L)
{
  const char f_msg[]=CVFEATURETREE_NAME" CreateSpillTree("CVMAT_NAME" raw_data, int naive=50, num rho=0.7, num tau=0.1)";
  int naive=50;
  double rho=0.7, tau=0.1;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            naive=checkint(L,2);
            break;
    case 3:
            naive=checkint(L,2);
            rho=checknumber(L,3);
            break;
    case 4:
            naive=checkint(L,2);
            rho=checknumber(L,3);
            tau=checknumber(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }

  pushCvFeatureTree(L,cvCreateSpillTree(checkCvMat(L,1),naive,rho,tau));
  return 1;
}

static int luacv_cvFindFeatures(lua_State *L)
{
  const char f_msg[]="FindFeatures("CVFEATURETREE_NAME" tr, "CVMAT_NAME" query_points, "CVMAT_NAME" indices, "CVMAT_NAME" dist, int k, int emax=20)";
  int emax=20;
  switch(lua_gettop(L))
  {
    case 5:
            break;
    case 6:
            emax=checkint(L,6);
            break;
    default:
            luaL_error(L,f_msg);
  }

  cvFindFeatures(checkCvFeatureTree(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,3),emax);
  return 0;
}

static int luacv_cvFindFeaturesBoxed(lua_State *L)
{
  const char f_msg[]="int FindFeaturesBoxed("CVFEATURETREE_NAME" tr, "CVMAT_NAME" bounds_im, "CVMAT_NAME" bounds_max, "CVMAT_NAME" out_indices)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  lua_pushnumber(L,cvFindFeaturesBoxed(checkCvFeatureTree(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4)));
  return 1;
}

static int luacv_cvCreateLSH(lua_State *L)
{
  const char f_msg[]=CVLSH_NAME" CreateLSH("CVLSHOPERATIONS_NAME" ops, int d, int l=10, int k=10, table type=CV_64FC1, double r=4, "CVRNG_NAME" seed=-1)";
  UNIMPL(L);
  switch(lua_gettop(L))
  {
    case 5:
            break;
    case 6:
            break;
    default:
            luaL_error(L,f_msg);
  }
  return 1;
}

static int luacv_cvCreateMemoryLSH(lua_State *L)
{
  const char f_msg[]=CVLSH_NAME" CreateMemoryLSH(int d, int n, int L=10, int k=10, table=CV_64FC1, num r=4, "CVRNG_NAME" seed=-1)";
  UNIMPL(L);
  switch(lua_gettop(L))
  {
    case 5:
            break;
    case 6:
            break;
    default:
            luaL_error(L,f_msg);
  }

  return 1;
}

static int luacv_cvLSHSize(lua_State *L)
{
  const char f_msg[]="int cvLSHSize("CVLSH_NAME" lsh)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  lua_pushnumber(L,LSHSize(checkCvLSH(L,1)));
  return 1;
}

static int luacv_cvLSHAdd(lua_State *L)
{
  const char f_msg[]="LSHAdd("CVLSH_NAME" lsh, "CVMAT_NAME" data, "CVMAT_NAME" indices=nil)";
  CvMat *indices=NULL;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            indices=checkCvMat(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  cvLSHAdd(checkCvLSH(L,1),checkCvMat(L,2),indices);
  return 0;
}

static int luacv_cvLSHQuery(lua_State *L)
{
  const char f_msg[]="cvLSHQuery("CVLSH_NAME" lsh, "CVMAT_NAME" query_points, "CVMAT_NAME" indices, "CVMAT_NAME" dist, int k, int emax)";
  if (lua_gettop(L)!=6) luaL_error(L,f_msg);
  cvLSHQuery(checkCvLSH(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),checkint(L,5),checkint(L,6));
  return 0;
}


static int luacv_cvConnectedComp(lua_State *L)
{//new function
  const char f_msg[]=CVCONNECTEDCOMP_NAME" ConnectedComp(num area,"CVSCALAR_NAME" value, "CVRECT_NAME" rect, "CVSEQ_NAME" contour=nil)";
  CvSeq *seq=NULL;
  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            seq=checkCvSeq(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }

  CvConnectedComp conn;
  conn.area=checknumber(L,1);
  conn.value=*checkCvScalar(L,2);
  conn.rect=*checkCvRect(L,3);
  conn.contour=seq;
  pushCvConnectedComp(L,&conn);
  return 1;
}





const luaL_Reg imgproc[]=
{
  funcReg(LSHSize),             funcReg(LSHAdd),              funcReg(LSHQuery),
  funcReg(CornerHarris),        funcReg(GoodFeaturesToTrack), funcReg(HoughCircles),
  funcReg(FitLine),             funcReg(CreateKDTree),        funcReg(CreateSpillTree),
  funcReg(FindFeatures),        funcReg(FindFeaturesBoxed),   funcReg(CreateLSH),
  funcReg(CreateMemoryLSH),     funcReg(EqualizeHist),        funcReg(DistTransform),
  funcReg(AdaptiveThreshold),   funcReg(FloodFill),           funcReg(PreCornerDetect),
  funcReg(CornerEigenValsAndVecs),funcReg(NormalizeHist),     funcReg(CompareHist),
  funcReg(CalcBayesianProb),    funcReg(CalcArrHist),         funcReg(CalcHist),
  funcReg(CalcArrBackProject),  funcReg(CalcArrBackProjectPatch),funcReg(CalcProbDensity),
  funcReg(MatchShapes),         funcReg(ConvexityDefects),    funcReg(FitEllipse2),
  funcReg(PointSeqFromMat),     funcReg(PointPolygonTest),    funcReg(CreateHist),
  funcReg(SetHistBinRanges),    funcReg(MakeHistHeaderForArray),funcReg(GetMinMaxHistValue),
  funcReg(Subdiv2DRotateEdge),  funcReg(Subdiv2DSymEdge),     funcReg(Subdiv2DGetEdge),
  funcReg(Subdiv2DEdgeOrg),     funcReg(Subdiv2DEdgeDst),     funcReg(TriangleArea),
  funcReg(ArcLength),           funcReg(ContourPerimeter),    funcReg(ContourArea),
  funcReg(MinAreaRect2),        funcReg(MinEnclosingCircle),  funcReg(SubdivDelaunay2DInsert),
  funcReg(Subdiv2DLocate),      funcReg(CalcSubdivVoronoi2D), funcReg(ClearSubdivVoronoi2D),
  funcReg(FindNearestPoint2D),  funcReg(Subdiv2DnextEdge),    funcReg(SampleLine),
  funcReg(GetQuadrangleSubPix), funcReg(MatchTemplate),       funcReg(CalcEMD2),
  funcReg(ApproxChains),        funcReg(InitSubdivDelaunay2D),funcReg(CreateSubdiv2D),
  funcReg(CreateSubdivDelaunay2D),funcReg(Undistort2),        funcReg(InitUndistortMap),
  funcReg(InitUndistortRectifyMap),funcReg(UndistortPoints),  funcReg(MorphologyEx),
  funcReg(GetNormalizedCentralMoment),funcReg(CreatePyramid), funcReg(PyrMeanShiftFiltering),
  funcReg(Remap),               funcReg(ConvertMaps),         funcReg(LogPolar),
  funcReg(LinearPolar),         funcReg(SquareAcc),           funcReg(MultiplyAcc),
  funcReg(RunningAvg),          funcReg(CopyMakeBorder),      funcReg(Integral),
  funcReg(PyrDown),             funcReg(PyrUp),               funcReg(Resize),
  funcReg(Filter2D),            funcReg(Sobel),               funcReg(Laplace),
  funcReg(Dilate),              funcReg(Erode),               funcReg(ConvexHull2),
  funcReg(BoundingRect),        funcReg(CreateStructuringElementEx),funcReg(FindCornerSubPix),
  funcReg(WarpAffine),          funcReg(GetAffineTransform),  funcReg(WarpPerspective),
  funcReg(ReleaseStructuringElement),funcReg(Moments),        funcReg(GetSpatialMoment),
  funcReg(GetCentralMoment),    funcReg(GetHuMoments),        funcReg(GetRectSubPix),
  funcReg(FindContours),        funcReg(StartFindContours),   funcReg(FindNextContour),
  funcReg(SubstituteContour),   funcReg(EndFindContours),     funcReg(StartReadChainPoints),
  funcReg(ReadChainPoint),      funcReg(CheckContourConvexity),funcReg(MaxRect),
  funcReg(BoxPoints),           funcReg(ReleaseHist),         funcReg(ClearHist),
  funcReg(ThreshHist),          funcReg(CopyHist),            funcReg(Threshold),
  funcReg(HoughLines2),         funcReg(Canny),               funcReg(ReleaseFeatureTree),
  funcReg(ReleaseLSH),          funcReg(LSHRemove),           funcReg(Smooth),
  funcReg(ReleasePyramid),      funcReg(PyrSegmentation),     funcReg(Watershed),
  funcReg(CvtColor),            funcReg(ApproxPoly),          funcReg(Acc),
  funcReg(ConnectedComp),       funcReg(Inpaint),             
  {"RotationMatrix",luacv_cv2DRotationMatrix},  {"CalcBackProject",luacv_cvCalcArrBackProject},  {"CalcBackProjectPatch",luacv_cvCalcArrBackProjectPatch},
  {NULL,NULL}
};

const luacv_var imgproc_object[]=
{
  {CVMOMENTS_NAME,sizeof(CvMoments)},
  {CVHUMOMENTS_NAME,sizeof(CvHuMoments)},
  {CVCONNECTEDCOMP_NAME,sizeof(CvConnectedComp)},
  {CVSUBDIV2D_NAME,sizeof(CvSubdiv2D)},
  {CVQUADEDGE2D_NAME,sizeof(CvQuadEdge2D)},
  {CVSUBDIV2DPOINT_NAME,sizeof(CvSubdiv2DPoint)},
  {CVCONTOURSCANNER_NAME,sizeof(CvContourScanner)},
  {CVSUBDIV2DEDGE_NAME,sizeof(CvSubdiv2DEdge)},
  {NULL,0}
};

const luacv_var imgproc_var[]=
{
  varReg(CV_BLUR_NO_SCALE),         varReg(CV_BLUR),        varReg(CV_GAUSSIAN),
  varReg(CV_MEDIAN),                varReg(CV_BILATERAL),   varReg(CV_INPAINT_NS),
  varReg(CV_INPAINT_TELEA),         varReg(CV_SCHARR),      varReg(CV_MAX_SOBEL_KSIZE),
  varReg(CV_BGR2BGRA),              varReg(CV_RGB2RGBA),    varReg(CV_BGRA2BGR),
  varReg(CV_RGBA2RGB),              varReg(CV_BGR2RGBA),    varReg(CV_RGB2BGRA),
  varReg(CV_RGBA2BGR),              varReg(CV_BGRA2RGB),    varReg(CV_BGR2RGB),
  varReg(CV_RGB2BGR),               varReg(CV_BGRA2RGBA),   varReg(CV_RGBA2BGRA),
  varReg(CV_BGR2GRAY),              varReg(CV_RGB2GRAY),    varReg(CV_GRAY2BGR),
  varReg(CV_GRAY2RGB),              varReg(CV_GRAY2BGRA),   varReg(CV_GRAY2RGBA),
  varReg(CV_BGRA2GRAY),             varReg(CV_RGBA2GRAY),   varReg(CV_BGR2BGR565),
  varReg(CV_RGB2BGR565),            varReg(CV_BGR5652BGR),  varReg(CV_BGR5652RGB),
  varReg(CV_BGRA2BGR565),           varReg(CV_RGBA2BGR565), varReg(CV_BGR5652BGRA),
  varReg(CV_BGR5652RGBA),           varReg(CV_GRAY2BGR565), varReg(CV_BGR5652GRAY),
  varReg(CV_BGR2BGR555),            varReg(CV_RGB2BGR555),  varReg(CV_BGR5552BGR),
  varReg(CV_BGR5552RGB),            varReg(CV_BGRA2BGR555), varReg(CV_RGBA2BGR555),
  varReg(CV_BGR5552BGRA),           varReg(CV_BGR5552RGBA), varReg(CV_GRAY2BGR555),
  varReg(CV_BGR5552GRAY),           varReg(CV_BGR2XYZ),     varReg(CV_RGB2XYZ),
  varReg(CV_XYZ2BGR),               varReg(CV_XYZ2RGB),     varReg(CV_BGR2YCrCb),
  varReg(CV_RGB2YCrCb),             varReg(CV_YCrCb2BGR),   varReg(CV_YCrCb2RGB),
  varReg(CV_BGR2HSV),               varReg(CV_RGB2HSV),     varReg(CV_BGR2Lab),
  varReg(CV_RGB2Lab),               varReg(CV_BayerBG2BGR), varReg(CV_BayerGB2BGR),
  varReg(CV_BayerRG2BGR),           varReg(CV_BayerGR2BGR), varReg(CV_BayerBG2RGB),
  varReg(CV_BayerGB2RGB),           varReg(CV_BayerRG2RGB), varReg(CV_BayerGR2RGB),
  varReg(CV_BGR2Luv),               varReg(CV_RGB2Luv),     varReg(CV_BGR2HLS),
  varReg(CV_RGB2HLS),               varReg(CV_HSV2BGR),     varReg(CV_HSV2RGB),
  varReg(CV_Lab2BGR),               varReg(CV_Lab2RGB),     varReg(CV_Luv2BGR),
  varReg(CV_Luv2RGB),               varReg(CV_HLS2BGR),     varReg(CV_HLS2RGB),
  varReg(CV_COLORCVT_MAX),          varReg(CV_INTER_NN),    varReg(CV_INTER_LINEAR),
  varReg(CV_INTER_CUBIC),           varReg(CV_INTER_AREA),  varReg(CV_WARP_FILL_OUTLIERS),
  varReg(CV_WARP_INVERSE_MAP),      varReg(CV_SHAPE_RECT),  varReg(CV_SHAPE_CROSS),
  varReg(CV_SHAPE_ELLIPSE),         varReg(CV_SHAPE_CUSTOM),varReg(CV_MOP_OPEN),
  varReg(CV_MOP_CLOSE),             varReg(CV_MOP_GRADIENT),varReg(CV_MOP_TOPHAT),
  varReg(CV_MOP_BLACKHAT),          varReg(CV_TM_SQDIFF),   varReg(CV_TM_SQDIFF_NORMED),
  varReg(CV_TM_CCORR),              varReg(CV_TM_CCORR_NORMED),varReg(CV_TM_CCOEFF),
  varReg(CV_TM_CCOEFF_NORMED),      varReg(CV_POLY_APPROX_DP),varReg(CV_CONTOURS_MATCH_I1),
  varReg(CV_CONTOURS_MATCH_I2),     varReg(CV_CONTOURS_MATCH_I3),varReg(CV_CLOCKWISE),
  varReg(CV_COUNTER_CLOCKWISE),     varReg(CV_COMP_CORREL), varReg(CV_COMP_CHISQR),
  varReg(CV_COMP_INTERSECT),        varReg(CV_COMP_BHATTACHARYYA),varReg(CV_DIST_MASK_3),
  varReg(CV_DIST_MASK_5),           varReg(CV_DIST_MASK_PRECISE),varReg(CV_THRESH_BINARY),
  varReg(CV_THRESH_BINARY_INV),     varReg(CV_THRESH_TRUNC),varReg(CV_THRESH_TOZERO),
  varReg(CV_THRESH_TOZERO_INV),     varReg(CV_THRESH_MASK), varReg(CV_THRESH_OTSU),
  varReg(CV_ADAPTIVE_THRESH_MEAN_C),varReg(CV_ADAPTIVE_THRESH_GAUSSIAN_C),varReg(CV_FLOODFILL_FIXED_RANGE),
  varReg(CV_FLOODFILL_MASK_ONLY),   varReg(CV_CANNY_L2_GRADIENT),varReg(CV_HOUGH_STANDARD),
  varReg(CV_HOUGH_PROBABILISTIC),   varReg(CV_HOUGH_MULTI_SCALE),varReg(CV_HOUGH_GRADIENT),
  varReg(CV_CHAIN_APPROX_SIMPLE),   varReg(CV_CHAIN_APPROX_NONE),varReg(CV_RETR_LIST),
  varReg(CV_RETR_TREE),             varReg(CV_POLY_APPROX_DP),varReg(CV_DIST_L2),
  varReg(CV_DIST_L1),               varReg(CV_NEXT_AROUND_LEFT),varReg(CV_RETR_CCOMP),
  {NULL,0}
};
