#include "lua_calib3d.h"

static int luacv_cvDrawChessboardCorners(lua_State *L)
{
  const char f_msg[]="DrawChessboardCorners("CVARR_NAME" image, "CVSIZE_NAME" pattern_size, "CVPOINT2D32F_NAME"[] corners, int count, int pattern_was_found)";
  const int len=lua_objlen(L,3),top=lua_gettop(L);
  if ((!lua_istable(L,3))||(!len)||(top!=5)) luaL_error(L,f_msg);
  lua_Integer count=checkint(L,4);
  CvPoint2D32f *corners=(CvPoint2D32f*)luacv_alloc(sizeof(CvPoint2D32f)*count);

  for (int i=1;i<=count;i++)
  {
    lua_rawgeti(L,3,i);
    corners[i-1]=*checkCvPoint2D32f(L,top+1);
    lua_pop(L,1);
  }
  cvDrawChessboardCorners(checkldata(L,1),*checkCvSize(L,2),corners,count,checkint(L,5));
  luacv_free(&corners);
  return 0;
}

static int luacv_cvFindChessboardCorners(lua_State *L)
{
  const char f_msg[]="int FindChessboardCorners("CVARR_NAME" image, "CVSIZE_NAME" pattern_size, "CVPOINT2D32F_NAME"[] corners, int flags=(CV_CALIB_CB_ADAPTIVE_THRESH+CV_CALIB_CB_NORMALIZE_IMAGE))";
  const int top=lua_gettop(L);
  if (!lua_istable(L,3)) luaL_error(L,f_msg);
  int cc,flags=(CV_CALIB_CB_ADAPTIVE_THRESH+CV_CALIB_CB_NORMALIZE_IMAGE);
  switch(top)
  {
    case 3:
      break;
    case 4:
      flags=checkint(L,4);
      break;
    default:
      luaL_error(L,f_msg);
  }
  CvPoint2D32f corners[LUAI_MAXVARS];

  lua_pushnumber(L,cvFindChessboardCorners(checkldata(L,1),*checkCvSize(L,2),corners,&cc,flags));
  for (int i=1;i<=cc;i++)
  {
    pushCvPoint2D32f(L,&corners[i-1]);
    lua_rawseti(L,3,i); 
  }
  lua_pushnumber(L,cc);
  return 2;
}



static int luacv_cvCreatePOSITObject(lua_State *L)
{
  const char f_msg[]=CVPOSITOBJECT_NAME" CreatePOSITObject("CVPOINT2D32F_NAME"[] points, int point_count)";
  int top=lua_gettop(L);
  if ((!lua_istable(L,3))||(top!=2)) luaL_error(L,f_msg);
  lua_Integer count=checkint(L,2);
  CvPoint3D32f *points=(CvPoint3D32f*)luacv_alloc(sizeof(CvPoint3D32f)*count);
  for (int i=1;i<=count;i++)
  {
    lua_rawgeti(L,1,i);
    points[i-1]=*checkCvPoint3D32f(L,top+1);
    lua_pop(L,1);
  }

  pushCvPOSITObject(L,cvCreatePOSITObject(points,count)); 
  luacv_free(&points);
  return 1;
}
static int luacv_cvPOSIT(lua_State *L)
{
  //const char f_msg[]="POSIT("CVPOSITOBJECT_NAME" posit_obj, "CVPOINT2D32F_NAME"[] image_points, num focal_lenght, "CVTERMCRITERIA_NAME" criteria, )";
  UNIMPL(L);
  return 0;
}
static int luacv_cvReleasePOSITObject(lua_State *L)
{
  const char f_msg[]="ReleasePOSITObject("CVPOSITOBJECT_NAME" posit_obj)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvPOSITObject(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cvRANSACUpdateNumIters(lua_State *L)
{
  const char f_msg[]="num RANSACUpdateNumIters(num p, num err_prob, int model_points, int max_iter)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvRANSACUpdateNumIters(checknumber(L,1),checknumber(L,2),checkint(L,3),checkint(L,4));
  return 1;
}
static int luacv_cvConvertPointsHomogeneous(lua_State *L)
{
  const char f_msg[]="ConvertPointsHomogeneous("CVMAT_NAME" src, "CVMAT_NAME" dst)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvConvertPointsHomogeneous(checkCvMat(L,1),checkCvMat(L,2));
  return 0;
}
static int luacv_cvFindFundamentalMat(lua_State *L)
{
  const char f_msg[]="num FindFundamentalMat("CVMAT_NAME" points1, "CVMAT_NAME" points2, "CVMAT_NAME" fundamental_matrix, int method=CV_FM_RANSAC, num param1=3, num param2=0.99, "CVMAT_NAME" status=nil)";
  lua_Integer method=CV_FM_RANSAC;
  lua_Number param1=3,param2=0.99;
  CvMat *status=NULL;
  switch(lua_gettop(L))
  {
    case 3:
      break;
    case 4:
      method=checkint(L,4);
      break;
    case 5:
      method=checkint(L,4);
      param1=checknumber(L,5);
      break;
    case 6:
      method=checkint(L,4);
      param1=checknumber(L,5);
      param2=checknumber(L,6);
      break;
    case 7:
      method=checkint(L,4);
      param1=checknumber(L,5);
      param2=checknumber(L,6);
      status=checkCvMat(L,7);
      break;
    default:
      luaL_error(L,f_msg);
  }

  lua_pushnumber(L,cvFindFundamentalMat(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),method,param1,param2,status));
  return 1;
}

