#include "CvBGCodeBookModel.h"
static int CvBGCodeBookModel_tostring(lua_State *L)
{
  CvBGCodeBookModel *e=checkCvBGCodeBookModel(L,1);
  lua_pushfstring(L,CVBGCODEBOOKMODEL_NAME" object %p\n\t"CVSIZE_NAME" size={ width=>%d, height=>%d }\n\tint t=%d\n\tcbBounds={ [1]=>%d, [2]=>%d, [3]=>%d }\n\tmodMin={ [1]=>%d, [2]=>%d, [3]=>%d }\n\tmodMax={ [1]=>%d, [2]=>%d, [3]=>%d }\n\t"CVBGCODEBOOKELEM_NAME" cbmap=%p\n\t"CVMEMSTORAGE_NAME" storage=%p\n\t"CVBGCODEBOOKELEM_NAME" freeList=%p",e,e->size.width,e->size.height,e->t,e->cbBounds[0],e->cbBounds[1],e->cbBounds[2],e->modMin[0],e->modMin[1],e->modMin[2],e->modMax[0],e->modMax[1],e->modMax[2],e->cbmap,e->storage,e->freeList);
  return 1;
}

static int CvBGCodeBookModel_gc(lua_State *L)
{
  luacv_obj<CvBGCodeBookModel>*obj=(luacv_obj<CvBGCodeBookModel>*)luaL_checkudata(L,1,CVBGCODEBOOKMODEL_NAME);
  if (obj->dealloc_data)
    cvReleaseBGCodeBookModel(&(obj->data)); 
  return 0;
}

static int CvBGCodeBookModel_cbBounds(lua_State *L)
{
    CvBGCodeBookModel *s=checkCvBGCodeBookModel(L,1);
    lua_newtable(L);
    for(int i=0;i<3;i++)
    {
      lua_pushnumber(L,s->cbBounds[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvBGCodeBookModel_ncbBounds(lua_State *L)
{
    const char f_msg[]=CVBGCODEBOOKMODEL_NAME".cbBounds={int, int, int}";
    CvBGCodeBookModel *s=checkCvBGCodeBookModel(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=3)) luaL_error(L,f_msg);
    for(int i=0;i<3;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->cbBounds[i]=(uchar)checkint(L,4);
      lua_pop(L,1);
    }
    return 0;
}

static int CvBGCodeBookModel_modMin(lua_State *L)
{
    CvBGCodeBookModel *s=checkCvBGCodeBookModel(L,1);
    lua_newtable(L);
    for(int i=0;i<3;i++)
    {
      lua_pushnumber(L,s->modMin[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvBGCodeBookModel_nmodMin(lua_State *L)
{
    const char f_msg[]=CVBGCODEBOOKMODEL_NAME".modMin={int, int, int}";
    CvBGCodeBookModel *s=checkCvBGCodeBookModel(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=3)) luaL_error(L,f_msg);
    for(int i=0;i<3;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->modMin[i]=(uchar)checkint(L,4);
      lua_pop(L,1);
    }
    return 0;
}
static int CvBGCodeBookModel_modMax(lua_State *L)
{
    CvBGCodeBookModel *s=checkCvBGCodeBookModel(L,1);
    lua_newtable(L);
    for(int i=0;i<3;i++)
    {
      lua_pushnumber(L,s->modMax[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvBGCodeBookModel_nmodMax(lua_State *L)
{
    const char f_msg[]=CVBGCODEBOOKMODEL_NAME".modMax={int, int, int}";
    CvBGCodeBookModel *s=checkCvBGCodeBookModel(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=3)) luaL_error(L,f_msg);
    for(int i=0;i<3;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->modMax[i]=(uchar)checkint(L,4);
      lua_pop(L,1);
    }
    return 0;
}


static int CvBGCodeBookModel_t(lua_State *L)
{
    lua_pushnumber(L,checkCvBGCodeBookModel(L,1)->t);
    return 1;
}

static int CvBGCodeBookModel_nt(lua_State *L)
{
    const char f_msg[]=CVBGCODEBOOKMODEL_NAME".t=int";
    checkCvBGCodeBookModel(L,1)->t=checkint(L,3);
    return 0;
}

static int CvBGCodeBookModel_size(lua_State *L)
{
    pushCvSize(L,&checkCvBGCodeBookModel(L,1)->size);
    return 1;
}

static int CvBGCodeBookModel_nsize(lua_State *L)
{
    checkCvBGCodeBookModel(L,1)->size=*checkCvSize(L,3);
    return 0;
}

static int CvBGCodeBookModel_freeList(lua_State *L)
{
    pushUserData(L,checkCvBGCodeBookModel(L,1)->freeList);
    luaL_getmetatable(L,CVBGCODEBOOKELEM_NAME);
    lua_setmetatable(L,-2);
    return 1;
}

static int CvBGCodeBookModel_nfreeList(lua_State *L)
{
    checkCvBGCodeBookModel(L,1)->freeList=checkCvBGCodeBookElem(L,3);
    return 0;
}

static int CvBGCodeBookModel_storage(lua_State *L)
{
    pushCvMemStorage(L,checkCvBGCodeBookModel(L,1)->storage);
    return 1;
}

static int CvBGCodeBookModel_nstorage(lua_State *L)
{
    checkCvBGCodeBookModel(L,1)->storage=checkCvMemStorage(L,3);
    return 0;
}

static int CvBGCodeBookModel_cbmap(lua_State *L)
{
    UNIMPL(L);
    return 1;
}

static int CvBGCodeBookModel_ncbmap(lua_State *L)
{
    UNIMPL(L);
    return 1;
}

static const luacv_method CvBGCodeBookModel_v[]=
{
  {NULL,9,NULL,NULL},
  methodReg(cbBounds,CvBGCodeBookModel),
  methodReg(cbmap,CvBGCodeBookModel),
  methodReg(freeList,CvBGCodeBookModel),
  methodReg(modMin,CvBGCodeBookModel),
  methodReg(modMax,CvBGCodeBookModel),
  methodReg(size,CvBGCodeBookModel),
  methodReg(storage,CvBGCodeBookModel),
  methodReg(t,CvBGCodeBookModel),
};

makeIndexFunctions(CvBGCodeBookModel)
makeObjectCallback(CvBGCodeBookModel);
