#include "CvSeqBlock.h"

static int CvSeqBlock_tostring(lua_State *L)
{
  CvSeqBlock *block=checkCvSeqBlock(L,1);
  lua_pushfstring(L,CVSEQBLOCK_NAME" object %p\n\tprev="CVSEQBLOCK_NAME"\n\tnext="CVSEQBLOCK_NAME"\n\tstart_index=%d\n\tcount=%d\n\tdata=%p",block,block->start_index,block->count,block->data);

  return 1;
}

static int CvSeqBlock_gc(lua_State *L)
{

  luacv_obj<CvSeqBlock>*obj=(luacv_obj<CvSeqBlock>*)luaL_checkudata(L,1,CVSEQBLOCK_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvSeqBlock_ncount(lua_State *L)
{
  const char f_msg[]=CVSEQBLOCK_NAME".count=int";
  checkCvSeqBlock(L,1)->count=checkint(L,3);
return 0;
}

static int CvSeqBlock_count(lua_State *L)
{
  lua_pushnumber(L,checkCvSeqBlock(L,1)->count);
return 1;
}

static int CvSeqBlock_nstart_index(lua_State *L)
{
  const char f_msg[]=CVSEQBLOCK_NAME".start_index=int";
  checkCvSeqBlock(L,1)->start_index=checkint(L,3);
return 0;
}

static int CvSeqBlock_start_index(lua_State *L)
{
  lua_pushnumber(L,checkCvSeqBlock(L,1)->start_index);
return 1;
}
static int CvSeqBlock_ndata(lua_State *L)
{
  const char f_msg[]=CVSEQBLOCK_NAME".data=userdata";
  checkCvSeqBlock(L,1)->data=(schar*)checkldata(L,3);
return 0;
}

static int CvSeqBlock_data(lua_State *L)
{
  pushUserData(L,checkCvSeqBlock(L,1)->data);
return 1;
}

static int CvSeqBlock_nprev(lua_State *L)
{
  checkCvSeqBlock(L,1)->prev=checkCvSeqBlock(L,3);
return 0;
}

static int CvSeqBlock_prev(lua_State *L)
{
  pushCvSeqBlock(L,checkCvSeqBlock(L,1)->prev);
return 1;
}

static int CvSeqBlock_nnext(lua_State *L)
{
  checkCvSeqBlock(L,1)->next=checkCvSeqBlock(L,3);
return 0;
}

static int CvSeqBlock_next(lua_State *L)
{
  pushCvSeqBlock(L,checkCvSeqBlock(L,1)->next);
return 1;
}

static const luacv_method CvSeqBlock_v[]=
{
  {NULL,6,NULL,NULL},
  methodReg(count,CvSeqBlock),
  methodReg(data,CvSeqBlock),
  methodReg(next,CvSeqBlock),
  methodReg(prev,CvSeqBlock),
  methodReg(start_index,CvSeqBlock),
};

makeIndexFunctions(CvSeqBlock)
makeObjectCallback(CvSeqBlock);