static int luacv_cvComputeCorrespondEpilines(lua_State *L)
{
  const char f_msg[]="ComputeCorrespondEpilines("CVMAT_NAME" points, int which_image, "CVMAT_NAME" fundamental_matrix, "CVMAT_NAME" correspondent_lines)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
 cvComputeCorrespondEpilines(checkCvMat(L,1),checkint(L,2),checkCvMat(L,3),checkCvMat(L,4));
  return 0;
}

static int luacv_cvTriangulatePoints(lua_State *L)
{
  const char f_msg[]="TriangulatePoints("CVMAT_NAME" projMatr1, "CVMAT_NAME" projMatr2, "CVMAT_NAME" projPoints1, "CVMAT_NAME" projPoints2, "CVMAT_NAME" points4D)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
 cvTriangulatePoints(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),checkCvMat(L,5));
  return 0;
}
static int luacv_cvCorrectMatches(lua_State *L)
{
  const char f_msg[]="CorrectMatches("CVMAT_NAME" F, "CVMAT_NAME" points1, "CVMAT_NAME" points2, "CVMAT_NAME" new_points1, "CVMAT_NAME" new_points2)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
 cvCorrectMatches(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),checkCvMat(L,5));
  return 0;
}

static int luacv_cvGetOptimalNewCameraMatrix(lua_State *L)
{
  const char f_msg[]="GetOptimalNewCameraMatrix("CVMAT_NAME" camera_matrix, "CVMAT_NAME" dist_coeffs, "CVSIZE_NAME" image_size, num alpha, "CVMAT_NAME" new_camera_matrix, "CVSIZE_NAME" new_image_size=cv.Size(0,0), "CVRECT_NAME" valid_pixel_roi=nil)";
  CvSize size=cvSize(0,0);
  CvRect *rect=NULL;
  switch(lua_gettop(L))
  {
    case 5:
      break;
    case 6:
      size=*checkCvSize(L,6);
      break;
    case 7:
      size=*checkCvSize(L,6);
      rect=checkCvRect(L,7);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvGetOptimalNewCameraMatrix(checkCvMat(L,1),checkCvMat(L,2),*checkCvSize(L,3),checknumber(L,4),checkCvMat(L,5),size,rect);
  return 0;
}

static int luacv_cvRodrigues2(lua_State *L)
{
  const char f_msg[]="num Rodrigues2("CVMAT_NAME" src, "CVMAT_NAME" dst, "CVMAT_NAME" jacobian=nil)";
  CvMat *jacob=NULL;
  switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      jacob=checkCvMat(L,3);
      break;
    default:
      luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvRodrigues2(checkCvMat(L,1),checkCvMat(L,2),jacob));
  return 1;
}
static int luacv_cvFindHomography(lua_State *L)
{
  const char f_msg[]="num FindHomography("CVMAT_NAME" src_points, "CVMAT_NAME" dst_points, "CVMAT_NAME" homography, int method=0, num ransacReprojThreshold=3, "CVMAT_NAME" mask=nil)";
  CvMat *mask=NULL;
  lua_Integer method=0;
  lua_Number ransac=3;
  switch(lua_gettop(L))
  {
    case 3:
      break;
    case 4:
      method=checkint(L,4);
      break;
    case 5:
      method=checkint(L,4);
      ransac=checknumber(L,5);
      break;
    case 6:
      method=checkint(L,4);
      ransac=checknumber(L,5);
      mask=checkCvMat(L,6);
      break;

    default:
      luaL_error(L,f_msg);
  }
  lua_pushnumber(L,cvFindHomography(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),method,ransac,mask));
  return 1;
}
static int luacv_cvRQDecomp3x3(lua_State *L)
{
 const char f_msg[]="RQDecomp3x3("CVMAT_NAME" matrixM, "CVMAT_NAME" matrixR, "CVMAT_NAME" matrixQ, "CVMAT_NAME" matrixQx=nil, "CVMAT_NAME" matrixQy=nil, "CVMAT_NAME" matrixQz=nil, "CVPOINT3D64F_NAME" eurlerAngles=nil)";
  CvMat *Qx=NULL,*Qy=NULL,*Qz=NULL;
  CvPoint3D64f *ang=NULL;
 switch(lua_gettop(L))
  {
    case 3:
      break;
    case 4:
      Qx=checkCvMat(L,4);
      break;
    case 5:
      Qx=checkCvMat(L,4);
      Qy=checkCvMat(L,5);
      break;
    case 6:
      Qx=checkCvMat(L,4);
      Qy=checkCvMat(L,5);
      Qz=checkCvMat(L,6);
      break;
    case 7:
      Qx=checkCvMat(L,4);
      Qy=checkCvMat(L,5);
      Qz=checkCvMat(L,6);
      ang=checkCvPoint3D64f(L,7);
      break;
    default:
      luaL_error(L,f_msg);
  }

  cvRQDecomp3x3(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),Qx,Qy,Qz,ang);
  return 0;
}

