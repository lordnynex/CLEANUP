#include "CvPoint.h"

static int CvPoint_tostring(lua_State *L)
{
  CvPoint *p=checkCvPoint(L,1);
  lua_pushfstring(L,CVPOINT_NAME" object: %p\n\tx=%d\n\ty=%d",p,p->x,p->y);
  return 1;
}

static int CvPoint_gc(lua_State *L)
{
  luacv_obj<CvPoint>*obj=(luacv_obj<CvPoint>*)luaL_checkudata(L,1,CVPOINT_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 

  return 0;
}

static int CvPoint_nx(lua_State *L)
{
  const char f_msg[]=CVPOINT_NAME".x=int";
  checkCvPoint(L,1)->x=checkint(L,3);
return 0;
}

static int CvPoint_x(lua_State *L)
{
  lua_pushnumber(L,checkCvPoint(L,1)->x);
return 1;
}

static int CvPoint_ny(lua_State *L)
{
  const char f_msg[]=CVPOINT_NAME".y=int";
  checkCvPoint(L,1)->y=checkint(L,3);
return 0;
}

static int CvPoint_y(lua_State *L)
{
  lua_pushnumber(L,checkCvPoint(L,1)->y);
return 1;
}



static const luacv_method CvPoint_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(x,CvPoint),
  methodReg(y,CvPoint),
};

makeIndexFunctions(CvPoint)
makeObjectCallback(CvPoint);
