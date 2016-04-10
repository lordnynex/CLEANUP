#include "lua_video.h"
static int luacv_cvCalcOpticalFlowFarneback(lua_State *L)
{
  const char f_msg[]="CalcOpticalFlowFarneback("CVARR_NAME" prev, "CVARR_NAME" next, "CVARR_NAME" flow, num pyr_scale, int levels, int winsize, int iterations, int poly_n, num poly_sigma, int flags)";
  if (lua_gettop(L)!=10) luaL_error(L,f_msg);
  cvCalcOpticalFlowFarneback(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3), checknumber(L,4),checkint(L,5),checkint(L,6),checkint(L,7),checkint(L,8),checknumber(L,9),checkint(L,10));
  
  return 0;
}

static int luacv_cvEstimateRigidTransform(lua_State *L)
{
  const char f_msg[]="EstimateRigidTransform("CVARR_NAME" A, "CVARR_NAME" B, "CVMAT_NAME" M, int full_affine)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  lua_pushnumber(L,cvEstimateRigidTransform(checkCvArr(L,1),checkCvArr(L,2),checkCvMat(L,3), checknumber(L,4)));
  
  return 1;
}

static int luacv_cvCalcOpticalFlowLK(lua_State *L)
{
  const char f_msg[]="CalcOpticalFlowLK("CVARR_NAME" prev, "CVARR_NAME" cur, "CVSIZE_NAME" win_size, "CVARR_NAME" velx, "CVARR_NAME" vely)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  cvCalcOpticalFlowLK(checkCvArr(L,1),checkCvArr(L,2),*checkCvSize(L,3),checkCvArr(L,4),checkCvArr(L,5));
  return 0;
}

static int luacv_cvCalcOpticalFlowBM(lua_State *L)
{
  const char f_msg[]="CalcOpticalFlowBM("CVARR_NAME" prev, "CVARR_NAME" cur, "CVSIZE_NAME" block_size, "CVSIZE_NAME" shift_size, "CVSIZE_NAME" max_range, int use_previous, "CVARR_NAME" velx, "CVARR_NAME" vely)";
  if (lua_gettop(L)!=8) luaL_error(L,f_msg);
  cvCalcOpticalFlowBM(checkCvArr(L,1),checkCvArr(L,2),*checkCvSize(L,3),*checkCvSize(L,4),*checkCvSize(L,5),checkint(L,6),checkCvArr(L,7),checkCvArr(L,8));
  return 0;
}

static int luacv_cvCalcOpticalFlowHS(lua_State *L)
{
  const char f_msg[]="CalcOpticalFlowHS("CVARR_NAME" prev, "CVARR_NAME" cur, int use_previous, "CVARR_NAME" velx, "CVARR_NAME" vely, num lambda, "CVTERMCRITERIA_NAME" criteria)";
  if (lua_gettop(L)!=7) luaL_error(L,f_msg);
  cvCalcOpticalFlowHS(checkCvArr(L,1),checkCvArr(L,2),checkint(L,3),checkCvArr(L,4),checkCvArr(L,5),checknumber(L,6),*checkCvTermCriteria(L,7));
  return 0;
}




