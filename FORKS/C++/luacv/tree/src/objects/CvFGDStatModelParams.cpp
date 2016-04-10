#include "CvFGDStatModelParams.h"

static int CvFGDStatModelParams_tostring(lua_State *L)
{
  CvFGDStatModelParams *m=checkCvFGDStatModelParams(L,1);
  lua_pushfstring(L,CVFGDSTATMODELPARAMS_NAME" object: %p\n\tint Lc=%d\n\tint N1c=%d\n\tint N2c=%d\n\t int Lcc=%d\n\tint N1cc=%d\n\tint N2cc=%d\n\tint is_obj_without_holes=%d\n\tint perform_morphing=%d\n\tnum alpha1=%f\n\tnum alpha2=%f\n\tnum alpha3=%f\n\tnum delta=%f\n\tnum T=%f\n\tnum minArea=%f",m,m->Lc,m->N1c,m->N2c,m->Lcc,m->N1cc,m->N2cc,m->is_obj_without_holes,m->perform_morphing,m->alpha1,m->alpha2,m->alpha3,m->delta,m->T,m->minArea);
  return 1;
}

static int CvFGDStatModelParams_gc(lua_State *L)
{
  luacv_obj<CvFGDStatModelParams>*obj=(luacv_obj<CvFGDStatModelParams>*)luaL_checkudata(L,1,CVFGDSTATMODELPARAMS_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 

  return 0;
}

static int CvFGDStatModelParams_nLc(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".Lc=int";
  checkCvFGDStatModelParams(L,1)->Lc=checkint(L,3);
return 0;
}

static int CvFGDStatModelParams_Lc(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->Lc);
return 1;
}

static int CvFGDStatModelParams_nN1c(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".N1c=int";
  checkCvFGDStatModelParams(L,1)->N1c=checkint(L,3);
return 0;
}

static int CvFGDStatModelParams_N1c(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->N1c);
return 1;
}



static int CvFGDStatModelParams_nN2c(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".N2c=int";
  checkCvFGDStatModelParams(L,1)->N2c=checkint(L,3);
return 0;
}

static int CvFGDStatModelParams_N2c(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->N2c);
return 1;
}


static int CvFGDStatModelParams_nLcc(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".Lcc=int";
  checkCvFGDStatModelParams(L,1)->Lcc=checkint(L,3);
return 0;
}

static int CvFGDStatModelParams_Lcc(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->Lcc);
return 1;
}


static int CvFGDStatModelParams_nN1cc(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".N1cc=int";
  checkCvFGDStatModelParams(L,1)->N1cc=checkint(L,3);
return 0;
}

static int CvFGDStatModelParams_N1cc(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->N1cc);
return 1;
}


static int CvFGDStatModelParams_nN2cc(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".N2cc=int";
  checkCvFGDStatModelParams(L,1)->N2cc=checkint(L,3);
return 0;
}

static int CvFGDStatModelParams_N2cc(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->N2cc);
return 1;
}



static int CvFGDStatModelParams_nis_obj_without_holes(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".is_obj_without_holes=int";
  checkCvFGDStatModelParams(L,1)->is_obj_without_holes=checkint(L,3);
return 0;
}

static int CvFGDStatModelParams_is_obj_without_holes(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->is_obj_without_holes);
return 1;
}


static int CvFGDStatModelParams_nperform_morphing(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".perform_morphing=int";
  checkCvFGDStatModelParams(L,1)->perform_morphing=checkint(L,3);
return 0;
}

static int CvFGDStatModelParams_perform_morphing(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->perform_morphing);
return 1;
}



static int CvFGDStatModelParams_nalpha1(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".alpha1=num";
  checkCvFGDStatModelParams(L,1)->alpha1=(float)checknumber(L,3);
return 0;
}

static int CvFGDStatModelParams_alpha1(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->alpha1);
return 1;
}
static int CvFGDStatModelParams_nalpha2(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".alpha2=num";
  checkCvFGDStatModelParams(L,1)->alpha2=(float)checknumber(L,3);
return 0;
}

static int CvFGDStatModelParams_alpha2(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->alpha2);
return 1;
}

static int CvFGDStatModelParams_nalpha3(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".alpha3=num";
  checkCvFGDStatModelParams(L,1)->alpha3=(float)checknumber(L,3);
return 0;
}

static int CvFGDStatModelParams_alpha3(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->alpha3);
return 1;
}

static int CvFGDStatModelParams_ndelta(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".delta=num";
  checkCvFGDStatModelParams(L,1)->delta=(float)checknumber(L,3);
return 0;
}

static int CvFGDStatModelParams_delta(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->delta);
return 1;
}

static int CvFGDStatModelParams_nT(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".T=num";
  checkCvFGDStatModelParams(L,1)->T=(float)checknumber(L,3);
return 0;
}

static int CvFGDStatModelParams_T(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->T);
return 1;
}

static int CvFGDStatModelParams_nminArea(lua_State *L)
{
  const char f_msg[]=CVFGDSTATMODELPARAMS_NAME".minArea=num";
  checkCvFGDStatModelParams(L,1)->minArea=(float)checknumber(L,3);
return 0;
}

static int CvFGDStatModelParams_minArea(lua_State *L)
{
  lua_pushnumber(L,checkCvFGDStatModelParams(L,1)->minArea);
return 1;
}

static const luacv_method CvFGDStatModelParams_v[]=
{
  {NULL,15,NULL,NULL},
  methodReg(alpha1,CvFGDStatModelParams),
  methodReg(alpha2,CvFGDStatModelParams),
  methodReg(alpha3,CvFGDStatModelParams),
  methodReg(delta,CvFGDStatModelParams),
  methodReg(is_obj_without_holes,CvFGDStatModelParams),
  methodReg(Lc,CvFGDStatModelParams),
  methodReg(Lcc,CvFGDStatModelParams),
  methodReg(N1c,CvFGDStatModelParams),
  methodReg(N2c,CvFGDStatModelParams),
  methodReg(N1cc,CvFGDStatModelParams),
  methodReg(N2cc,CvFGDStatModelParams),
  methodReg(minArea,CvFGDStatModelParams),
  methodReg(perform_morphing,CvFGDStatModelParams),
  methodReg(T,CvFGDStatModelParams),
};

makeIndexFunctions(CvFGDStatModelParams)
makeObjectCallback(CvFGDStatModelParams);