static int luacv_cvDecomposeProjectionMatrix(lua_State *L)
{
 const char f_msg[]="DecomposeProjectionMatrix("CVMAT_NAME" projMatr, "CVMAT_NAME" calibMatr, "CVMAT_NAME" rotMatr, "CVMAT_NAME" posVect, "CVMAT_NAME" rotMatrX=nil, "CVMAT_NAME" rotMatrY=nil, "CVMAT_NAME" rotMatrZ=nil, "CVPOINT3D64F_NAME" eurlerAngles=nil)";
  CvMat *Qx=NULL,*Qy=NULL,*Qz=NULL;
  CvPoint3D64f *ang=NULL;
 switch(lua_gettop(L))
  {
    case 4:
      break;
    case 5:
      Qx=checkCvMat(L,5);
      break;
    case 6:
      Qx=checkCvMat(L,5);
      Qy=checkCvMat(L,6);
      break;
    case 7:
      Qx=checkCvMat(L,5);
      Qy=checkCvMat(L,6);
      Qz=checkCvMat(L,7);
      break;
    case 8:
      Qx=checkCvMat(L,5);
      Qy=checkCvMat(L,6);
      Qz=checkCvMat(L,7);
      ang=checkCvPoint3D64f(L,8);
      break;
    default:
      luaL_error(L,f_msg);
  }

  cvDecomposeProjectionMatrix(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),Qx,Qy,Qz,ang);
  return 0;
}

static int luacv_cvCalcMatMulDeriv(lua_State *L)
{
  const char f_msg[]="CalcMatMulDeriv("CVMAT_NAME" A, "CVMAT_NAME" B, "CVMAT_NAME" dAbda, "CVMAT_NAME" daBdb)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
 cvCalcMatMulDeriv(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4));
  return 0;
}

