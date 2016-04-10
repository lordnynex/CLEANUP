#include "CvLSVMFilterPosition.h"

static int CvLSVMFilterPosition_tostring(lua_State *L)
{
  CvLSVMFilterPosition *p=checkCvLSVMFilterPosition(L,1);
  lua_pushfstring(L,CVLSVMFILTERPOSITION_NAME" object: %p\n\tx=%d\n\ty=%d\n\tl=%d",p,p->x,p->y,p->l);
  return 1;
}

static int CvLSVMFilterPosition_gc(lua_State *L)
{
  luacv_obj<CvLSVMFilterPosition>*obj=(luacv_obj<CvLSVMFilterPosition>*)luaL_checkudata(L,1,CVLSVMFILTERPOSITION_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 

  return 0;
}

static int CvLSVMFilterPosition_nx(lua_State *L)
{
  const char f_msg[]=CVLSVMFILTERPOSITION_NAME".x=int";
  checkCvLSVMFilterPosition(L,1)->x=checkint(L,3);
return 0;
}

static int CvLSVMFilterPosition_x(lua_State *L)
{
  lua_pushnumber(L,checkCvLSVMFilterPosition(L,1)->x);
return 1;
}

static int CvLSVMFilterPosition_ny(lua_State *L)
{
  const char f_msg[]=CVLSVMFILTERPOSITION_NAME".y=int";
  checkCvLSVMFilterPosition(L,1)->y=checkint(L,3);
return 0;
}

static int CvLSVMFilterPosition_y(lua_State *L)
{
  lua_pushnumber(L,checkCvLSVMFilterPosition(L,1)->y);
return 1;
}

static int CvLSVMFilterPosition_nl(lua_State *L)
{
  const char f_msg[]=CVLSVMFILTERPOSITION_NAME".l=int";
  checkCvLSVMFilterPosition(L,1)->l=checkint(L,3);
return 0;
}

static int CvLSVMFilterPosition_l(lua_State *L)
{
  lua_pushnumber(L,checkCvLSVMFilterPosition(L,1)->l);
return 1;
}

static const luacv_method CvLSVMFilterPosition_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(l,CvLSVMFilterPosition),
  methodReg(x,CvLSVMFilterPosition),
  methodReg(y,CvLSVMFilterPosition),
};

makeIndexFunctions(CvLSVMFilterPosition)
makeObjectCallback(CvLSVMFilterPosition);
