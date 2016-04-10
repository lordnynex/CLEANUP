#include "IplConvKernel.h"

static int IplConvKernel_tostring(lua_State *L)
{
  IplConvKernel *kernel=checkIplConvKernel(L,1);
  lua_pushfstring(L,IPLCONVKERNEL_NAME" object %p\n\tnCols=%d\n\tnRows=%d\n\tanchorX=%d\n\tanchorY=%d\n\tnShiftR=%d",kernel,kernel->nCols,kernel->nRows,kernel->anchorX,kernel->anchorY,kernel->nShiftR);

  return 1;
}

static int IplConvKernel_gc(lua_State *L)
{

  luacv_obj<IplConvKernel>*obj=(luacv_obj<IplConvKernel>*)luaL_checkudata(L,1,IPLCONVKERNEL_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}
static int IplConvKernel_nnCols(lua_State *L)
{
  const char f_msg[]=IPLCONVKERNEL_NAME".nCols=int";
  checkIplConvKernel(L,1)->nCols=checkint(L,3);
return 0;
}

static int IplConvKernel_nCols(lua_State *L)
{
  lua_pushnumber(L,checkIplConvKernel(L,1)->nCols);
return 1;
}


static int IplConvKernel_nnRows(lua_State *L)
{
  const char f_msg[]=IPLCONVKERNEL_NAME".nRows=int";
  checkIplConvKernel(L,1)->nRows=checkint(L,3);
return 0;
}

static int IplConvKernel_nRows(lua_State *L)
{
  lua_pushnumber(L,checkIplConvKernel(L,1)->nRows);
return 1;
}


static int IplConvKernel_nanchorX(lua_State *L)
{
  const char f_msg[]=IPLCONVKERNEL_NAME".anchorX=int";
  checkIplConvKernel(L,1)->anchorX=checkint(L,3);
return 0;
}

static int IplConvKernel_anchorX(lua_State *L)
{
  lua_pushnumber(L,checkIplConvKernel(L,1)->anchorX);
return 1;
}


static int IplConvKernel_nanchorY(lua_State *L)
{
  const char f_msg[]=IPLCONVKERNEL_NAME".anchorY=int";
  checkIplConvKernel(L,1)->anchorY=checkint(L,3);
return 0;
}

static int IplConvKernel_anchorY(lua_State *L)
{
  lua_pushnumber(L,checkIplConvKernel(L,1)->anchorY);
return 1;
}


static int IplConvKernel_nnShiftR(lua_State *L)
{
  const char f_msg[]=IPLCONVKERNEL_NAME".nShiftR=int";
  checkIplConvKernel(L,1)->nShiftR=checkint(L,3);
return 0;
}

static int IplConvKernel_nShiftR(lua_State *L)
{
  lua_pushnumber(L,checkIplConvKernel(L,1)->nShiftR);
return 1;
}


static int IplConvKernel_nvalues(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int IplConvKernel_values(lua_State *L)
{
  UNIMPL(L);
return 1;
}

static const luacv_method IplConvKernel_v[]=
{
  {NULL,7,NULL,NULL},
  methodReg(anchorX,IplConvKernel),
  methodReg(anchorY,IplConvKernel),
  methodReg(nCols,IplConvKernel),
  methodReg(nRows,IplConvKernel),
  methodReg(nShiftR,IplConvKernel),
  methodReg(values,IplConvKernel),
};

makeIndexFunctions(IplConvKernel)
makeObjectCallback(IplConvKernel);