static int luacv_cvComposeRT(lua_State *L)
{
const char f_msg[]="ComposeRT("CVMAT_NAME" _rvec1, "CVMAT_NAME" _tvec1, "CVMAT_NAME" _rvec2, "CVMAT_NAME" _tvec2, "CVMAT_NAME" _rvec3, "CVMAT_NAME" _tvec3, "CVMAT_NAME" dr3dr1=nil, "CVMAT_NAME" dr3dt1=nil, "CVMAT_NAME" dr3dr2=nil, "CVMAT_NAME" dr3dt2=nil, "CVMAT_NAME" dt3dr1=nil, "CVMAT_NAME" dt3dt1=nil, "CVMAT_NAME" dt3dr2=nil, "CVMAT_NAME" dt3dt2=ni)";
  CvMat *dr3dr1=NULL,*dr3dt1=NULL,*dr3dr2=NULL,*dr3dt2=NULL,*dt3dr1=NULL,*dt3dt1=NULL,*dt3dr2=NULL,*dt3dt2=NULL;
 switch(lua_gettop(L))
  {
    case 6:
      break;
    case 7:
      dr3dr1=checkCvMat(L,7);
      break;
    case 8:
      dr3dr1=checkCvMat(L,7);
      dr3dt1=checkCvMat(L,8);
      break;
    case 9:
      dr3dr1=checkCvMat(L,7);
      dr3dt1=checkCvMat(L,8);
      dr3dr2=checkCvMat(L,9);
      break;
    case 10:
      dr3dr1=checkCvMat(L,7);
      dr3dt1=checkCvMat(L,8);
      dr3dr2=checkCvMat(L,9);
      dr3dt2=checkCvMat(L,10);
      break;
    case 11:
      dr3dr1=checkCvMat(L,7);
      dr3dt1=checkCvMat(L,8);
      dr3dr2=checkCvMat(L,9);
      dr3dt2=checkCvMat(L,10);
      dt3dr1=checkCvMat(L,11);
      break;
    case 12:
      dr3dr1=checkCvMat(L,7);
      dr3dt1=checkCvMat(L,8);
      dr3dr2=checkCvMat(L,9);
      dr3dt2=checkCvMat(L,10);
      dt3dr1=checkCvMat(L,11);
      dt3dt1=checkCvMat(L,12);
    case 13:
      dr3dr1=checkCvMat(L,7);
      dr3dt1=checkCvMat(L,8);
      dr3dr2=checkCvMat(L,9);
      dr3dt2=checkCvMat(L,10);
      dt3dr1=checkCvMat(L,11);
      dt3dt1=checkCvMat(L,12);
      dt3dr2=checkCvMat(L,13);
    case 14:
      dr3dr1=checkCvMat(L,7);
      dr3dt1=checkCvMat(L,8);
      dr3dr2=checkCvMat(L,9);
      dr3dt2=checkCvMat(L,10);
      dt3dr1=checkCvMat(L,11);
      dt3dt1=checkCvMat(L,12);
      dt3dr2=checkCvMat(L,13);
      dt3dt2=checkCvMat(L,14);
    default:
      luaL_error(L,f_msg);
  }

  cvComposeRT(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),checkCvMat(L,5),checkCvMat(L,6),dr3dr1,dr3dt1,dr3dr2,dr3dt2,dt3dr1,dt3dt1,dt3dr2,dt3dt2);
  return 0;
}
static int luacv_cvProjectPoints2(lua_State *L)
{
  const char f_msg[]="ProjectPoints2("CVMAT_NAME" object_points, "CVMAT_NAME" rotation_vector, "CVMAT_NAME" translation_vector, "CVMAT_NAME" camera_matrix, "CVMAT_NAME" distortoin_coeffs, "CVMAT_NAME" image_points, "CVMAT_NAME" dpdrot=nil, "CVMAT_NAME" dpdt=nil, "CVMAT_NAME" dpdf=nil, "CVMAT_NAME" dpdc=nil, "CVMAT_NAME" dpddist=nil, num aspect_ratio=0)";
  CvMat *dpdrot=NULL,*dpdt=NULL,*dpdf=NULL,*dpdc=NULL,*dpddist=NULL;
  lua_Number ratio=0;
 switch(lua_gettop(L))
  {
    case 6:
      break;
    case 7:
      dpdrot=checkCvMat(L,7);
      break;
    case 8:
      dpdrot=checkCvMat(L,7);
      dpdt=checkCvMat(L,8);
      break;
    case 9:
      dpdrot=checkCvMat(L,7);
      dpdt=checkCvMat(L,8);
      dpdf=checkCvMat(L,9);
      break;
    case 10:
      dpdrot=checkCvMat(L,7);
      dpdt=checkCvMat(L,8);
      dpdf=checkCvMat(L,9);
      dpdc=checkCvMat(L,10);
      break;
    case 11:
      dpdrot=checkCvMat(L,7);
      dpdt=checkCvMat(L,8);
      dpdf=checkCvMat(L,9);
      dpdc=checkCvMat(L,10);
      dpddist=checkCvMat(L,11);
      break;
    case 12:
      dpdrot=checkCvMat(L,7);
      dpdt=checkCvMat(L,8);
      dpdf=checkCvMat(L,9);
      dpdc=checkCvMat(L,10);
      dpddist=checkCvMat(L,11);
      ratio=checknumber(L,12);
      break;
    default:
      luaL_error(L,f_msg);
  }

  cvProjectPoints2(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),checkCvMat(L,5),checkCvMat(L,6),dpdrot,dpdt,dpdf,dpdc,dpddist,ratio);
  return 0;
}

