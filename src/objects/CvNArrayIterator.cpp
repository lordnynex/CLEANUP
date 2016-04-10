#include "CvNArrayIterator.h"

static int CvNArrayIterator_tostring(lua_State *L)
{
  CvNArrayIterator *iter=checkCvNArrayIterator(L,1);
  lua_pushfstring(L,CVNARRAYITERATOR_NAME" object %p\n\tcount=%d\n\tdims=%d\n\tsize"CVSIZE_NAME"\n\tptr[]=%p\n\tstack[]=%p\n\thdr[]="CVMATND_NAME,iter,iter->count,iter->dims,iter->ptr,iter->stack);

  return 1;
}

static int CvNArrayIterator_gc(lua_State *L)
{

  luacv_obj<CvNArrayIterator>*obj=(luacv_obj<CvNArrayIterator>*)luaL_checkudata(L,1,CVNARRAYITERATOR_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvNArrayIterator_ncount(lua_State *L)
{
  const char f_msg[]=CVNARRAYITERATOR_NAME".count=int";
  checkCvNArrayIterator(L,1)->count=checkint(L,3);
return 0;
}

static int CvNArrayIterator_count(lua_State *L)
{
  lua_pushnumber(L,checkCvNArrayIterator(L,1)->count);
return 1;
}

static int CvNArrayIterator_ndims(lua_State *L)
{
  const char f_msg[]=CVNARRAYITERATOR_NAME".dims=int";
  checkCvNArrayIterator(L,1)->dims=checkint(L,3);
return 0;
}

static int CvNArrayIterator_dims(lua_State *L)
{
  lua_pushnumber(L,checkCvNArrayIterator(L,1)->dims);
return 1;
}

static int CvNArrayIterator_nsize(lua_State *L)
{
  checkCvNArrayIterator(L,1)->size=*checkCvSize(L,3);
return 0;
}

static int CvNArrayIterator_size(lua_State *L)
{
  pushCvSize(L,&checkCvNArrayIterator(L,1)->size);
return 1;
}

static int CvNArrayIterator_nptr(lua_State *L)
{
  UNIMPL(L);
return 0;
}

static int CvNArrayIterator_ptr(lua_State *L)
{
  UNIMPL(L);
return 1;
}
static int CvNArrayIterator_nstack(lua_State *L)
{
  if ((!lua_istable(L,3))|| (lua_objlen(L,3)>CV_MAX_DIM))luaL_error(L,CVNARRAYITERATOR_NAME"->stack must be table of elements indexed from 0. Max number of elements must be %d",CV_MAX_DIM);
  const char f_msg[]=CVNARRAYITERATOR_NAME".stack[]=int";
  CvNArrayIterator *iterator=checkCvNArrayIterator(L,1);
  unsigned int len=lua_objlen(L,3);
  for(unsigned int i=0;i<len;i++)
  {
    lua_rawgeti(L,3,i);
    iterator->stack[i]=checkint(L,4);
    lua_pop(L,1);
  }
return 0;
}

static int CvNArrayIterator_stack(lua_State *L)
{
  CvNArrayIterator *iterator=checkCvNArrayIterator(L,1);
  lua_newtable(L);
  for(unsigned int i=0;i<CV_MAX_DIM;i++)
  {
    lua_pushnumber(L,iterator->stack[i]);
    lua_rawseti(L,-2,i);
  }

  return 1;
}

static int CvNArrayIterator_nhdr(lua_State *L)
{
  if ((!lua_istable(L,3))|| (lua_objlen(L,3)>CV_MAX_ARR))luaL_error(L,CVNARRAYITERATOR_NAME"->hdr must be table of elements indexed from 0. Max number of elements must be %d",CV_MAX_ARR);

  CvNArrayIterator *iterator=checkCvNArrayIterator(L,1);
  unsigned int len=lua_objlen(L,3);
  for(unsigned int i=0;i<len;i++)
  {
    lua_rawgeti(L,3,i);
    iterator->hdr[i]=checkCvMatND(L,4);
    lua_pop(L,1);
  }
return 0;
}

static int CvNArrayIterator_hdr(lua_State *L)
{
  CvNArrayIterator *iterator=checkCvNArrayIterator(L,1);
  lua_newtable(L);
  for(unsigned int i=0;i<CV_MAX_ARR;i++)
  {
    pushCvMatND(L,iterator->hdr[i]);
    lua_rawseti(L,-2,i);
  }
  return 1;
}



static const luacv_method CvNArrayIterator_v[]=
{
  {NULL,7,NULL,NULL},
  methodReg(count,CvNArrayIterator),
  methodReg(dims,CvNArrayIterator),
  methodReg(hdr,CvNArrayIterator),
  methodReg(ptr,CvNArrayIterator),
  methodReg(size,CvNArrayIterator),
  methodReg(stack,CvNArrayIterator),
};

makeIndexFunctions(CvNArrayIterator)
makeObjectCallback(CvNArrayIterator);
