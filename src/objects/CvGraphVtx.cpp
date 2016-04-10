#include "CvGraphVtx.h"

static int CvGraphVtx_tostring(lua_State *L)
{
  CvGraphVtx *vtx=checkCvGraphVtx(L,1);
  lua_pushfstring(L,CVGRAPHVTX_NAME" object %p\n\tflags=%d\n\tfirst="CVGRAPHEDGE_NAME,vtx,vtx->flags);

  return 1;
}
static int CvGraphVtx_gc(lua_State *L)
{

  luacv_obj<CvGraphVtx>*obj=(luacv_obj<CvGraphVtx>*)luaL_checkudata(L,1,CVGRAPHVTX_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}
static int CvGraphVtx_nflags(lua_State *L)
{
  const char f_msg[]=CVGRAPHVTX_NAME".flags=int";
  checkCvGraphVtx(L,1)->flags=checkint(L,3);
return 0;
}

static int CvGraphVtx_flags(lua_State *L)
{
  lua_pushnumber(L,checkCvGraphVtx(L,1)->flags);
return 1;
}

static int CvGraphVtx_nfirst(lua_State *L)
{
  checkCvGraphVtx(L,1)->first=checkCvGraphEdge(L,3);
return 0;
}

static int CvGraphVtx_first(lua_State *L)
{
  pushCvGraphEdge(L,checkCvGraphVtx(L,1)->first);
return 1;
}

static const luacv_method CvGraphVtx_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(flags,CvGraphVtx),
  methodReg(first,CvGraphVtx),
};

makeIndexFunctions(CvGraphVtx)
makeObjectCallback(CvGraphVtx);
