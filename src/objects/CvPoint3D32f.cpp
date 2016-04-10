#include "CvPoint3D32f.h"

static int CvPoint3D32f_tostring(lua_State *L)
{
  CvPoint3D32f *p=checkCvPoint3D32f(L,1);
  lua_pushfstring(L,CVPOINT3D32F_NAME" object: %p\n\tx=%f\n\ty=%f\n\tz=%f",p,p->x,p->y,p->z);
  return 1;
}
static int CvPoint3D32f_gc(lua_State *L)
{

  luacv_obj<CvPoint3D32f>*obj=(luacv_obj<CvPoint3D32f>*)luaL_checkudata(L,1,CVPOINT3D32F_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvPoint3D32f_nx(lua_State *L)
{
  const char f_msg[]=CVPOINT3D32F_NAME".x=num";
  checkCvPoint3D32f(L,1)->x=(float)checknumber(L,3);
return 0;
}

static int CvPoint3D32f_x(lua_State *L)
{
  lua_pushnumber(L,checkCvPoint3D32f(L,1)->x);
return 1;
}

static int CvPoint3D32f_ny(lua_State *L)
{
  const char f_msg[]=CVPOINT3D32F_NAME".y=num";
  checkCvPoint3D32f(L,1)->y=(float)checknumber(L,3);
return 0;
}

static int CvPoint3D32f_y(lua_State *L)
{
  lua_pushnumber(L,checkCvPoint3D32f(L,1)->y);
return 1;
}

static int CvPoint3D32f_nz(lua_State *L)
{
  const char f_msg[]=CVPOINT3D32F_NAME".z=num";
  checkCvPoint3D32f(L,1)->z=(float)checknumber(L,3);
return 0;
}

static int CvPoint3D32f_z(lua_State *L)
{
  lua_pushnumber(L,checkCvPoint3D32f(L,1)->z);
return 1;
}


static const luacv_method CvPoint3D32f_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(x,CvPoint3D32f),
  methodReg(y,CvPoint3D32f),
  methodReg(z,CvPoint3D32f),

};

makeIndexFunctions(CvPoint3D32f)
makeObjectCallback(CvPoint3D32f);
