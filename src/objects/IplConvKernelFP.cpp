#include "IplConvKernelFP.h"

static int IplConvKernelFP_tostring(lua_State *L)
{
  IplConvKernelFP *kernel=checkIplConvKernelFP(L,1);
  lua_pushfstring(L,IPLCONVKERNELFP_NAME" object %p\n\tnCols=%d\n\tnRows=%d\n\tanchorX=%d\n\tanchorY=%d",kernel,kernel->nCols,kernel->nRows,kernel->anchorX,kernel->anchorY);

  return 1;
}

static int IplConvKernelFP_gc(lua_State *L)
{

  luacv_obj<IplConvKernelFP>*obj=(luacv_obj<IplConvKernelFP>*)luaL_checkudata(L,1,IPLCONVKERNELFP_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}


static int IplConvKernelFP_nnCols(lua_State *L)
{
  const char f_msg[]=IPLCONVKERNELFP_NAME".nCols=int";
  checkIplConvKernelFP(L,1)->nCols=checkint(L,3);
return 0;
}

static int IplConvKernelFP_nCols(lua_State *L)
{
  lua_pushnumber(L,checkIplConvKernelFP(L,1)->nCols);
return 1;
}


static int IplConvKernelFP_nnRows(lua_State *L)
{
  const char f_msg[]=IPLCONVKERNELFP_NAME".nRows=int";
  checkIplConvKernelFP(L,1)->nRows=checkint(L,3);
return 0;
}

static int IplConvKernelFP_nRows(lua_State *L)
{
  lua_pushnumber(L,checkIplConvKernelFP(L,1)->nRows);
return 1;
}


static int IplConvKernelFP_nanchorX(lua_State *L)
{
  const char f_msg[]=IPLCONVKERNELFP_NAME".anchorX=int";
  checkIplConvKernelFP(L,1)->anchorX=checkint(L,3);
return 0;
}

static int IplConvKernelFP_anchorX(lua_State *L)
{
  lua_pushnumber(L,checkIplConvKernelFP(L,1)->anchorX);
return 1;
}


static int IplConvKernelFP_nanchorY(lua_State *L)
{
  const char f_msg[]=IPLCONVKERNELFP_NAME".anchorY=int";
  checkIplConvKernelFP(L,1)->anchorY=checkint(L,3);
return 0;
}

static int IplConvKernelFP_anchorY(lua_State *L)
{
  lua_pushnumber(L,checkIplConvKernelFP(L,1)->anchorY);
return 1;
}


static int IplConvKernelFP_nvalues(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int IplConvKernelFP_values(lua_State *L)
{
  UNIMPL(L);
return 1;
}

static const luacv_method IplConvKernelFP_v[]=
{
  {NULL,6,NULL,NULL},
  methodReg(anchorX,IplConvKernelFP),
  methodReg(anchorY,IplConvKernelFP),
  methodReg(nCols,IplConvKernelFP),
  methodReg(nRows,IplConvKernelFP),
  methodReg(values,IplConvKernelFP),
};

makeIndexFunctions(IplConvKernelFP)
makeObjectCallback(IplConvKernelFP);
