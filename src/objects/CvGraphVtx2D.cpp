#include "CvGraphVtx2D.h"

static int CvGraphVtx2D_tostring(lua_State *L)
{
  CvGraphVtx2D *vtx=checkCvGraphVtx2D(L,1);
  lua_pushfstring(L,CVGRAPHVTX2D_NAME" object %p\n\tflags=%d\n\tfirst="CVGRAPHEDGE_NAME"\n\tptr="CVPOINT2D32F_NAME,vtx,vtx->flags,vtx->ptr);

  return 1;
}
static int CvGraphVtx2D_gc(lua_State *L)
{

  luacv_obj<CvGraphVtx2D>*obj=(luacv_obj<CvGraphVtx2D>*)luaL_checkudata(L,1,CVGRAPHVTX2D_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}



static int CvGraphVtx2D_nflags(lua_State *L)
{
  const char f_msg[]=CVGRAPHVTX2D_NAME".flags=int";
  checkCvGraphVtx2D(L,1)->flags=checkint(L,3);
return 0;
}

static int CvGraphVtx2D_flags(lua_State *L)
{
  lua_pushnumber(L,checkCvGraphVtx2D(L,1)->flags);
return 1;
}

static int CvGraphVtx2D_nfirst(lua_State *L)
{
  checkCvGraphVtx2D(L,1)->first=checkCvGraphEdge(L,3);
return 0;
}

static int CvGraphVtx2D_first(lua_State *L)
{
  pushCvGraphEdge(L,checkCvGraphVtx2D(L,1)->first);
return 1;
}

static int CvGraphVtx2D_nptr(lua_State *L)
{
  checkCvGraphVtx2D(L,1)->ptr=checkCvPoint2D32f(L,3);
return 0;
}

static int CvGraphVtx2D_ptr(lua_State *L)
{
  pushCvPoint2D32f(L,checkCvGraphVtx2D(L,1)->ptr);
return 1;
}

static const luacv_method CvGraphVtx2D_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(flags,CvGraphVtx2D),
  methodReg(first,CvGraphVtx2D),
  methodReg(ptr,CvGraphVtx2D),
};

makeIndexFunctions(CvGraphVtx2D)
makeObjectCallback(CvGraphVtx2D);
