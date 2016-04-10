#include "CvBGPixelStat.h"

static int CvBGPixelStat_tostring(lua_State *L)
{
  CvBGPixelStat *p=checkCvBGPixelStat(L,1);
  lua_pushfstring(L,CVBGPIXELSTAT_NAME" object %p\n\tnum Pbc=%f\n\tnum Pbcc=%f\n\t"CVBGPIXELCSTATTABLE_NAME" ctable=%p\n\t"CVBGPIXELCCSTATTABLE_NAME" cctable=%p\n\tint is_trained_st_model=%d\n\tint is_trained_dyn_model",p,p->Pbc,p->Pbcc,p->ctable,p->cctable,p->is_trained_st_model,p->is_trained_dyn_model);
  return 1;
}

static int CvBGPixelStat_gc(lua_State *L)
{
  luacv_obj<CvBGPixelStat>*obj=(luacv_obj<CvBGPixelStat>*)luaL_checkudata(L,1,CVBGPIXELSTAT_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvBGPixelStat_nPbc(lua_State *L)
{
  const char f_msg[]=CVBGPIXELSTAT_NAME".Pbc=num";
  checkCvBGPixelStat(L,1)->Pbc=(float)checknumber(L,3);
return 0;
}

static int CvBGPixelStat_Pbc(lua_State *L)
{
  lua_pushnumber(L,checkCvBGPixelStat(L,1)->Pbc);
return 1;
}
static int CvBGPixelStat_nPbcc(lua_State *L)
{
  const char f_msg[]=CVBGPIXELSTAT_NAME".Pbcc=num";
  checkCvBGPixelStat(L,1)->Pbcc=(float)checknumber(L,3);
return 0;
}

static int CvBGPixelStat_Pbcc(lua_State *L)
{
  lua_pushnumber(L,checkCvBGPixelStat(L,1)->Pbcc);
return 1;
}
static int CvBGPixelStat_nis_trained_st_model(lua_State *L)
{
  const char f_msg[]=CVBGPIXELSTAT_NAME".is_trained_st_model=int";
  checkCvBGPixelStat(L,1)->is_trained_st_model=(uchar)checkint(L,3);
return 0;
}

static int CvBGPixelStat_is_trained_st_model(lua_State *L)
{
  lua_pushnumber(L,checkCvBGPixelStat(L,1)->is_trained_st_model);
return 1;
}

static int CvBGPixelStat_nis_trained_dyn_model(lua_State *L)
{
  const char f_msg[]=CVBGPIXELSTAT_NAME".is_trained_dyn_model=int";
  checkCvBGPixelStat(L,1)->is_trained_dyn_model=(uchar)checkint(L,3);
return 0;
}

static int CvBGPixelStat_is_trained_dyn_model(lua_State *L)
{
  lua_pushnumber(L,checkCvBGPixelStat(L,1)->is_trained_dyn_model);
return 1;
}



static int CvBGPixelStat_nctable(lua_State *L)
{
  checkCvBGPixelStat(L,1)->ctable=checkCvBGPixelCStatTable(L,3);
return 0;
}

static int CvBGPixelStat_ctable(lua_State *L)
{
  pushCvBGPixelCStatTable(L,checkCvBGPixelStat(L,1)->ctable);
return 1;
}

static int CvBGPixelStat_ncctable(lua_State *L)
{
  checkCvBGPixelStat(L,1)->cctable=checkCvBGPixelCCStatTable(L,3);
return 0;
}

static int CvBGPixelStat_cctable(lua_State *L)
{
  pushCvBGPixelCCStatTable(L,checkCvBGPixelStat(L,1)->cctable);
return 1;
}
static const luacv_method CvBGPixelStat_v[]=
{
  {NULL,7,NULL,NULL},
  methodReg(ctable,CvBGPixelStat),
  methodReg(cctable,CvBGPixelStat),
  methodReg(is_trained_st_model,CvBGPixelStat),
  methodReg(is_trained_dyn_model,CvBGPixelStat),
  methodReg(Pbcc,CvBGPixelStat),
  methodReg(Pbc,CvBGPixelStat),
};

makeIndexFunctions(CvBGPixelStat)
makeObjectCallback(CvBGPixelStat);

