#include "CvSparseNode.h"

static int CvSparseNode_tostring(lua_State *L)
{
  CvSparseNode *node=checkCvSparseNode(L,1);
  lua_pushfstring(L,CVSPARSENODE_NAME" object %p\n\thashval=%d\n\tnext="CVSPARSENODE_NAME,node,node->hashval);
  return 1;
}

static int CvSparseNode_gc(lua_State *L)
{

  luacv_obj<CvSparseNode>*obj=(luacv_obj<CvSparseNode>*)luaL_checkudata(L,1,CVSPARSENODE_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvSparseNode_nhashval(lua_State *L)
{
  const char f_msg[]=CVSPARSENODE_NAME".hashval=uint";
  checkCvSparseNode(L,1)->hashval=checkint(L,3);
return 0;
}

static int CvSparseNode_hashval(lua_State *L)
{
  lua_pushnumber(L,checkCvSparseNode(L,1)->hashval);
return 1;
}

static int CvSparseNode_nnext(lua_State *L)
{
  checkCvSparseNode(L,1)->next=checkCvSparseNode(L,3);
return 0;
}

static int CvSparseNode_next(lua_State *L)
{
  pushCvSparseNode(L,checkCvSparseNode(L,1)->next);
return 1;
}


static const luacv_method CvSparseNode_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(hashval,CvSparseNode),
  methodReg(next,CvSparseNode),
};

makeIndexFunctions(CvSparseNode)
makeObjectCallback(CvSparseNode);


