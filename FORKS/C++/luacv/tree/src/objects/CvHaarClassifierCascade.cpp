#include "CvHaarClassifierCascade.h"
static int CvHaarClassifierCascade_tostring(lua_State *L)
{
  CvHaarClassifierCascade *p=checkCvHaarClassifierCascade(L,1);
  lua_pushfstring(L,CVHAARCLASSIFIERCASCADE_NAME" object: %p\n\tflags=%d\n\tcount=%d\n\torig_window_size="CVSIZE_NAME"\n\treal_window_size="CVSIZE_NAME"\n\tscale=%f\n\tstage_classifier="CVHAARSTAGECLASSIFIER_NAME,p,p->flags,p->count,p->scale);
  return 1;
}

static int CvHaarClassifierCascade_gc(lua_State *L)
{
  luacv_obj<CvHaarClassifierCascade>*obj=(luacv_obj<CvHaarClassifierCascade>*)luaL_checkudata(L,1,CVHAARCLASSIFIERCASCADE_NAME);
  if (obj->dealloc_data)
    cvReleaseHaarClassifierCascade(&obj->data);
  return 0;
}

static int CvHaarClassifierCascade_nflags(lua_State *L)
{
  const char f_msg[]=CVHAARCLASSIFIERCASCADE_NAME".flags=int";
  checkCvHaarClassifierCascade(L,1)->flags=checkint(L,3);
return 0;
}

static int CvHaarClassifierCascade_flags(lua_State *L)
{
  lua_pushnumber(L,checkCvHaarClassifierCascade(L,1)->flags);
return 1;
}

static int CvHaarClassifierCascade_ncount(lua_State *L)
{
  const char f_msg[]=CVHAARCLASSIFIERCASCADE_NAME".count=int";
  checkCvHaarClassifierCascade(L,1)->count=checkint(L,3);
return 0;
}

static int CvHaarClassifierCascade_count(lua_State *L)
{
  lua_pushnumber(L,checkCvHaarClassifierCascade(L,1)->count);
return 1;
}
static int CvHaarClassifierCascade_nscale(lua_State *L)
{
  const char f_msg[]=CVHAARCLASSIFIERCASCADE_NAME".scale=num";
  checkCvHaarClassifierCascade(L,1)->scale=checknumber(L,3);
return 0;
}

static int CvHaarClassifierCascade_scale(lua_State *L)
{
  lua_pushnumber(L,checkCvHaarClassifierCascade(L,1)->scale);
return 1;
}

static int CvHaarClassifierCascade_norig_window_size(lua_State *L)
{
  checkCvHaarClassifierCascade(L,1)->orig_window_size=*checkCvSize(L,3);
return 0;
}

static int CvHaarClassifierCascade_orig_window_size(lua_State *L)
{
  pushCvSize(L,&checkCvHaarClassifierCascade(L,1)->orig_window_size);
return 1;
}

static int CvHaarClassifierCascade_nreal_window_size(lua_State *L)
{
  checkCvHaarClassifierCascade(L,1)->real_window_size=*checkCvSize(L,3);
return 0;
}

static int CvHaarClassifierCascade_real_window_size(lua_State *L)
{
  pushCvSize(L,&checkCvHaarClassifierCascade(L,1)->real_window_size);
return 1;
}


static int CvHaarClassifierCascade_nstage_classifier(lua_State *L)
{
  checkCvHaarClassifierCascade(L,1)->stage_classifier=checkCvHaarStageClassifier(L,3);
return 0;
}

static int CvHaarClassifierCascade_stage_classifier(lua_State *L)
{
  pushCvHaarStageClassifier(L,checkCvHaarClassifierCascade(L,1)->stage_classifier);
return 1;
}


static const luacv_method CvHaarClassifierCascade_v[]=
{
  {NULL,7,NULL,NULL},
  methodReg(count,CvHaarClassifierCascade),
  methodReg(flags,CvHaarClassifierCascade),
  methodReg(orig_window_size,CvHaarClassifierCascade),
  methodReg(real_window_size,CvHaarClassifierCascade),
  methodReg(scale,CvHaarClassifierCascade),
  methodReg(stage_classifier,CvHaarClassifierCascade),
};

makeIndexFunctions(CvHaarClassifierCascade)
makeObjectCallback(CvHaarClassifierCascade);
