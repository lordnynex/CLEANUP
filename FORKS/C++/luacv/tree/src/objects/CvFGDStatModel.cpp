#include "CvFGDStatModel.h"

static int CvFGDStatModel_tostring(lua_State *L)
{
  CvFGDStatModel *m=checkCvFGDStatModel(L,1);
  lua_pushfstring(L,CVFGDSTATMODEL_NAME" object %p\n\tint type=%d\n\t"IPLIMAGE_NAME" background=%p\n\t"IPLIMAGE_NAME" foreground=%p\n\t"IPLIMAGE_NAME"[] layers=%p\n\tint layer_count=%d\n\t"CVMEMSTORAGE_NAME" storage=%p\n\t"CVSEQ_NAME" foreground_regions=%p\n\t"CVBGPIXELSTAT_NAME" pixel_stat=%p\n\t"IPLIMAGE_NAME" Ftd=%p\n\t"IPLIMAGE_NAME" Fbd=%p\n\t"IPLIMAGE_NAME" prev_frame=%p\n\t"CVFGDSTATMODELPARAMS_NAME" params=%p",m,m->background,m->foreground,m->layers,m->layer_count,m->storage,m->foreground_regions,m->pixel_stat,m->Ftd,m->Fbd,m->prev_frame,&m->params);
  return 1;
}

static int CvFGDStatModel_gc(lua_State *L)
{

  luacv_obj<CvFGDStatModel>*obj=(luacv_obj<CvFGDStatModel>*)luaL_checkudata(L,1,CVFGDSTATMODEL_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static inline int CvFGDStatModel_ntype(lua_State *L)
{
    const char f_msg[]=CVFGDSTATMODEL_NAME".type=int";
    checkCvFGDStatModel(L,1)->type=checkint(L,3);
    return 0;
}
static inline int CvFGDStatModel_type(lua_State *L)
{
    lua_pushnumber(L,checkCvFGDStatModel(L,1)->type);
    return 1;
}


static inline int CvFGDStatModel_nlayer_count(lua_State *L)
{
    const char f_msg[]=CVFGDSTATMODEL_NAME".layer_count=int";
    checkCvFGDStatModel(L,1)->layer_count=checkint(L,3);
    return 0;
}

static inline int CvFGDStatModel_layer_count(lua_State *L)
{
    lua_pushnumber(L,checkCvFGDStatModel(L,1)->layer_count);
    return 1;
}


static inline int CvFGDStatModel_nbackground(lua_State *L)
{
    checkCvFGDStatModel(L,1)->background=checkIplImage(L,3);
    return 0;
}

static inline int CvFGDStatModel_background(lua_State *L)
{
    pushUserData(L,checkCvFGDStatModel(L,1)->background);
    luaL_getmetatable(L,IPLIMAGE_NAME);
    lua_setmetatable(L,-2);

    return 1;
}


static inline int CvFGDStatModel_nforeground(lua_State *L)
{
    checkCvFGDStatModel(L,1)->foreground=checkIplImage(L,3);
    return 0;
}

static inline int CvFGDStatModel_foreground(lua_State *L)
{
    pushUserData(L,checkCvFGDStatModel(L,1)->foreground);
    luaL_getmetatable(L,IPLIMAGE_NAME);
    lua_setmetatable(L,-2);


    return 1;
}
static inline int CvFGDStatModel_nstorage(lua_State *L)
{
    checkCvFGDStatModel(L,1)->storage=checkCvMemStorage(L,3);
    return 0;
}

static inline int CvFGDStatModel_storage(lua_State *L)
{
    pushCvMemStorage(L,checkCvFGDStatModel(L,1)->storage);
    return 1;
}

static inline int CvFGDStatModel_nforeground_regions(lua_State *L)
{
    checkCvFGDStatModel(L,1)->foreground_regions=checkCvSeq(L,3);
    return 0;
}

static inline int CvFGDStatModel_foreground_regions(lua_State *L)
{
    pushCvSeq(L,checkCvFGDStatModel(L,1)->foreground_regions);
    return 1;
}

static int CvFGDStatModel_layers(lua_State *L)
{
    CvFGDStatModel *s=checkCvFGDStatModel(L,1);
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

static int CvFGDStatModel_nlayers(lua_State *L)
{
    const char f_msg[]=CVFGDSTATMODEL_NAME".layers={"IPLIMAGE_NAME", ...}";
    CvFGDStatModel *s=checkCvFGDStatModel(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=(unsigned)s->layer_count)) luaL_error(L,f_msg);
    for(int i=0;i<s->layer_count;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->layers[i]=checkIplImage(L,4);
      lua_pop(L,1);
    }
    return 0;
}

static inline int CvFGDStatModel_nFtd(lua_State *L)
{
    checkCvFGDStatModel(L,1)->Ftd=checkIplImage(L,3);
    return 0;
}

static inline int CvFGDStatModel_Ftd(lua_State *L)
{
    pushUserData(L,checkCvFGDStatModel(L,1)->Ftd);
    luaL_getmetatable(L,IPLIMAGE_NAME);
    lua_setmetatable(L,-2);

    return 1;
}

static inline int CvFGDStatModel_nFbd(lua_State *L)
{
    checkCvFGDStatModel(L,1)->Fbd=checkIplImage(L,3);
    return 0;
}

static inline int CvFGDStatModel_Fbd(lua_State *L)
{
    pushUserData(L,checkCvFGDStatModel(L,1)->Fbd);
    luaL_getmetatable(L,IPLIMAGE_NAME);
    lua_setmetatable(L,-2);


    return 1;
}

static inline int CvFGDStatModel_nprev_frame(lua_State *L)
{
    checkCvFGDStatModel(L,1)->prev_frame=checkIplImage(L,3);
    return 0;
}

static inline int CvFGDStatModel_prev_frame(lua_State *L)
{
    pushUserData(L,checkCvFGDStatModel(L,1)->prev_frame);
    luaL_getmetatable(L,IPLIMAGE_NAME);
    lua_setmetatable(L,-2);


    return 1;
}

static inline int CvFGDStatModel_nparams(lua_State *L)
{
    checkCvFGDStatModel(L,1)->params=*checkCvFGDStatModelParams(L,3);
    return 0;
}

static inline int CvFGDStatModel_params(lua_State *L)
{
    pushCvFGDStatModelParams(L,&checkCvFGDStatModel(L,1)->params);
    return 1;
}

static inline int CvFGDStatModel_npixel_stat(lua_State *L)
{
    checkCvFGDStatModel(L,1)->pixel_stat=checkCvBGPixelStat(L,3);
    return 0;
}

static inline int CvFGDStatModel_pixel_stat(lua_State *L)
{
    pushCvBGPixelStat(L,checkCvFGDStatModel(L,1)->pixel_stat);
    return 1;
}


static const luacv_method CvFGDStatModel_v[]=
{
  {NULL,8,NULL,NULL},
  methodReg(Ftd,CvFGDStatModel),
  methodReg(Fbd,CvFGDStatModel),
  methodReg(background,CvFGDStatModel),
  methodReg(foreground,CvFGDStatModel),
  methodReg(foreground_regions,CvFGDStatModel),
  methodReg(layers,CvFGDStatModel),
  methodReg(layer_count,CvFGDStatModel),
  methodReg(pixel_stat,CvFGDStatModel),
  methodReg(prev_frame,CvFGDStatModel),
  methodReg(params,CvFGDStatModel),
  methodReg(storage,CvFGDStatModel),
  methodReg(type,CvFGDStatModel),
};

makeIndexFunctions(CvFGDStatModel)
makeObjectCallback(CvFGDStatModel);
