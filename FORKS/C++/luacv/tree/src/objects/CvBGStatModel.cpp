#include "CvBGStatModel.h"

static int CvBGStatModel_tostring(lua_State *L)
{
  CvBGStatModel *m=checkCvBGStatModel(L,1);
  lua_pushfstring(L,CVBGSTATMODEL_NAME" object %p\n\tint type=%d\n\t"IPLIMAGE_NAME" background=%p\n\t"IPLIMAGE_NAME" foreground=%p\n\t"IPLIMAGE_NAME"[] layers=%p\n\tint layer_count=%d\n\t"CVMEMSTORAGE_NAME" storage=%p\n\t"CVSEQ_NAME" foreground_regions=%p",m,m->background,m->foreground,m->layers,m->layer_count,m->storage,m->foreground_regions);
  return 1;
}

static int CvBGStatModel_gc(lua_State *L)
{

  luacv_obj<CvBGStatModel>*obj=(luacv_obj<CvBGStatModel>*)luaL_checkudata(L,1,CVBGSTATMODEL_NAME);
  if (obj->dealloc_data)
    cvReleaseBGStatModel(&(obj->data)); 
  return 0;
}

static inline int CvBGStatModel_ntype(lua_State *L)
{
    const char f_msg[]=CVBGSTATMODEL_NAME".type=int";
    checkCvBGStatModel(L,1)->type=checkint(L,3);
    return 0;
}
static inline int CvBGStatModel_type(lua_State *L)
{
    lua_pushnumber(L,checkCvBGStatModel(L,1)->type);
    return 1;
}


static inline int CvBGStatModel_nlayer_count(lua_State *L)
{
    const char f_msg[]=CVBGSTATMODEL_NAME".layer_count=int";
    checkCvBGStatModel(L,1)->layer_count=checkint(L,3);
    return 0;
}

static inline int CvBGStatModel_layer_count(lua_State *L)
{
    lua_pushnumber(L,checkCvBGStatModel(L,1)->layer_count);
    return 1;
}


static inline int CvBGStatModel_nbackground(lua_State *L)
{
    checkCvBGStatModel(L,1)->background=checkIplImage(L,3);
    return 0;
}

static inline int CvBGStatModel_background(lua_State *L)
{
    pushUserData(L,checkCvBGStatModel(L,1)->background);
    luaL_getmetatable(L,IPLIMAGE_NAME);
    lua_setmetatable(L,-2);
    return 1;
}


static inline int CvBGStatModel_nforeground(lua_State *L)
{
    checkCvBGStatModel(L,1)->foreground=checkIplImage(L,3);
    return 0;
}

static inline int CvBGStatModel_foreground(lua_State *L)
{
    pushUserData(L,checkCvBGStatModel(L,1)->foreground);
    luaL_getmetatable(L,IPLIMAGE_NAME);
    lua_setmetatable(L,-2);

    return 1;
}
static inline int CvBGStatModel_nstorage(lua_State *L)
{
    checkCvBGStatModel(L,1)->storage=checkCvMemStorage(L,3);
    return 0;
}

static inline int CvBGStatModel_storage(lua_State *L)
{
    pushCvMemStorage(L,checkCvBGStatModel(L,1)->storage);
    return 1;
}

static inline int CvBGStatModel_nforeground_regions(lua_State *L)
{
    checkCvBGStatModel(L,1)->foreground_regions=checkCvSeq(L,3);
    return 0;
}

static inline int CvBGStatModel_foreground_regions(lua_State *L)
{
    pushCvSeq(L,checkCvBGStatModel(L,1)->foreground_regions);
    return 1;
}

static int CvBGStatModel_layers(lua_State *L)
{
    CvBGStatModel *s=checkCvBGStatModel(L,1);
    lua_newtable(L);
    for(int i=0;i<s->layer_count;i++)
    {
      pushUserData(L,s->layers[i]);
      luaL_getmetatable(L,IPLIMAGE_NAME);
      lua_setmetatable(L,-2);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvBGStatModel_nlayers(lua_State *L)
{
    const char f_msg[]=CVBGSTATMODEL_NAME".layers={"IPLIMAGE_NAME", ...}";
    CvBGStatModel *s=checkCvBGStatModel(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=(unsigned)s->layer_count)) luaL_error(L,f_msg);
    for(int i=0;i<s->layer_count;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->layers[i]=checkIplImage(L,4);
      lua_pop(L,1);
    }
    return 0;
}
static const luacv_method CvBGStatModel_v[]=
{
  {NULL,8,NULL,NULL},
  methodReg(background,CvBGStatModel),
  methodReg(foreground,CvBGStatModel),
  methodReg(foreground_regions,CvBGStatModel),
  methodReg(layers,CvBGStatModel),
  methodReg(layer_count,CvBGStatModel),
  methodReg(storage,CvBGStatModel),
  methodReg(type,CvBGStatModel),
};

makeIndexFunctions(CvBGStatModel)
makeObjectCallback(CvBGStatModel);

