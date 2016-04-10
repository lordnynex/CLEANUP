#include "CvSlice.h"

static int CvSlice_tostring(lua_State *L)
{
  CvSlice *slice=checkCvSlice(L,1);
  lua_pushfstring(L,CVSLICE_NAME" object %p\n\tstart_index=%d\n\tend_index=%d",slice,slice->start_index,slice->end_index);

  return 1;
}
static int CvSlice_gc(lua_State *L)
{
  luacv_obj<CvSlice>*obj=(luacv_obj<CvSlice>*)luaL_checkudata(L,1,CVSLICE_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 

  return 0;
}

static inline int CvSlice_nstart_index(lua_State *L)
{
    const char f_msg[]=CVSLICE_NAME".start_index=int";
    checkCvSlice(L,1)->start_index=checkint(L,3);
    return 0;
}

static inline int CvSlice_start_index(lua_State *L)
{
    lua_pushnumber(L,checkCvSlice(L,1)->start_index);
    return 1;
}


static inline int CvSlice_nend_index(lua_State *L)
{
    const char f_msg[]=CVSLICE_NAME".end_index=int";
    checkCvSlice(L,1)->end_index=checkint(L,3);
    return 0;
}

static inline int CvSlice_end_index(lua_State *L)
{
    lua_pushnumber(L,checkCvSlice(L,1)->end_index);
    return 1;
}

static const luacv_method CvSlice_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(end_index,CvSlice),
  methodReg(start_index,CvSlice),
};

makeIndexFunctions(CvSlice)
makeObjectCallback(CvSlice);
