#include "CvMemStorage.h"

static int CvMemStorage_tostring(lua_State *L)
{
  CvMemStorage *mem=checkCvMemStorage(L,1);
  lua_pushfstring(L,CVMEMSTORAGE_NAME" object %p\n\tsignature=%d\n\tbottom="CVMEMBLOCK_NAME"\n\ttop="CVMEMBLOCK_NAME"\n\tparent="CVMEMSTORAGE_NAME"\n\tblock_size=%d\n\tfree_space=%d",mem,mem->signature,mem->block_size,mem->free_space);
return 1;
}

static int CvMemStorage_gc(lua_State *L)
{

  luacv_obj<CvMemStorage>*obj=(luacv_obj<CvMemStorage>*)luaL_checkudata(L,1,CVMEMSTORAGE_NAME);
  if (obj->dealloc_data)
    cvReleaseMemStorage(&obj->data);
  return 0;
}

static int CvMemStorage_nsignature(lua_State *L)
{
  const char f_msg[]=CVMEMSTORAGE_NAME".signature=int";
  checkCvMemStorage(L,1)->signature=checkint(L,3);
return 0;
}

static int CvMemStorage_signature(lua_State *L)
{
  lua_pushnumber(L,checkCvMemStorage(L,1)->signature);
return 1;
}

static int CvMemStorage_nblock_size(lua_State *L)
{
  const char f_msg[]=CVMEMSTORAGE_NAME".block_size=int";
  checkCvMemStorage(L,1)->block_size=checkint(L,3);
return 0;
}

static int CvMemStorage_block_size(lua_State *L)
{
  lua_pushnumber(L,checkCvMemStorage(L,1)->block_size);
return 1;
}
static int CvMemStorage_nfree_space(lua_State *L)
{
  const char f_msg[]=CVMEMSTORAGE_NAME".free_space=int";
  checkCvMemStorage(L,1)->free_space=checkint(L,3);
return 0;
}

static int CvMemStorage_free_space(lua_State *L)
{
  lua_pushnumber(L,checkCvMemStorage(L,1)->free_space);
return 1;
}

static int CvMemStorage_nbottom(lua_State *L)
{
  checkCvMemStorage(L,1)->bottom=checkCvMemBlock(L,3);
return 0;
}

static int CvMemStorage_bottom(lua_State *L)
{
  pushCvMemBlock(L,checkCvMemStorage(L,1)->bottom);
return 1;
}
static int CvMemStorage_ntop(lua_State *L)
{
  checkCvMemStorage(L,1)->top=checkCvMemBlock(L,3);
return 0;
}

static int CvMemStorage_top(lua_State *L)
{
  pushCvMemBlock(L,checkCvMemStorage(L,1)->top);
return 1;
}
static int CvMemStorage_nparent(lua_State *L)
{
  checkCvMemStorage(L,1)->parent=checkCvMemStorage(L,3);
return 0;
}

static int CvMemStorage_parent(lua_State *L)
{
  pushCvMemStorage(L,checkCvMemStorage(L,1)->parent);
return 1;
}

static const luacv_method CvMemStorage_v[]=
{
  {NULL,7,NULL,NULL},
  methodReg(bottom,CvMemStorage),
  methodReg(block_size,CvMemStorage),
  methodReg(free_space,CvMemStorage),
  methodReg(parent,CvMemStorage),
  methodReg(signature,CvMemStorage),
  methodReg(top,CvMemStorage),
};

makeIndexFunctions(CvMemStorage)
makeObjectCallback(CvMemStorage);