static int luacv_cvCalcOpticalFlowPyrLK(lua_State *L)
{//zmena
  const char f_msg[]="string CalcOpticalFlowPyrLK("CVARR_NAME" prev, "CVARR_NAME" curr, "CVARR_NAME" prev_pyr, "CVARR_NAME" curr_pyr, "CVPOINT2D32F_NAME"[] prev_features, "CVPOINT2D32F_NAME"[] curr_features, int count, "CVSIZE_NAME" win_size, int level, "CVTERMCRITERIA_NAME" criteria, int flags)";
  if ((lua_gettop(L)!=11)||(!lua_istable(L,5))||(!lua_istable(L,6))) luaL_error(L,f_msg);

  int count=checkint(L,7);
  CvPoint2D32f *prev=(CvPoint2D32f*)luacv_alloc(sizeof(CvPoint2D32f)*count);
  CvPoint2D32f *cur=(CvPoint2D32f*)luacv_alloc(sizeof(CvPoint2D32f)*count);

  for(int i=0;i<count;i++)
  {
    lua_rawgeti(L,5,i+1); 
    prev[i]=*checkCvPoint2D32f(L,lua_gettop(L));
    lua_pop(L,1);
    lua_rawgeti(L,6,i+1); 
    cur[i]=*checkCvPoint2D32f(L,lua_gettop(L));
    lua_pop(L,1);
  }

  char *status=(char*)luacv_alloc(count);

  cvCalcOpticalFlowPyrLK(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4),prev,cur,count,*checkCvSize(L,8),checkint(L,9),status,0,*checkCvTermCriteria(L,10),checkint(L,11));

  for(int i=0;i<count;i++)
  {
    pushCvPoint2D32f(L,&prev[i]); 
    lua_rawseti(L,5,i+1); 
    pushCvPoint2D32f(L,&cur[i]); 
    lua_rawseti(L,6,i+1); 
  }

  lua_pushstring(L,status);
  luacv_free(&prev);
  luacv_free(&cur);
  luacv_free(&status);
  return 1;
}

static int luacv_cvCalcAffineFlowPyrLK(lua_State *L)
{//zmena
  const char f_msg[]="string CalcAffineFlowPyrLK("CVARR_NAME" prev, "CVARR_NAME" curr, "CVARR_NAME" prev_pyr, "CVARR_NAME" curr_pyr, "CVPOINT2D32F_NAME"[] prev_features, "CVPOINT2D32F_NAME"[] curr_features, num[] matrices, int count, "CVSIZE_NAME" win_size, int level, "CVTERMCRITERIA_NAME" criteria, int flags)";
  if ((lua_gettop(L)!=12)||(!lua_istable(L,5))||(!lua_istable(L,6))||(!lua_istable(L,7))) luaL_error(L,f_msg);

  int count=checkint(L,8);
  CvPoint2D32f *prev=(CvPoint2D32f*)luacv_alloc(sizeof(CvPoint2D32f)*count);
  CvPoint2D32f *cur=(CvPoint2D32f*)luacv_alloc(sizeof(CvPoint2D32f)*count);
  float *matrices=(float*)luacv_alloc(sizeof(float)*count);
  for(int i=0;i<count;i++)
  {
    lua_rawgeti(L,5,i+1); 
    prev[i]=*checkCvPoint2D32f(L,lua_gettop(L));
    lua_pop(L,1);
    lua_rawgeti(L,6,i+1); 
    cur[i]=*checkCvPoint2D32f(L,lua_gettop(L));
    lua_pop(L,1);
    lua_rawgeti(L,7,i+1); 
    matrices[i]=checknumber(L,lua_gettop(L));
    lua_pop(L,1);
  }

  char *status=(char*)luacv_alloc(count);

  cvCalcOpticalFlowPyrLK(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checkCvArr(L,4),prev,cur,count,*checkCvSize(L,9),checkint(L,10),status,0,*checkCvTermCriteria(L,11),checkint(L,12));

  for(int i=0;i<count;i++)
  {
    pushCvPoint2D32f(L,&prev[i]); 
    lua_rawseti(L,5,i+1); 
    pushCvPoint2D32f(L,&cur[i]); 
    lua_rawseti(L,6,i+1); 
    lua_pushnumber(L,matrices[i]); 
    lua_rawseti(L,7,i+1); 
  }

  lua_pushstring(L,status);
  luacv_free(&prev);
  luacv_free(&cur);
  luacv_free(&matrices);
  luacv_free(&status);
  return 1;
}



static int luacv_cvCreateKalman(lua_State *L)
{  
  const char f_msg[]=CVKALMAN_NAME" CreateKalman(int dynam_params, int measure_params, int control_params=0)";
  int control_params=0;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            control_params=checkint(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }
  pushCvKalman(L,cvCreateKalman(checkint(L,1),checkint(L,2),control_params)); 
  return 1;
}

