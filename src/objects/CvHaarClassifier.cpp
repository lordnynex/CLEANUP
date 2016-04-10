#include "CvHaarClassifier.h"

static int CvHaarClassifier_tostring(lua_State *L)
{
  CvHaarClassifier *h=checkCvHaarClassifier(L,1);
  lua_pushfstring(L,CVHAARCLASSIFIER_NAME" object: %p\n\tcount=%d\n\thaar_feature="CVHAARFEATURE_NAME"\n\tthreshold[]=num\n\tleft[]=num\n\tright[]=num\n\talpha[]=num",h,h->count);
  return 1;
}

static int CvHaarClassifier_gc(lua_State *L)
{
  luacv_obj<CvHaarClassifier>*obj=(luacv_obj<CvHaarClassifier>*)luaL_checkudata(L,1,CVHAARCLASSIFIER_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvHaarClassifier_ncount(lua_State *L)
{
  const char f_msg[]=CVHAARCLASSIFIER_NAME".count=int";
  checkCvHaarClassifier(L,1)->count=checkint(L,3);
return 0;
}

static int CvHaarClassifier_count(lua_State *L)
{
  lua_pushnumber(L,checkCvHaarClassifier(L,1)->count);
return 1;
}

static int CvHaarClassifier_nhaar_feature(lua_State *L)
{
  checkCvHaarClassifier(L,1)->haar_feature=checkCvHaarFeature(L,3);
return 0;
}

static int CvHaarClassifier_haar_feature(lua_State *L)
{
  pushCvHaarFeature(L,checkCvHaarClassifier(L,1)->haar_feature);
return 1;
}

static int CvHaarClassifier_nthreshold(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvHaarClassifier_threshold(lua_State *L)
{
  UNIMPL(L);
return 1;
}
static int CvHaarClassifier_nleft(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvHaarClassifier_left(lua_State *L)
{
  UNIMPL(L);
return 1;
}
static int CvHaarClassifier_nright(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvHaarClassifier_right(lua_State *L)
{
  UNIMPL(L);
return 1;
}


static int CvHaarClassifier_nalpha(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvHaarClassifier_alpha(lua_State *L)
{
  UNIMPL(L);
return 1;
}

static const luacv_method CvHaarClassifier_v[]=
{
  {NULL,7,NULL,NULL},
  methodReg(alpha,CvHaarClassifier),
  methodReg(count,CvHaarClassifier),
  methodReg(haar_feature,CvHaarClassifier),
  methodReg(left,CvHaarClassifier),
  methodReg(right,CvHaarClassifier),
  methodReg(threshold,CvHaarClassifier),
};

makeIndexFunctions(CvHaarClassifier)
makeObjectCallback(CvHaarClassifier);
