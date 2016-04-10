#include "CvHaarFeature.h"

static int CvHaarFeature_tostring(lua_State *L)
{
  CvHaarFeature *h=checkCvHaarFeature(L,1);
  lua_pushfstring(L,CVHAARFEATURE_NAME" object: %p\n\ttilted=%d\n\trect[%d]=[ r=>"CVRECT_NAME", weight=num ]",h,h->tilted,CV_HAAR_FEATURE_MAX);
  return 1;
}

static int CvHaarFeature_gc(lua_State *L)
{
  luacv_obj<CvHaarFeature>*obj=(luacv_obj<CvHaarFeature>*)luaL_checkudata(L,1,CVHAARFEATURE_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvHaarFeature_ntilted(lua_State *L)
{
  const char f_msg[]=CVHAARFEATURE_NAME".tilted=int";
  checkCvHaarFeature(L,1)->tilted=checkint(L,3);
return 0;
}

static int CvHaarFeature_tilted(lua_State *L)
{
  lua_pushnumber(L,checkCvHaarFeature(L,1)->tilted);
return 1;
}

static int CvHaarFeature_nrect(lua_State *L)
{
  if ((!lua_istable(L,3))|| (lua_objlen(L,3)>CV_HAAR_FEATURE_MAX))luaL_error(L,CVHAARFEATURE_NAME"->rect must be table of max %d elements indexed from 0.",CV_HAAR_FEATURE_MAX);
  CvHaarFeature *f=checkCvHaarFeature(L,1);
  size_t len=lua_objlen(L,3);
  const char f_msg[]=CVHAARFEATURE_NAME"->rect[].weight must be float!";
  for(size_t i=0;i<len;i++)
  {
    lua_rawgeti(L,3,i);
    if (!lua_istable(L,4)) luaL_error(L,"There is no 2D array that can be converted to "CVHAARFEATURE_NAME"->rect");
    lua_getfield(L,4,"r");
    f->rect[i].r=*checkCvRect(L,5);
    lua_pop(L,1);
    lua_getfield(L,4,"weight");
    f->rect[i].weight=(float)checknumber(L,5);
    lua_pop(L,2);
  }

return 0;
}

static int CvHaarFeature_rect(lua_State *L)
{
  CvHaarFeature *f=checkCvHaarFeature(L,1);
  lua_newtable(L);
  for(size_t i=0;i<CV_HAAR_FEATURE_MAX;i++) 
  {
    lua_newtable(L);
    pushCvRect(L,&f->rect[i].r);
    lua_setfield(L,-2,"r");
    lua_rawseti(L,-1,(int)f->rect[i].weight);
  }
return 1;
}

static const luacv_method CvHaarFeature_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(rect,CvHaarFeature),
  methodReg(tilted,CvHaarFeature),
};

makeIndexFunctions(CvHaarFeature)
makeObjectCallback(CvHaarFeature);