static int luacv_cvKalmanCorrect(lua_State *L)
{
  const char f_msg[]=CVMAT_NAME" KalmanCorrect("CVKALMAN_NAME" kalman, "CVMAT_NAME" measurement)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  pushCvMat(L,(CvMat*)cvKalmanCorrect(checkCvKalman(L,1),checkCvMat(L,2)));
  return 1;
}


static int luacv_cvKalmanPredict(lua_State *L)
{  
  const char f_msg[]=CVMAT_NAME" KalmanPredict("CVKALMAN_NAME" kalman, "CVMAT_NAME" control=nil)";
  CvMat *control=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            control=checkCvMat(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }
  pushCvMat(L,(CvMat*)cvKalmanPredict(checkCvKalman(L,1),control)); 
  return 1;
}

static int luacv_cvReleaseKalman(lua_State*L)
{
 const char f_msg[]="ReleaseKalman("CVKALMAN_NAME" kalman)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvKalman(L,1);
  luacv_releaseObject(L,1);
	return 0;
}


static int luacv_cvUpdateMotionHistory(lua_State*L)
{
 const char f_msg[]="UpdateMotionHistory("CVARR_NAME" silhouette, "CVARR_NAME" mhi, num timestamp, num duration)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  cvUpdateMotionHistory(checkCvArr(L,1),checkCvArr(L,2),checknumber(L,3),checknumber(L,4));
	return 0;
}


static int luacv_cvCalcMotionGradient(lua_State *L)
{  
  const char f_msg[]="CalcMotionGradient("CVARR_NAME" mhi, "CVARR_NAME" mask, "CVARR_NAME" orientation, num delta1, num delta2, int aperature_size=3)";
  int size=3;
  switch(lua_gettop(L))
  {
    case 5:
            break;
    case 6:
            size=checkint(L,6);
            break;
    default:
            luaL_error(L,f_msg);
  }

  cvCalcMotionGradient(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checknumber(L,4),checknumber(L,5),size);
  return 0;
}

static int luacv_cvCalcGlobalOrientation(lua_State*L)
{
 const char f_msg[]="num CalcGlobalOrientation("CVARR_NAME" orientation, "CVARR_NAME" mask, "CVARR_NAME" mhi, num timestamp, num duration)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  lua_pushnumber(L,cvCalcGlobalOrientation(checkCvArr(L,1),checkCvArr(L,2),checkCvArr(L,3),checknumber(L,4),checknumber(L,5)));
	return 1;
}

static int luacv_cvSegmentMotion(lua_State*L)
{
 const char f_msg[]=CVSEQ_NAME" SegmentMotion("CVARR_NAME" mhi, "CVARR_NAME" seg_msk, "CVMEMSTORAGE_NAME" storage , num timestamp, num duration)";
  if (lua_gettop(L)!=5) luaL_error(L,f_msg);
  pushCvSeq(L,cvSegmentMotion(checkCvArr(L,1),checkCvArr(L,2),checkCvMemStorage(L,3),checknumber(L,4),checknumber(L,5)));
	return 1;
}

