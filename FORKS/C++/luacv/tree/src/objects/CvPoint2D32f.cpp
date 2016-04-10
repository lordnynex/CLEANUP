#include "CvPoint2D32f.h"

static int CvPoint2D32f_tostring(lua_State *L)
{
  CvPoint2D32f *p=checkCvPoint2D32f(L,1);
  lua_pushfstring(L,CVPOINT2D32F_NAME" object: %p\n\tx=%f\n\ty=%f",p,p->x,p->y);
  return 1;
}

static int CvPoint2D32f_gc(lua_State *L)
{

  luacv_obj<CvPoint2D32f>*obj=(luacv_obj<CvPoint2D32f>*)luaL_checkudata(L,1,CVPOINT2D32F_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvPoint2D32f_nx(lua_State *L)
{
  const char f_msg[]=CVPOINT2D32F_NAME".x=num";
  checkCvPoint2D32f(L,1)->x=(float)checknumber(L,3);
return 0;
}

static int CvPoint2D32f_x(lua_State *L)
{
  lua_pushnumber(L,checkCvPoint2D32f(L,1)->x);
return 1;
}

static int CvPoint2D32f_ny(lua_State *L)
{
  const char f_msg[]=CVPOINT2D32F_NAME".y=num";
  checkCvPoint2D32f(L,1)->y=(float)checknumber(L,3);
return 0;
}

static int CvPoint2D32f_y(lua_State *L)
{
  lua_pushnumber(L,checkCvPoint2D32f(L,1)->y);
return 1;
}

static const luacv_method CvPoint2D32f_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(x,CvPoint2D32f),
  methodReg(y,CvPoint2D32f),
};

makeIndexFunctions(CvPoint2D32f)
makeObjectCallback(CvPoint2D32f);
