#include "CvPoint2D64f.h"

static int CvPoint2D64f_tostring(lua_State *L)
{
  CvPoint2D64f *p=checkCvPoint2D64f(L,1);
  lua_pushfstring(L,CVPOINT2D64F_NAME" object: %p\n\tx=%f\n\ty=%f",p,p->x,p->y);
  return 1;
}

static int CvPoint2D64f_gc(lua_State *L)
{

  luacv_obj<CvPoint2D64f>*obj=(luacv_obj<CvPoint2D64f>*)luaL_checkudata(L,1,CVPOINT2D64F_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvPoint2D64f_nx(lua_State *L)
{
  const char f_msg[]=CVPOINT2D64F_NAME".x=num";
  checkCvPoint2D64f(L,1)->x=checknumber(L,3);
return 0;
}

static int CvPoint2D64f_x(lua_State *L)
{
  lua_pushnumber(L,checkCvPoint2D64f(L,1)->x);
return 1;
}

static int CvPoint2D64f_ny(lua_State *L)
{
  const char f_msg[]=CVPOINT2D64F_NAME".y=num";
  checkCvPoint2D64f(L,1)->y=checknumber(L,3);
return 0;
}

static int CvPoint2D64f_y(lua_State *L)
{
  lua_pushnumber(L,checkCvPoint2D64f(L,1)->y);
return 1;
}



static const luacv_method CvPoint2D64f_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(x,CvPoint2D64f),
  methodReg(y,CvPoint2D64f),
};

makeIndexFunctions(CvPoint2D64f)
makeObjectCallback(CvPoint2D64f);