static int luacv_cvCamShift(lua_State *L)
{  
  const char f_msg[]="num CamShift("CVARR_NAME" prob_image, "CVRECT_NAME" window, "CVTERMCRITERIA_NAME" criteria, "CVCONNECTEDCOMP_NAME" comp, "CVBOX2D_NAME" box=nil)";
  CvBox2D *box=NULL;
  switch(lua_gettop(L))
  {
    case 4:
            break;
    case 5:
            box=checkCvBox2D(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }

  lua_pushnumber(L,cvCamShift(checkCvArr(L,1),*checkCvRect(L,2),*checkCvTermCriteria(L,3),checkCvConnectedComp(L,4),box));
  return 1;
}

static int luacv_cvMeanShift(lua_State*L)
{
  const char f_msg[]="num MeanShift("CVARR_NAME" prob_image, "CVRECT_NAME" window, "CVTERMCRITERIA_NAME" criteria, "CVCONNECTEDCOMP_NAME" comp)";
  if (lua_gettop(L)!=4) luaL_error(L,f_msg);
  lua_pushnumber(L,cvMeanShift(checkCvArr(L,1),*checkCvRect(L,2),*checkCvTermCriteria(L,3),checkCvConnectedComp(L,4)));
	return 1;
}

/***************************************
 *           background sengmentation  */

static int luacv_cvReleaseBGStatModel(lua_State*L)
{
  const char f_msg[]="ReleaseBGStatModel("CVBGSTATMODEL_NAME" model)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvBGStatModel(L,1);
  luacv_releaseObject(L,1);
	return 0;
}

static int luacv_cvUpdateBGStatModel(lua_State *L)
{  
  const char f_msg[]="num UpdateBGStatModel("IPLIMAGE_NAME" current_frame, "CVBGSTATMODEL_NAME" bg_model, num learningRate=-1)";
  double rate=-1;
  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            rate=checknumber(L,3);
            break;
    default:
            luaL_error(L,f_msg);
  }

  lua_pushnumber(L,cvUpdateBGStatModel(checkIplImage(L,1),checkCvBGStatModel(L,2),rate));
  return 1;
}

static int luacv_cvRefineForegroundMaskBySegm(lua_State*L)
{
  const char f_msg[]="RefineForegroundMaskBySegm("CVSEQ_NAME" segments, "CVBGSTATMODEL_NAME" bg_model)";
  if (lua_gettop(L)!=2) luaL_error(L,f_msg);
  cvRefineForegroundMaskBySegm(checkCvSeq(L,1),checkCvBGStatModel(L,2));
	return 0;
}

static int luacv_cvChangeDetection(lua_State*L)
{
  const char f_msg[]="num ChangeDetection("IPLIMAGE_NAME" prev_frame, "IPLIMAGE_NAME" curr_frame, "IPLIMAGE_NAME" change_mask)";
  if (lua_gettop(L)!=3) luaL_error(L,f_msg);
  lua_pushnumber(L,cvChangeDetection(checkIplImage(L,1),checkIplImage(L,2),checkIplImage(L,3)));
	return 1;
}

static int luacv_cvCreateFGDStatModel(lua_State *L)
{  
  const char f_msg[]=CVBGSTATMODEL_NAME" CreateFDGStatModel("IPLIMAGE_NAME" first_frame, "CVFGDSTATMODELPARAMS_NAME" params=nil)";
  CvFGDStatModelParams *params=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            params=checkCvFGDStatModelParams(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }

  pushCvBGStatModel(L,cvCreateFGDStatModel(checkIplImage(L,1),params));
  return 1;
}

static int luacv_cvCreateGaussianBGModel(lua_State *L)
{  
  const char f_msg[]=CVBGSTATMODEL_NAME" CreateGaussianBGModel("IPLIMAGE_NAME" first_frame, "CVGAUSSBGSTATMODELPARAMS_NAME" params=nil)";
  CvGaussBGStatModelParams *params=NULL;
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            params=checkCvGaussBGStatModelParams(L,2);
            break;
    default:
            luaL_error(L,f_msg);
  }

  pushCvBGStatModel(L,cvCreateGaussianBGModel(checkIplImage(L,1),params));
  return 1;
}


static int luacv_cvCreateBGCodeBookModel(lua_State*L)
{
  const char f_msg[]=CVBGCODEBOOKMODEL_NAME" CreateBGCodeBookModel()";
  if (lua_gettop(L)!=0) luaL_error(L,f_msg);
  pushCvBGCodeBookModel(L,cvCreateBGCodeBookModel());
	return 1;
}

static int luacv_cvReleaseBGCodeBookModel(lua_State*L)
{
  const char f_msg[]="ReleaseBGCodeBookModel("CVBGCODEBOOKMODEL_NAME" model)";
  if (lua_gettop(L)!=1) luaL_error(L,f_msg);
  checkCvBGCodeBookModel(L,1);
  luacv_releaseObject(L,1);
	return 0;
}

