#include "CvSparseMatIterator.h"

static int CvSparseMatIterator_tostring(lua_State *L)
{
  CvSparseMatIterator *iter=checkCvSparseMatIterator(L,1);
  lua_pushfstring(L,CVSPARSEMATITERATOR_NAME" object %p\n\tmat="CVSPARSEMAT_NAME"\n\tnode="CVSPARSENODE_NAME"\n\tcuridx=%d",iter,iter->curidx);

  return 1;
}

static int CvSparseMatIterator_gc(lua_State *L)
{

  luacv_obj<CvSparseMatIterator>*obj=(luacv_obj<CvSparseMatIterator>*)luaL_checkudata(L,1,CVSPARSEMATITERATOR_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvSparseMatIterator_ncuridx(lua_State *L)
{
  const char f_msg[]=CVSPARSEMATITERATOR_NAME".curidx=int";
  checkCvSparseMatIterator(L,1)->curidx=checkint(L,3);
return 0;
}

static int CvSparseMatIterator_curidx(lua_State *L)
{
  lua_pushnumber(L,checkCvSparseMatIterator(L,1)->curidx);
return 1;
}


static int CvSparseMatIterator_nmat(lua_State *L)
{
  checkCvSparseMatIterator(L,1)->mat=checkCvSparseMat(L,3);
return 0;
}

static int CvSparseMatIterator_mat(lua_State *L)
{
  pushCvSparseMat(L,checkCvSparseMatIterator(L,1)->mat);
return 1;
}



static int CvSparseMatIterator_nnode(lua_State *L)
{
  checkCvSparseMatIterator(L,1)->node=checkCvSparseNode(L,3);
return 0;
}

static int CvSparseMatIterator_node(lua_State *L)
{
  pushCvSparseNode(L,checkCvSparseMatIterator(L,1)->node);
return 1;
}


static const luacv_method CvSparseMatIterator_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(curidx,CvSparseMatIterator),
  methodReg(mat,CvSparseMatIterator),
  methodReg(node,CvSparseMatIterator),
};

makeIndexFunctions(CvSparseMatIterator)
makeObjectCallback(CvSparseMatIterator);