static int luacv_cvFindExtrinsicCameraParams2(lua_State *L)
{
  const char f_msg[]="FindExtrinsicCameraParams2("CVMAT_NAME" object_points, "CVMAT_NAME" image_points, "CVMAT_NAME" camera_matrix, "CVMAT_NAME" distortoin_coeffs, "CVMAT_NAME" rotation_vector, "CVMAT_NAME" translation_vector, num use_extrinsic_guess=0)";
  lua_Number use=0;
 switch(lua_gettop(L))
  {
    case 6:
      break;
    case 7:
      use=checknumber(L,7);
      break;
    default:
      luaL_error(L,f_msg);
  }

  cvFindExtrinsicCameraParams2(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),checkCvMat(L,5),checkCvMat(L,6),use);
  return 0;
}

static int luacv_cvInitIntrinsicParams2D(lua_State *L)
{
  const char f_msg[]="InitIntrinsicParams2D("CVMAT_NAME" object_points, "CVMAT_NAME" image_points, "CVMAT_NAME" npoints, "CVSIZE_NAME" image_size, "CVMAT_NAME" camera_matrix, num aspect_ratio=0)";
  lua_Number ratio=0;
 switch(lua_gettop(L))
  {
    case 5:
      break;
    case 6:
      ratio=checknumber(L,6);
      break;
    default:
      luaL_error(L,f_msg);
  }

  cvInitIntrinsicParams2D(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),*checkCvSize(L,4),checkCvMat(L,5),ratio);
  return 0;
}

static int luacv_cvCheckChessboard(lua_State *L)
{
 const char f_msg[]="CheckChessboard("IPLIMAGE_NAME" src, "CVSIZE_NAME" size)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  lua_pushnumber(L,cvCheckChessboard(checkIplImage(L,1),*checkCvSize(L,2)));
  return 1;
}

static int luacv_cvCalibrateCamera2(lua_State *L)
{
  const char f_msg[]="num CalibrateCamera2("CVMAT_NAME" object_points, "CVMAT_NAME" image_points, "CVMAT_NAME" point_counts, "CVSIZE_NAME" image_size, "CVMAT_NAME" camera_matrix, "CVMAT_NAME" distortion_matrix, "CVMAT_NAME" rotation_vectors=nil, "CVMAT_NAME" translation_vectors=nil, int flags=0)";
  lua_Integer flags=0;
  CvMat *rot=NULL,*trans=NULL;
 switch(lua_gettop(L))
  {
    case 6:
      break;
    case 7:
      rot=checkCvMat(L,7);
      break;
    case 8:
      rot=checkCvMat(L,7);
      trans=checkCvMat(L,8);
      break;
    case 9:
      rot=checkCvMat(L,7);
      trans=checkCvMat(L,8);
      flags=checkint(L,9);
      break;

    default:
      luaL_error(L,f_msg);
  }

 lua_pushnumber(L,cvCalibrateCamera2(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),*checkCvSize(L,4),checkCvMat(L,5),checkCvMat(L,6),rot,trans,flags));
  return 1;
}

static int luacv_cvCalibrationMatrixValues(lua_State *L)
{//zmena
  const char f_msg[]="num,num,num,num CalibrationMatrixValues("CVMAT_NAME" camera_matrix, "CVSIZE_NAME" image_size, num aperature_width=0, num aperature_height=0, "CVPOINT2D64F_NAME" principal_point=nil)";
 lua_Number width=0,height=0,fovx,fovy,focal,pixel; 
 CvPoint2D64f *point=NULL;
 switch(lua_gettop(L))
  {
    case 2:
      break;
    case 3:
      width=checknumber(L,3);
      break;
    case 4:
      width=checknumber(L,3);
      height=checknumber(L,4);
      break;
    case 5:
      width=checknumber(L,3);
      height=checknumber(L,4);
      point=checkCvPoint2D64f(L,5);
      break;
 
    default:
      luaL_error(L,f_msg);
  }
  
  cvCalibrationMatrixValues(checkCvMat(L,1),*checkCvSize(L,2),width,height,&fovx,&fovy,&focal,point,&pixel);
  lua_pushnumber(L,fovx);
  lua_pushnumber(L,fovy);
  lua_pushnumber(L,focal);
  lua_pushnumber(L,pixel);
  return 4;
}