static int luacv_cvBGCodeBookUpdate(lua_State *L)
{  
  const char f_msg[]="BGCodeBookUpdate("CVBGCODEBOOKMODEL_NAME" model, "CVARR_NAME" image, "CVRECT_NAME" roi=cv.Rect(0,0,0,0), "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  CvRect roi=cvRect(0,0,0,0);

  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            roi=*checkCvRect(L,3);
            break;
    case 4:
            roi=*checkCvRect(L,3);
            mask=checkCvArr(L,4);
            break;

    default:
            luaL_error(L,f_msg);
  }

  cvBGCodeBookUpdate(checkCvBGCodeBookModel(L,1),checkCvArr(L,2),roi,mask);
  return 0;
}

static int luacv_cvBGCodeBookDiff(lua_State *L)
{  
  const char f_msg[]="num BGCodeBookDiff("CVBGCODEBOOKMODEL_NAME" model, "CVARR_NAME" image, "CVARR_NAME" fgmask,"CVRECT_NAME" roi=cv.Rect(0,0,0,0))";
  CvRect roi=cvRect(0,0,0,0);

  switch(lua_gettop(L))
  {
    case 3:
            break;
    case 4:
            roi=*checkCvRect(L,4);
            break;
    default:
            luaL_error(L,f_msg);
  }

  lua_pushnumber(L,cvBGCodeBookDiff(checkCvBGCodeBookModel(L,1),checkCvArr(L,2),checkCvArr(L,3),roi));
  return 1;
}

static int luacv_cvBGCodeBookClearStale(lua_State *L)
{  
  const char f_msg[]="BGCodeBookClearStale("CVBGCODEBOOKMODEL_NAME" model, int staleThresh, "CVRECT_NAME" roi=cv.Rect(0,0,0,0), "CVARR_NAME" mask=nil)";
  CvArr *mask=NULL;
  CvRect roi=cvRect(0,0,0,0);

  switch(lua_gettop(L))
  {
    case 2:
            break;
    case 3:
            roi=*checkCvRect(L,3);
            break;
    case 4:
            roi=*checkCvRect(L,3);
            mask=checkCvArr(L,4);
            break;

    default:
            luaL_error(L,f_msg);
  }

  cvBGCodeBookClearStale(checkCvBGCodeBookModel(L,1),checkint(L,2),roi,mask);
  return 0;
}

static int luacv_cvSegmentFGMask(lua_State *L)
{  
  const char f_msg[]=CVSEQ_NAME" SegmentFGMask("CVARR_NAME" fgmask, int poly1Hull0=1, num perimScale=4, "CVMEMSTORAGE_NAME" storage=nil, "CVPOINT_NAME" offset=cv.Point(0,0))";
  int poly=1;
  float perim=4;
  CvMemStorage *storage=NULL;
  CvPoint off=cvPoint(0,0);
  switch(lua_gettop(L))
  {
    case 1:
            break;
    case 2:
            poly=checkint(L,2);
            break;
    case 3:
            poly=checkint(L,2);
            perim=(float)checknumber(L,3);
            break;
    case 4:
            poly=checkint(L,2);
            perim=(float)checknumber(L,3);
            storage=checkCvMemStorage(L,4);
            break;
    case 5:
            poly=checkint(L,2);
            perim=(float)checknumber(L,3);
            storage=checkCvMemStorage(L,4);
            off=*checkCvPoint(L,5);
            break;
    default:
            luaL_error(L,f_msg);
  }

  pushCvSeq(L,cvSegmentFGMask(checkCvArr(L,1),poly,perim,storage,off));
  return 1;
}

