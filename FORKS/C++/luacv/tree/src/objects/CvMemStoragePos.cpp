#include "CvMemStoragePos.h"

static int CvMemStoragePos_tostring(lua_State *L)
{
  CvMemStoragePos *pos=checkCvMemStoragePos(L,1);
  lua_pushfstring(L,CVMEMSTORAGEPOS_NAME" object %p\n\ttop="CVMEMBLOCK_NAME"\n\tfree_space=%d",pos,pos->free_space);

  return 1;
}

static int CvMemStoragePos_gc(lua_State *L)
{

  luacv_obj<CvMemStoragePos>*obj=(luacv_obj<CvMemStoragePos>*)luaL_checkudata(L,1,CVMEMSTORAGEPOS_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvMemStoragePos_nfree_space(lua_State *L)
{
  const char f_msg[]=CVMEMSTORAGEPOS_NAME".free_space=int";
  checkCvMemStoragePos(L,1)->free_space=checkint(L,3);
return 0;
}

static int CvMemStoragePos_free_space(lua_State *L)
{
  lua_pushnumber(L,checkCvMemStoragePos(L,1)->free_space);
return 1;
}

static int CvMemStoragePos_ntop(lua_State *L)
{
  checkCvMemStoragePos(L,1)->top=checkCvMemBlock(L,3);
return 0;
}

static int CvMemStoragePos_top(lua_State *L)
{
  pushCvMemBlock(L,checkCvMemStoragePos(L,1)->top);
return 1;
}

static const luacv_method CvMemStoragePos_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(free_space,CvMemStoragePos),
  methodReg(top,CvMemStoragePos),
};

makeIndexFunctions(CvMemStoragePos)
makeObjectCallback(CvMemStoragePos);