static int luacv_cvStereoCalibrate(lua_State *L)
{
  const char f_msg[]="num StereoCalibrate("CVMAT_NAME" object_points, "CVMAT_NAME" image_points, "CVMAT_NAME" camera_matrix1, "CVMAT_NAME" dist_coeffs1, "CVMAT_NAME" camera_matrix2, "CVMAT_NAME" dist_coeffs2, "CVSIZE_NAME" size, "CVMAT_NAME" R, "CVMAT_NAME" T, "CVMAT_NAME" E=nil, "CVMAT_NAME" P2, "CVMAT_NAME" F=nil, "CVTERMCRITERIA_NAME" term_crit=cv.TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,30,0,000006), int flags=CV_CALIB_FIX_INTRINSIC)";
  lua_Integer flags=CV_CALIB_FIX_INTRINSIC;
  CvMat *E=NULL,*F=NULL;
  CvTermCriteria term_crit=cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS,30,1e-6);

 switch(lua_gettop(L))
  {
    case 11:
      break;
    case 12:
      E=checkCvMat(L,12);
      break;
    case 13:
      E=checkCvMat(L,12);
      F=checkCvMat(L,13);
      break;
    case 14:
      E=checkCvMat(L,12);
      F=checkCvMat(L,13);
      term_crit=*checkCvTermCriteria(L,14);
      break;
    case 15:
      E=checkCvMat(L,12);
      F=checkCvMat(L,13);
      term_crit=*checkCvTermCriteria(L,14);
      flags=checkint(L,15);
      break;

    default:
      luaL_error(L,f_msg);
  }

 lua_pushnumber(L,cvStereoCalibrate(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),checkCvMat(L,5),checkCvMat(L,6),checkCvMat(L,7),checkCvMat(L,8),*checkCvSize(L,9),checkCvMat(L,10),checkCvMat(L,11),E,F,term_crit,flags));
 return 1;
}
static int luacv_cvStereoRectify(lua_State *L)
{
    const char f_msg[]="StereoRectify("CVMAT_NAME" camera_matrix1, "CVMAT_NAME" camera_matrix2, "CVMAT_NAME" dist_coeffs1, "CVMAT_NAME" dist_coeffs2, "CVSIZE_NAME" image_size, "CVMAT_NAME" R, "CVMAT_NAME" T, "CVMAT_NAME" R1, "CVMAT_NAME" R2, "CVMAT_NAME" P1, "CVMAT_NAME" P2, "CVMAT_NAME" Q=nil, int flags=CV_CALIB_ZERO_DISPARITY, num alpha=-1, "CVSIZE_NAME" new_image_size=cv.Size(0,0), "CVRECT_NAME" valid_pix_roi1=nil, "CVRECT_NAME" valid_pix_roi2=nil)";

  lua_Integer flags=0;
  lua_Number alpha=-1;
  CvMat *Q=NULL;
  CvSize size=cvSize(0,0);
  CvRect *roi1=NULL,*roi2=NULL;

 switch(lua_gettop(L))
  {
    case 11:
      break;
    case 12:
      Q=checkCvMat(L,12);
      break;
    case 13:
      Q=checkCvMat(L,12);
      flags=checkint(L,13);
      break;
    case 14:
      Q=checkCvMat(L,12);
      flags=checkint(L,13);
      alpha=checknumber(L,14);
      break;
    case 15:
      Q=checkCvMat(L,12);
      flags=checkint(L,13);
      alpha=checknumber(L,14);
      size=*checkCvSize(L,15);
      break;
    case 16:
      Q=checkCvMat(L,12);
      flags=checkint(L,13);
      alpha=checknumber(L,14);
      size=*checkCvSize(L,15);
      roi1=checkCvRect(L,16);
      break;
    case 17:
      Q=checkCvMat(L,12);
      flags=checkint(L,13);
      alpha=checknumber(L,14);
      size=*checkCvSize(L,15);
      roi1=checkCvRect(L,16);
      roi2=checkCvRect(L,17);
      break;
    default:
      luaL_error(L,f_msg);
  }

 cvStereoRectify(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),*checkCvSize(L,5),checkCvMat(L,6),checkCvMat(L,7),checkCvMat(L,8),checkCvMat(L,9),checkCvMat(L,10),checkCvMat(L,11),Q,flags,alpha,size,roi1,roi2);

  return 0;
}
static int luacv_cvStereoRectifyUncalibrated(lua_State *L)
{
   const char f_msg[]="num StereoRectifyUncalibrated("CVMAT_NAME" points1, "CVMAT_NAME" points2, "CVMAT_NAME" F, "CVSIZE_NAME" img_size, "CVMAT_NAME" H1, "CVMAT_NAME" T, "CVMAT_NAME" H2, num threshold=5)";
 lua_Number thresh=5;

 switch(lua_gettop(L))
  {
    case 6:
      break;
    case 7:
      thresh=checknumber(L,7);
      break;
    default:
      luaL_error(L,f_msg);
  }

 lua_pushnumber(L,cvStereoRectifyUncalibrated(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),*checkCvSize(L,4),checkCvMat(L,5),checkCvMat(L,6),thresh));
  return 1;
}