const luaL_Reg video[]=
{
  funcReg(CalcOpticalFlowFarneback),  funcReg(CreateKalman),  funcReg(KalmanPredict),
  funcReg(KalmanCorrect),             funcReg(ReleaseKalman),
  {"KalmanUpdateByTime",luacv_cvKalmanPredict},{"KalmanUpdateByMeasurement",luacv_cvKalmanCorrect},
  funcReg(UpdateMotionHistory),       funcReg(CalcMotionGradient),
  funcReg(CalcGlobalOrientation),     funcReg(SegmentMotion),
  funcReg(CalcOpticalFlowPyrLK),      funcReg(CamShift),      funcReg(MeanShift),
  funcReg(EstimateRigidTransform),    funcReg(CalcAffineFlowPyrLK),
  funcReg(CalcOpticalFlowLK),         funcReg(CalcOpticalFlowBM),
  funcReg(CalcOpticalFlowHS),         funcReg(ReleaseBGStatModel),
  funcReg(UpdateBGStatModel),         funcReg(RefineForegroundMaskBySegm),
  funcReg(ChangeDetection),           funcReg(CreateFGDStatModel),
  funcReg(CreateGaussianBGModel),     funcReg(CreateBGCodeBookModel),
  funcReg(ReleaseBGCodeBookModel),    funcReg(BGCodeBookUpdate),
  funcReg(BGCodeBookDiff),            funcReg(BGCodeBookClearStale),
  funcReg(SegmentFGMask),
  {NULL,NULL}
};
 
const luacv_var video_object[]=
{
  {CVKALMAN_NAME,sizeof(CvKalman)},
  {CVBGSTATMODEL_NAME,sizeof(CvBGStatModel)},
  {CVFGDSTATMODELPARAMS_NAME,sizeof(CvFGDStatModelParams)},
  {CVBGPIXELCSTATTABLE_NAME,sizeof(CvBGPixelCStatTable)},
  {CVBGPIXELCCSTATTABLE_NAME,sizeof(CvBGPixelCCStatTable)},
  {CVBGPIXELSTAT_NAME,sizeof(CvBGPixelStat)},
  {CVFGDSTATMODEL_NAME,sizeof(CvFGDStatModel)},
  {CVGAUSSBGSTATMODELPARAMS_NAME,sizeof(CvGaussBGStatModelParams)},
  {CVGAUSSBGVALUES_NAME,sizeof(CvGaussBGValues)},
  {CVGAUSSBGPOINT_NAME,sizeof(CvGaussBGPoint)},
  {CVGAUSSBGMODEL_NAME,sizeof(CvGaussBGModel)},
  {CVBGCODEBOOKELEM_NAME,sizeof(CvBGCodeBookElem)},
  {CVBGCODEBOOKMODEL_NAME,sizeof(CvBGCodeBookModel)},

  {NULL,0}
};


const luacv_var video_var[]=
{
  varReg(CV_LKFLOW_PYR_A_READY),            varReg(CV_LKFLOW_PYR_B_READY),
  varReg(CV_LKFLOW_INITIAL_GUESSES),        varReg(CV_LKFLOW_GET_MIN_EIGENVALS),
  varReg(CV_BGFG_FGD_LC),                   varReg(CV_BGFG_FGD_N1C),
  varReg(CV_BGFG_FGD_N2C),                  varReg(CV_BGFG_FGD_LCC),
  varReg(CV_BGFG_FGD_N1CC),                 varReg(CV_BGFG_FGD_N2CC),
  varReg(CV_BGFG_FGD_ALPHA_1),              varReg(CV_BGFG_FGD_ALPHA_2),
  varReg(CV_BGFG_FGD_ALPHA_3),              varReg(CV_BGFG_FGD_DELTA),
  varReg(CV_BGFG_FGD_T),                    varReg(CV_BGFG_FGD_MINAREA),
  varReg(CV_BGFG_FGD_BG_UPDATE_TRESH),      varReg(CV_BGFG_MOG_MAX_NGAUSSIANS),
  varReg(CV_BGFG_MOG_BACKGROUND_THRESHOLD), varReg(CV_BGFG_MOG_STD_THRESHOLD),
  varReg(CV_BGFG_MOG_WINDOW_SIZE),          varReg(CV_BGFG_MOG_NGAUSSIANS),
  varReg(CV_BGFG_MOG_WEIGHT_INIT),          varReg(CV_BGFG_MOG_SIGMA_INIT),
  varReg(CV_BGFG_MOG_MINAREA),              varReg(CV_BGFG_MOG_NCOLORS),
  {NULL,0}
};
 
