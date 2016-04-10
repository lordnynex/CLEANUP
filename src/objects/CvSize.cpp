#include "CvSize.h"


static int CvSize_tostring(lua_State *L)
{
  CvSize *size=checkCvSize(L,1);
  lua_pushfstring(L,CVSIZE_NAME" object %p\n\tint width=%d\n\tint height=%d",size,size->width,size->height);
  return 1;
}

static int CvSize_gc(lua_State *L)
{

  luacv_obj<CvSize>*obj=(luacv_obj<CvSize>*)luaL_checkudata(L,1,CVSIZE_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static inline int CvSize_nwidth(lua_State *L)
{
    const char f_msg[]=CVSIZE_NAME".width=int";
    checkCvSize(L,1)->width=checkint(L,3);
    return 0;
}

static inline int CvSize_nheight(lua_State *L)
{
    const char f_msg[]=CVSIZE_NAME".height=int";
    checkCvSize(L,1)->height=checkint(L,3);
    return 0;
}

static inline int CvSize_width(lua_State *L)
{
    lua_pushnumber(L,checkCvSize(L,1)->width);
    return 1;
}

static inline int CvSize_height(lua_State *L)
{
    lua_pushnumber(L,checkCvSize(L,1)->height);
    return 1;
}

static const luacv_method CvSize_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(height,CvSize),
  methodReg(width,CvSize),
};

makeIndexFunctions(CvSize)
makeObjectCallback(CvSize);