static int luacv_cvCreateStereoBMState(lua_State *L)
{
   const char f_msg[]=CVSTEREOBMSTATE_NAME" CreateStereoBMState(int preset=CV_STEREO_BM_BASIC, int numberOfDisparities=0)";
 lua_Integer num=5;

 switch(lua_gettop(L))
  {
    case 1:
      break;
    case 2:
      num=checkint(L,2);
      break;
    default:
      luaL_error(L,f_msg);
  }

 pushCvStereoBMState(L,cvCreateStereoBMState(checkint(L,1),num));
  return 1;
}

static int luacv_cvReleaseStereoBMState(lua_State *L)
{
  const char f_msg[]="ReleaseStereoBMState("CVSTEREOBMSTATE_NAME" state)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvStereoBMState(L,1);
  luacv_releaseObject(L,1);
  return 0;
}

static int luacv_cvFindStereoCorrespondenceBM(lua_State *L)
{
  const char f_msg[]="FindStereoCorrespondenceBM("CVARR_NAME" left, "CVARR_NAME" right, "CVARR_NAME" disparity, "CVSTEREOBMSTATE_NAME"state)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);

 cvFindStereoCorrespondenceBM(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvStereoBMState(L,4));
  return 0;
}

static int luacv_cvGetValidDisparityROI(lua_State *L)
{
  const char f_msg[]=CVRECT_NAME" GetValidDisparityROI("CVRECT_NAME" roi1, "CVRECT_NAME" roi2, int minDisparity, int numberOfDisparities, int SADWIndowSize)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  CvRect rect=cvGetValidDisparityROI(*checkCvRect(L,1),*checkCvRect(L,2),checkint(L,3),checkint(L,4),checkint(L,5));
  pushCvRect(L,&rect);
  return 1;
}
static int luacv_cvValidateDisparity(lua_State *L)
{
  const char f_msg[]="ValidateDisparity("CVARR_NAME" disparity, "CVARR_NAME" cost, int minDisparity, int numberOfDisparities, int disp12MaxDiff=1)";
 lua_Integer disp=1;

 switch(lua_gettop(L))
  {
    case 4:
      break;
    case 5:
      disp=checkint(L,5);
      break;
    default:
      luaL_error(L,f_msg);
  }

 cvValidateDisparity(checkCvArr(L,1),checkCvArr(L,2),checkint(L,3),checkint(L,4),disp);
 return 0;
}


static int luacv_cvCreateStereoGCState(lua_State *L)
{
  const char f_msg[]=CVSTEREOGCSTATE_NAME" CreateStereoGCState(int numberOfDisparities, int maxIters)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  pushCvStereoGCState(L,cvCreateStereoGCState(checkint(L,1),checkint(L,2)));
  return 1;

}

static int luacv_cvReleaseStereoGCState(lua_State *L)
{
  const char f_msg[]="ReleaseStereoGCState("CVSTEREOGCSTATE_NAME" state)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvStereoGCState(L,1);
  luacv_releaseObject(L,1);

  return 0;
}
static int luacv_cvFindStereoCorrespondenceGC(lua_State *L)
{
  const char f_msg[]="FindStereoCorrespondenceGC("CVMAT_NAME" left, "CVMAT_NAME" right, "CVMAT_NAME" disparityLeft, "CVMAT_NAME" disparityRight, "CVSTEREOBMSTATE_NAME"state, int useDiparityGuess=0)";
 lua_Integer use=0;
 switch(lua_gettop(L))
  {
    case 5:
      break;
    case 6:
      use=checkint(L,6);
      break;
    default:
      luaL_error(L,f_msg);
  }

  cvFindStereoCorrespondenceGC(checkCvMat(L,1),checkCvMat(L,2),checkCvMat(L,3),checkCvMat(L,4),checkCvStereoGCState(L,5),use);
  return 0;
}

