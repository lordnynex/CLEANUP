#include "CvStereoBMState.h"

static int CvStereoBMState_tostring(lua_State *L)
{
  CvStereoBMState *s=checkCvStereoBMState(L,1);
  lua_pushfstring(L,CVSTEREOBMSTATE_NAME" object: %p\n\tint preFilterType=%d\n\tint preFilterSize=%d\n\tint preFilterCap=%d\n\tint SADWindowSize=%d\n\tint minDisparity=%d\n\tint numberOfDisparities=%d\n\tint textureThreshold=%d\n\tint uniquenessRatio=%d\n\tint speckleWindowSize=%d\n\tint speckleRange=%d\n\tint trySmallerWindows=%d\n\t"CVRECT_NAME" roi1 { [x]=>%d, [y]=>%d, [width]=>%d, [height]=>%d }\n\t"CVRECT_NAME" roi2 { [x]=>%d, [y]=>%d, [width]=>%d, [height]=>%d }\n\tint disp12MaxDiff=%d\n\t"CVMAT_NAME" preFilteredImg0=%p\n\t"CVMAT_NAME" preFilteredImg1=%p\n\t"CVMAT_NAME" slidingSumBuf=%p\n\t"CVMAT_NAME" cost=%p\n\t"CVMAT_NAME" disp=%p",s,s->preFilterType,s->preFilterSize,s->preFilterCap,s->SADWindowSize,s->minDisparity,s->numberOfDisparities,s->textureThreshold,s->uniquenessRatio,s->speckleWindowSize,s->speckleRange,s->trySmallerWindows,s->roi1.x,s->roi1.y,s->roi1.width,s->roi1.height,s->roi2.x,s->roi2.y,s->roi2.width,s->roi2.height,s->disp12MaxDiff,s->preFilteredImg0,s->preFilteredImg1,s->slidingSumBuf,s->cost,s->disp);
  return 1;
}

static int CvStereoBMState_gc(lua_State *L)
{
  luacv_obj<CvStereoBMState>*obj=(luacv_obj<CvStereoBMState>*)luaL_checkudata(L,1,CVSTEREOBMSTATE_NAME);
  if (obj->dealloc_data)
    cvReleaseStereoBMState(&(obj->data)); 

  return 0;
}

static int CvStereoBMState_npreFilterType(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".preFilterType=int";
  checkCvStereoBMState(L,1)->preFilterType=checkint(L,3);
return 0;
}

static int CvStereoBMState_preFilterType(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->preFilterType);
return 1;
}

static int CvStereoBMState_npreFilterSize(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".preFilterSize=int";
  checkCvStereoBMState(L,1)->preFilterSize=checkint(L,3);
return 0;
}

static int CvStereoBMState_preFilterSize(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->preFilterSize);
return 1;
}

static int CvStereoBMState_npreFilterCap(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".preFilterCap=int";
  checkCvStereoBMState(L,1)->preFilterCap=checkint(L,3);
return 0;
}

static int CvStereoBMState_preFilterCap(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->preFilterCap);
return 1;
}

static int CvStereoBMState_nSADWindowSize(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".SADWindowSize=int";
  checkCvStereoBMState(L,1)->SADWindowSize=checkint(L,3);
return 0;
}

static int CvStereoBMState_SADWindowSize(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->SADWindowSize);
return 1;
}

static int CvStereoBMState_nminDisparity(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".minDisparity=int";
  checkCvStereoBMState(L,1)->minDisparity=checkint(L,3);
return 0;
}

static int CvStereoBMState_minDisparity(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->minDisparity);
return 1;
}

static int CvStereoBMState_nnumberOfDisparities(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".numberOfDisparities=int";
  checkCvStereoBMState(L,1)->numberOfDisparities=checkint(L,3);
return 0;
}

static int CvStereoBMState_numberOfDisparities(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->numberOfDisparities);
return 1;
}

static int CvStereoBMState_ntextureThreshold(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".textureThreshold=int";
  checkCvStereoBMState(L,1)->textureThreshold=checkint(L,3);
return 0;
}

static int CvStereoBMState_textureThreshold(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->textureThreshold);
return 1;
}

static int CvStereoBMState_nuniquenessRatio(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".uniquenessRatio=int";
  checkCvStereoBMState(L,1)->uniquenessRatio=checkint(L,3);
return 0;
}

static int CvStereoBMState_uniquenessRatio(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->uniquenessRatio);
return 1;
}

static int CvStereoBMState_nspeckleWindowSize(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".speckleWindowSize=int";
  checkCvStereoBMState(L,1)->speckleWindowSize=checkint(L,3);
return 0;
}

static int CvStereoBMState_speckleWindowSize(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->speckleWindowSize);
return 1;
}

