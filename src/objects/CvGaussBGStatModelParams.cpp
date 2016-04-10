#include "CvGaussBGStatModelParams.h"

static int CvGaussBGStatModelParams_tostring(lua_State *L)
{
  CvGaussBGStatModelParams *p=checkCvGaussBGStatModelParams(L,1);
  lua_pushfstring(L,CVGAUSSBGSTATMODELPARAMS_NAME" object: %p\n\tint win_size=%d\n\t int n_gaussy=%d\n\tbg_threshold=%f\n\tstd_threshold=%f\n\tminArea=%f\n\tweight_init=%f\n\tvariance_init=%f",p,p->win_size,p->n_gauss,p->bg_threshold,p->std_threshold,p->minArea,p->weight_init,p->variance_init);
  return 1;
}

static int CvGaussBGStatModelParams_gc(lua_State *L)
{
  luacv_obj<CvGaussBGStatModelParams>*obj=(luacv_obj<CvGaussBGStatModelParams>*)luaL_checkudata(L,1,CVGAUSSBGSTATMODELPARAMS_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 

  return 0;
}

static int CvGaussBGStatModelParams_nwin_size(lua_State *L)
{
  const char f_msg[]=CVGAUSSBGSTATMODELPARAMS_NAME".win_size=int";
  checkCvGaussBGStatModelParams(L,1)->win_size=checkint(L,3);
return 0;
}

static int CvGaussBGStatModelParams_win_size(lua_State *L)
{
  lua_pushnumber(L,checkCvGaussBGStatModelParams(L,1)->win_size);
return 1;
}

static int CvGaussBGStatModelParams_nn_gauss(lua_State *L)
{
  const char f_msg[]=CVGAUSSBGSTATMODELPARAMS_NAME".n_gauss=int";
  checkCvGaussBGStatModelParams(L,1)->n_gauss=checkint(L,3);
return 0;
}

static int CvGaussBGStatModelParams_n_gauss(lua_State *L)
{
  lua_pushnumber(L,checkCvGaussBGStatModelParams(L,1)->n_gauss);
return 1;
}


static int CvGaussBGStatModelParams_nbg_threshold(lua_State *L)
{
  const char f_msg[]=CVGAUSSBGSTATMODELPARAMS_NAME".bg_threshold=num";
  checkCvGaussBGStatModelParams(L,1)->bg_threshold=checknumber(L,3);
return 0;
}

static int CvGaussBGStatModelParams_bg_threshold(lua_State *L)
{
  lua_pushnumber(L,checkCvGaussBGStatModelParams(L,1)->bg_threshold);
return 1;
}
static int CvGaussBGStatModelParams_nstd_threshold(lua_State *L)
{
  const char f_msg[]=CVGAUSSBGSTATMODELPARAMS_NAME".std_threshold=num";
  checkCvGaussBGStatModelParams(L,1)->std_threshold=checknumber(L,3);
return 0;
}

static int CvGaussBGStatModelParams_std_threshold(lua_State *L)
{
  lua_pushnumber(L,checkCvGaussBGStatModelParams(L,1)->std_threshold);
return 1;
}

static int CvGaussBGStatModelParams_nminArea(lua_State *L)
{
  const char f_msg[]=CVGAUSSBGSTATMODELPARAMS_NAME".minArea=num";
  checkCvGaussBGStatModelParams(L,1)->minArea=checknumber(L,3);
return 0;
}

static int CvGaussBGStatModelParams_minArea(lua_State *L)
{
  lua_pushnumber(L,checkCvGaussBGStatModelParams(L,1)->minArea);
return 1;
}
static int CvGaussBGStatModelParams_nweight_init(lua_State *L)
{
  const char f_msg[]=CVGAUSSBGSTATMODELPARAMS_NAME".weight_init=num";
  checkCvGaussBGStatModelParams(L,1)->weight_init=checknumber(L,3);
return 0;
}

static int CvGaussBGStatModelParams_weight_init(lua_State *L)
{
  lua_pushnumber(L,checkCvGaussBGStatModelParams(L,1)->weight_init);
return 1;
}

static int CvGaussBGStatModelParams_nvariance_init(lua_State *L)
{
  const char f_msg[]=CVGAUSSBGSTATMODELPARAMS_NAME".variance_init=num";
  checkCvGaussBGStatModelParams(L,1)->variance_init=checknumber(L,3);
return 0;
}

static int CvGaussBGStatModelParams_variance_init(lua_State *L)
{
  lua_pushnumber(L,checkCvGaussBGStatModelParams(L,1)->variance_init);
return 1;
}

static const luacv_method CvGaussBGStatModelParams_v[]=
{
  {NULL,8,NULL,NULL},
  methodReg(bg_threshold,CvGaussBGStatModelParams),
  methodReg(minArea,CvGaussBGStatModelParams),
  methodReg(n_gauss,CvGaussBGStatModelParams),
  methodReg(std_threshold,CvGaussBGStatModelParams),
  methodReg(variance_init,CvGaussBGStatModelParams),
  methodReg(weight_init,CvGaussBGStatModelParams),
  methodReg(win_size,CvGaussBGStatModelParams),
};

makeIndexFunctions(CvGaussBGStatModelParams)
makeObjectCallback(CvGaussBGStatModelParams);
