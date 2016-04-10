#include "CvMemBlock.h"
static int CvMemBlock_tostring(lua_State *L)
{
  CvMemBlock *mem=checkCvMemBlock(L,1);
  lua_pushfstring(L,CVMEMBLOCK_NAME" object %p\n\tprev="CVMEMBLOCK_NAME"\n\tnext="CVMEMBLOCK_NAME,mem,mem->prev,mem->next);

  return 1;
}

static int CvMemBlock_gc(lua_State *L)
{
  
  luacv_obj<CvMemBlock>*obj=(luacv_obj<CvMemBlock>*)luaL_checkudata(L,1,CVMEMBLOCK_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvMemBlock_nprev(lua_State *L)
{
  checkCvMemBlock(L,1)->prev=checkCvMemBlock(L,3);
return 0;
}

static int CvMemBlock_prev(lua_State *L)
{
  pushCvMemBlock(L,checkCvMemBlock(L,1)->prev);
return 1;
}


static int CvMemBlock_nnext(lua_State *L)
{
  checkCvMemBlock(L,1)->next=checkCvMemBlock(L,3);
return 0;
}

static int CvMemBlock_next(lua_State *L)
{
  pushCvMemBlock(L,checkCvMemBlock(L,1)->next);
return 1;
}


static const luacv_method CvMemBlock_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(next,CvMemBlock),
  methodReg(prev,CvMemBlock),
};

makeIndexFunctions(CvMemBlock)
makeObjectCallback(CvMemBlock);