static int luacv_cvReprojectImageTo3D(lua_State *L)
{
  const char f_msg[]="ReprojectImageTo3D("CVARR_NAME" disparityImage, "CVARR_NAME" _3dImage, "CVMAT_NAME" Q, int handleMissingValues=0)";
 lua_Integer handle=0;
 switch(lua_gettop(L))
  {
    case 3:
      break;
    case 4:
      handle=checkint(L,4);
      break;
    default:
      luaL_error(L,f_msg);
  }
  cvReprojectImageTo3D(checkCvArr(L,1),checkCvArr(L,2),checkCvMat(L,3),handle);
  return 0;
}

const luaL_Reg calib3d[]=
{

  funcReg(CreatePOSITObject),                     funcReg(POSIT),
  funcReg(ReleasePOSITObject),                    funcReg(RANSACUpdateNumIters),
  funcReg(ConvertPointsHomogeneous),              funcReg(FindFundamentalMat),
  funcReg(ComputeCorrespondEpilines),             funcReg(TriangulatePoints),
  funcReg(CorrectMatches),                        funcReg(GetOptimalNewCameraMatrix),
  funcReg(Rodrigues2),                            funcReg(FindHomography),
  funcReg(RQDecomp3x3),                           funcReg(DecomposeProjectionMatrix),
  funcReg(CalcMatMulDeriv),                       funcReg(ComposeRT),
  funcReg(ProjectPoints2),                        funcReg(FindExtrinsicCameraParams2),
  funcReg(InitIntrinsicParams2D),                 funcReg(CheckChessboard),
  funcReg(FindChessboardCorners),                 funcReg(DrawChessboardCorners),
  funcReg(CalibrateCamera2),                      funcReg(CalibrationMatrixValues),
  funcReg(StereoCalibrate),                       funcReg(StereoRectify),
  funcReg(StereoRectifyUncalibrated),             funcReg(CreateStereoBMState),
  funcReg(ReleaseStereoBMState),                  funcReg(FindStereoCorrespondenceBM),
  funcReg(GetValidDisparityROI),                  funcReg(ValidateDisparity),
  funcReg(CreateStereoGCState),                   funcReg(ReleaseStereoGCState),
  funcReg(FindStereoCorrespondenceGC),            funcReg(ReprojectImageTo3D),
  {NULL,NULL}
};

const luacv_var calib3d_object[]=
{
  //{CVPOSITOBJECT_NAME,sizeof(CvPOSITObject)},
  {CVSTEREOBMSTATE_NAME,sizeof(CvStereoBMState)},
  {CVSTEREOGCSTATE_NAME,sizeof(CvStereoGCState)},
  {NULL,0}
};



const luacv_var calib3d_var[]=
{
  varReg(CV_CALIB_CB_ADAPTIVE_THRESH),      varReg(CV_CALIB_CB_NORMALIZE_IMAGE),
  varReg(CV_CALIB_CB_FILTER_QUADS),         varReg(CV_CALIB_USE_INTRINSIC_GUESS),
  varReg(CV_CALIB_FIX_ASPECT_RATIO),        varReg(CV_CALIB_FIX_PRINCIPAL_POINT),
  varReg(CV_CALIB_ZERO_TANGENT_DIST),       varReg(CV_CALIB_FIX_FOCAL_LENGTH),
  varReg(CV_CALIB_FIX_K1),                  varReg(CV_CALIB_FIX_K2),
  varReg(CV_CALIB_FIX_K3),                  varReg(CV_CALIB_FIX_INTRINSIC),
  varReg(CV_CALIB_SAME_FOCAL_LENGTH),       varReg(CV_CALIB_ZERO_DISPARITY),
  varReg(CV_FM_7POINT),                     varReg(CV_FM_8POINT),
  varReg(CV_FM_LMEDS_ONLY),                 varReg(CV_FM_RANSAC_ONLY),
  varReg(CV_FM_LMEDS),                      varReg(CV_FM_RANSAC),
  varReg(CV_STEREO_BM_NORMALIZED_RESPONSE), varReg(CV_STEREO_BM_BASIC),
  varReg(CV_STEREO_BM_FISH_EYE),            varReg(CV_STEREO_BM_NARROW),
  varReg(CV_STEREO_GC_OCCLUDED),            varReg(CV_LMEDS),
  varReg(CV_RANSAC),
  {NULL,0}
};