static int CvStereoBMState_nspeckleRange(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".speckleRange=int";
  checkCvStereoBMState(L,1)->speckleRange=checkint(L,3);
return 0;
}

static int CvStereoBMState_speckleRange(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->speckleRange);
return 1;
}

static int CvStereoBMState_ntrySmallerWindows(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".trySmallerWindows=int";
  checkCvStereoBMState(L,1)->trySmallerWindows=checkint(L,3);
return 0;
}

static int CvStereoBMState_trySmallerWindows(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->trySmallerWindows);
return 1;
}

static int CvStereoBMState_ndisp12MaxDiff(lua_State *L)
{
  const char f_msg[]=CVSTEREOBMSTATE_NAME".disp12MaxDiff=int";
  checkCvStereoBMState(L,1)->disp12MaxDiff=checkint(L,3);
return 0;
}

static int CvStereoBMState_disp12MaxDiff(lua_State *L)
{
  lua_pushnumber(L,checkCvStereoBMState(L,1)->disp12MaxDiff);
return 1;
}



static int CvStereoBMState_nroi1(lua_State *L)
{
  checkCvStereoBMState(L,1)->roi1=*checkCvRect(L,3);
return 0;
}

static int CvStereoBMState_roi1(lua_State *L)
{
  pushCvRect(L,&checkCvStereoBMState(L,1)->roi1);
return 1;
}

static int CvStereoBMState_nroi2(lua_State *L)
{
  checkCvStereoBMState(L,1)->roi2=*checkCvRect(L,3);
return 0;
}

static int CvStereoBMState_roi2(lua_State *L)
{
  pushCvRect(L,&checkCvStereoBMState(L,1)->roi2);
return 1;
}

static int CvStereoBMState_npreFilteredImg0(lua_State *L)
{
  checkCvStereoBMState(L,1)->preFilteredImg0=checkCvMat(L,3);
return 0;
}

static int CvStereoBMState_preFilteredImg0(lua_State *L)
{
  pushUserData(L,checkCvStereoBMState(L,1)->preFilteredImg0);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static int CvStereoBMState_npreFilteredImg1(lua_State *L)
{
  checkCvStereoBMState(L,1)->preFilteredImg1=checkCvMat(L,3);
return 0;
}

static int CvStereoBMState_preFilteredImg1(lua_State *L)
{
  pushUserData(L,checkCvStereoBMState(L,1)->preFilteredImg1);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static int CvStereoBMState_nslidingSumBuf(lua_State *L)
{
  checkCvStereoBMState(L,1)->slidingSumBuf=checkCvMat(L,3);
return 0;
}
static int CvStereoBMState_slidingSumBuf(lua_State *L)
{
  pushUserData(L,checkCvStereoBMState(L,1)->slidingSumBuf);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static int CvStereoBMState_ncost(lua_State *L)
{
  checkCvStereoBMState(L,1)->cost=checkCvMat(L,3);
return 0;
}

static int CvStereoBMState_cost(lua_State *L)
{
  pushUserData(L,checkCvStereoBMState(L,1)->cost);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}

static int CvStereoBMState_ndisp(lua_State *L)
{
  checkCvStereoBMState(L,1)->disp=checkCvMat(L,3);
return 0;
}

static int CvStereoBMState_disp(lua_State *L)
{
  pushUserData(L,checkCvStereoBMState(L,1)->disp);
  luaL_getmetatable(L,CVMAT_NAME);
  lua_setmetatable(L,-2);
return 1;
}
static const luacv_method CvStereoBMState_v[]=
{
  {NULL,20,NULL,NULL},
  methodReg(SADWindowSize,CvStereoBMState),
  methodReg(cost,CvStereoBMState),
  methodReg(disp,CvStereoBMState),
  methodReg(disp12MaxDiff,CvStereoBMState),
  methodReg(minDisparity,CvStereoBMState),
  methodReg(numberOfDisparities,CvStereoBMState),
  methodReg(preFilterType,CvStereoBMState),
  methodReg(preFilterSize,CvStereoBMState),
  methodReg(preFilterCap,CvStereoBMState),
  methodReg(preFilteredImg0,CvStereoBMState),
  methodReg(preFilteredImg1,CvStereoBMState),
  methodReg(roi1,CvStereoBMState),
  methodReg(roi2,CvStereoBMState),
  methodReg(speckleWindowSize,CvStereoBMState),
  methodReg(speckleRange,CvStereoBMState),
  methodReg(slidingSumBuf,CvStereoBMState),
  methodReg(textureThreshold,CvStereoBMState),
  methodReg(trySmallerWindows,CvStereoBMState),
  methodReg(uniquenessRatio,CvStereoBMState),
};

makeIndexFunctions(CvStereoBMState)
makeObjectCallback(CvStereoBMState);
