#include "CvSparseMat.h"

static int CvSparseMat_tostring(lua_State *L)
{
  CvSparseMat *mat=checkCvSparseMat(L,1);
  lua_pushfstring(L,CVSPARSEMAT_NAME" object %p\n\ttype=%d\n\tdims=%d\n\thdr_refcount=%d\n\thashsize=%d\n\tvaloffset=%d\n\tidxoffset=%d",mat,mat->type,mat->dims,mat->hdr_refcount,mat->hashsize,mat->valoffset,mat->idxoffset);

  return 1;
}

static int CvSparseMat_gc(lua_State *L)
{

  luacv_obj<CvSparseMat>*obj=(luacv_obj<CvSparseMat>*)luaL_checkudata(L,1,CVSPARSEMAT_NAME);
  if (obj->dealloc_data)
    cvReleaseSparseMat(&obj->data); 

  return 0;
}
static int CvSparseMat_ntype(lua_State *L)
{
  const char f_msg[]=CVSPARSEMAT_NAME".type=int";
  checkCvSparseMat(L,1)->type=checkint(L,3);
return 0;
}

static int CvSparseMat_type(lua_State *L)
{
  lua_pushnumber(L,checkCvSparseMat(L,1)->type);
return 1;
}
static int CvSparseMat_ndims(lua_State *L)
{
  const char f_msg[]=CVSPARSEMAT_NAME".dims=int";
  checkCvSparseMat(L,1)->dims=checkint(L,3);
return 0;
}

static int CvSparseMat_dims(lua_State *L)
{
  lua_pushnumber(L,checkCvSparseMat(L,1)->dims);
return 1;
}

static int CvSparseMat_nhdr_refcount(lua_State *L)
{
  const char f_msg[]=CVSPARSEMAT_NAME".hdr_refcount=int";
  checkCvSparseMat(L,1)->hdr_refcount=checkint(L,3);
return 0;
}

static int CvSparseMat_hdr_refcount(lua_State *L)
{
  lua_pushnumber(L,checkCvSparseMat(L,1)->hdr_refcount);
return 1;
}

static int CvSparseMat_nhashsize(lua_State *L)
{
  const char f_msg[]=CVSPARSEMAT_NAME".hashsize=int";
  checkCvSparseMat(L,1)->hashsize=checkint(L,3);
return 0;
}

static int CvSparseMat_hashsize(lua_State *L)
{
  lua_pushnumber(L,checkCvSparseMat(L,1)->hashsize);
return 1;
}
static int CvSparseMat_nvaloffset(lua_State *L)
{
  const char f_msg[]=CVSPARSEMAT_NAME".valoffset=int";
  checkCvSparseMat(L,1)->valoffset=checkint(L,3);
return 0;
}

static int CvSparseMat_valoffset(lua_State *L)
{
  lua_pushnumber(L,checkCvSparseMat(L,1)->valoffset);
return 1;
}

static int CvSparseMat_nidxoffset(lua_State *L)
{
  const char f_msg[]=CVSPARSEMAT_NAME".idxoffset=int";
  checkCvSparseMat(L,1)->idxoffset=checkint(L,3);
return 0;
}

static int CvSparseMat_idxoffset(lua_State *L)
{
  lua_pushnumber(L,checkCvSparseMat(L,1)->idxoffset);
return 1;
}

static int CvSparseMat_nsize(lua_State *L)
{
  if ((!lua_istable(L,3))|| (lua_objlen(L,3)>CV_MAX_DIM))luaL_error(L,CVSPARSEMAT_NAME"->size must be table of elements indexed from 0. Max number of elements must be %d.",CV_MAX_DIM);
  CvSparseMat *mat=checkCvSparseMat(L,1);
  unsigned int len=lua_objlen(L,3);
  const char f_msg[]=CVSPARSEMAT_NAME".size[]=int";

  for(unsigned int i=0;i<len;i++)
  {
    lua_rawgeti(L,3,i);
    mat->size[i]=checkint(L,4);
    lua_pop(L,1);
  }
return 0;
}

static int CvSparseMat_size(lua_State *L)
{
  CvSparseMat *mat=checkCvSparseMat(L,1);
  lua_newtable(L);
  for(unsigned int i=0;i<CV_MAX_DIM;i++)
  {
    lua_pushnumber(L,mat->size[i]);
    lua_rawseti(L,-2,i);
  }
  return 1;
}

static int CvSparseMat_nrefcount(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvSparseMat_refcount(lua_State *L)
{
  UNIMPL(L);
return 1;
}

static int CvSparseMat_nheap(lua_State *L)
{
  checkCvSparseMat(L,1)->heap=checkCvSet(L,3);
return 0;
}

static int CvSparseMat_heap(lua_State *L)
{
  pushCvSet(L,checkCvSparseMat(L,1)->heap);
return 1;
}


static int CvSparseMat_nhashtable(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvSparseMat_hashtable(lua_State *L)
{
  UNIMPL(L);
return 1;
}

static const luacv_method CvSparseMat_v[]=
{
  {NULL,10,NULL,NULL},
  methodReg(dims,CvSparseMat),
  methodReg(hdr_refcount,CvSparseMat),
  methodReg(heap,CvSparseMat),
  methodReg(hashtable,CvSparseMat),
  methodReg(hashsize,CvSparseMat),
  methodReg(idxoffset,CvSparseMat),
  methodReg(refcount,CvSparseMat),
  methodReg(size,CvSparseMat),
  methodReg(type,CvSparseMat),
  methodReg(valoffset,CvSparseMat),
};

makeIndexFunctions(CvSparseMat)
makeObjectCallback(CvSparseMat);

