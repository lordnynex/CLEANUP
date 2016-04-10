#include "CvGaussBGModel.h"

static int CvGaussBGModel_tostring(lua_State *L)
{
  CvGaussBGModel *m=checkCvGaussBGModel(L,1);
  lua_pushfstring(L,CVGAUSSBGMODEL_NAME" object %p\n\tint type=%d\n\t"IPLIMAGE_NAME" background=%p\n\t"IPLIMAGE_NAME" foreground=%p\n\t"IPLIMAGE_NAME"[] layers=%p\n\tint layer_count=%d\n\t"CVMEMSTORAGE_NAME" storage=%p\n\t"CVSEQ_NAME" foreground_regions=%p\n\t"CVGAUSSBGSTATMODELPARAMS_NAME" params=%p\n\t"CVGAUSSBGPOINT_NAME" g_point=%p\n\tint countFrames=%d",m,m->background,m->foreground,m->layers,m->layer_count,m->storage,m->foreground_regions,&(m->params),m->g_point,m->countFrames);
  return 1;
}

static int CvGaussBGModel_gc(lua_State *L)
{

  luacv_obj<CvGaussBGModel>*obj=(luacv_obj<CvGaussBGModel>*)luaL_checkudata(L,1,CVGAUSSBGMODEL_NAME);
  if (obj->dealloc_data)
    cvReleaseBGStatModel((CvBGStatModel**)&(obj->data)); 
  return 0;
}

static inline int CvGaussBGModel_ncountFrames(lua_State *L)
{
    const char f_msg[]=CVGAUSSBGMODEL_NAME".countFrames=int";
    checkCvGaussBGModel(L,1)->countFrames=checkint(L,3);
    return 0;
}
static inline int CvGaussBGModel_countFrames(lua_State *L)
{
    lua_pushnumber(L,checkCvGaussBGModel(L,1)->countFrames);
    return 1;
}



static inline int CvGaussBGModel_ntype(lua_State *L)
{
    const char f_msg[]=CVGAUSSBGMODEL_NAME".type=int";
    checkCvGaussBGModel(L,1)->type=checkint(L,3);
    return 0;
}
static inline int CvGaussBGModel_type(lua_State *L)
{
    lua_pushnumber(L,checkCvGaussBGModel(L,1)->type);
    return 1;
}


static inline int CvGaussBGModel_nlayer_count(lua_State *L)
{
    const char f_msg[]=CVGAUSSBGMODEL_NAME".layer_count=int";
    checkCvGaussBGModel(L,1)->layer_count=checkint(L,3);
    return 0;
}

static inline int CvGaussBGModel_layer_count(lua_State *L)
{
    lua_pushnumber(L,checkCvGaussBGModel(L,1)->layer_count);
    return 1;
}


static inline int CvGaussBGModel_nbackground(lua_State *L)
{
    checkCvGaussBGModel(L,1)->background=checkIplImage(L,3);
    return 0;
}

static inline int CvGaussBGModel_background(lua_State *L)
{
    pushUserData(L,checkCvGaussBGModel(L,1)->background);
    luaL_getmetatable(L,IPLIMAGE_NAME);
    lua_setmetatable(L,-2);
    return 1;
}


static inline int CvGaussBGModel_nforeground(lua_State *L)
{
    checkCvGaussBGModel(L,1)->foreground=checkIplImage(L,3);
    return 0;
}

static inline int CvGaussBGModel_foreground(lua_State *L)
{
    pushUserData(L,checkCvGaussBGModel(L,1)->foreground);
    luaL_getmetatable(L,IPLIMAGE_NAME);
    lua_setmetatable(L,-2);

    return 1;
}
static inline int CvGaussBGModel_nstorage(lua_State *L)
{
    checkCvGaussBGModel(L,1)->storage=checkCvMemStorage(L,3);
    return 0;
}

static inline int CvGaussBGModel_storage(lua_State *L)
{
    pushCvMemStorage(L,checkCvGaussBGModel(L,1)->storage);
    return 1;
}

static inline int CvGaussBGModel_nforeground_regions(lua_State *L)
{
    checkCvGaussBGModel(L,1)->foreground_regions=checkCvSeq(L,3);
    return 0;
}

static inline int CvGaussBGModel_foreground_regions(lua_State *L)
{
    pushCvSeq(L,checkCvGaussBGModel(L,1)->foreground_regions);
    return 1;
}

static inline int CvGaussBGModel_nparams(lua_State *L)
{
    checkCvGaussBGModel(L,1)->params=*checkCvGaussBGStatModelParams(L,3);
    return 0;
}

static inline int CvGaussBGModel_params(lua_State *L)
{
    pushCvGaussBGStatModelParams(L,&checkCvGaussBGModel(L,1)->params);
    return 1;
}

static inline int CvGaussBGModel_ng_point(lua_State *L)
{
    checkCvGaussBGModel(L,1)->g_point=checkCvGaussBGPoint(L,3);
    return 0;
}

static inline int CvGaussBGModel_g_point(lua_State *L)
{
    pushCvGaussBGPoint(L,checkCvGaussBGModel(L,1)->g_point);
    return 1;
}
static int CvGaussBGModel_layers(lua_State *L)
{
    CvGaussBGModel *s=checkCvGaussBGModel(L,1);
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

static int CvGaussBGModel_nlayers(lua_State *L)
{
    const char f_msg[]=CVGAUSSBGMODEL_NAME".layers={"IPLIMAGE_NAME", ...}";
    CvGaussBGModel *s=checkCvGaussBGModel(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=(unsigned)s->layer_count)) luaL_error(L,f_msg);
    for(int i=0;i<s->layer_count;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->layers[i]=checkIplImage(L,4);
      lua_pop(L,1);
    }
    return 0;
}
static const luacv_method CvGaussBGModel_v[]=
{
  {NULL,11,NULL,NULL},
  methodReg(background,CvGaussBGModel),
  methodReg(countFrames,CvGaussBGModel),
  methodReg(foreground,CvGaussBGModel),
  methodReg(foreground_regions,CvGaussBGModel),
  methodReg(g_point,CvGaussBGModel),
  methodReg(layers,CvGaussBGModel),
  methodReg(layer_count,CvGaussBGModel),
  methodReg(params,CvGaussBGModel),
  methodReg(storage,CvGaussBGModel),
  methodReg(type,CvGaussBGModel),

};

makeIndexFunctions(CvGaussBGModel)
makeObjectCallback(CvGaussBGModel);

