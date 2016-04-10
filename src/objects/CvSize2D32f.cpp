#include "CvSize2D32f.h"

static int CvSize2D32f_tostring(lua_State *L)
{
  CvSize2D32f *size=checkCvSize2D32f(L,1);
  lua_pushfstring(L,CVSIZE2D32F_NAME" object %p\n\twidth=%d\n\theight=%d",size,size->width,size->height);

  return 1;
}

static int CvSize2D32f_gc(lua_State *L)
{

  luacv_obj<CvSize2D32f>*obj=(luacv_obj<CvSize2D32f>*)luaL_checkudata(L,1,CVSIZE2D32F_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static inline int CvSize2D32f_nwidth(lua_State *L)
{
    const char f_msg[]=CVSIZE2D32F_NAME".width=num";
    checkCvSize2D32f(L,1)->width=(float)checknumber(L,3);
    return 0;
}

static inline int CvSize2D32f_nheight(lua_State *L)
{
    const char f_msg[]=CVSIZE2D32F_NAME".height=num";
    checkCvSize2D32f(L,1)->height=(float)checknumber(L,3);
    return 0;
}

static inline int CvSize2D32f_width(lua_State *L)
{
    lua_pushnumber(L,checkCvSize2D32f(L,1)->width);
    return 1;
}

static inline int CvSize2D32f_height(lua_State *L)
{
    lua_pushnumber(L,checkCvSize2D32f(L,1)->height);
    return 1;
}

static const luacv_method CvSize2D32f_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(height,CvSize2D32f),
  methodReg(width,CvSize2D32f),
};

makeIndexFunctions(CvSize2D32f)
makeObjectCallback(CvSize2D32f);

