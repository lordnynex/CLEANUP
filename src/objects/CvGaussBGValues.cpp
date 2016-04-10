#include "CvGaussBGValues.h"
static int CvGaussBGValues_tostring(lua_State *L)
{
  CvGaussBGValues *s=checkCvGaussBGValues(L,1);
  lua_pushfstring(L,CVGAUSSBGVALUES_NAME" object %p\n\tint match_sum=%d\n\tnum weight=%f\n\tvariance={ [1]=>%f, [2]=>%f, [3]=>%f }\n\tmean={ [1]=>%f, [2]=>%f, [3]=>%f }",s,s->match_sum,s->weight,s->variance[0],s->variance[1],s->variance[2],s->mean[0],s->mean[1],s->mean[2]);
  return 1;
}

static int CvGaussBGValues_gc(lua_State *L)
{
  luacv_obj<CvGaussBGValues>*obj=(luacv_obj<CvGaussBGValues>*)luaL_checkudata(L,1,CVGAUSSBGVALUES_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvGaussBGValues_variance(lua_State *L)
{
    CvGaussBGValues *s=checkCvGaussBGValues(L,1);
    lua_newtable(L);
    for(int i=0;i<CV_BGFG_MOG_NCOLORS;i++)
    {
      lua_pushnumber(L,s->variance[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvGaussBGValues_nvariance(lua_State *L)
{
    const char f_msg[]=CVGAUSSBGVALUES_NAME".variance={int, int, int}";
    CvGaussBGValues *s=checkCvGaussBGValues(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=CV_BGFG_MOG_NCOLORS)) luaL_error(L,f_msg);
    for(int i=0;i<CV_BGFG_MOG_NCOLORS;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->variance[i]=checknumber(L,4);
      lua_pop(L,1);
    }
    return 0;
}

static int CvGaussBGValues_mean(lua_State *L)
{
    CvGaussBGValues *s=checkCvGaussBGValues(L,1);
    lua_newtable(L);
    for(int i=0;i<CV_BGFG_MOG_NCOLORS;i++)
    {
      lua_pushnumber(L,s->mean[i]);
      lua_rawseti(L,3,i+1);
    }
    return 1;
}

static int CvGaussBGValues_nmean(lua_State *L)
{
    const char f_msg[]=CVGAUSSBGVALUES_NAME".mean={int, int, int}";
    CvGaussBGValues *s=checkCvGaussBGValues(L,1);
    if ((!lua_istable(L,3))||(lua_objlen(L,3)!=CV_BGFG_MOG_NCOLORS)) luaL_error(L,f_msg);
    for(int i=0;i<CV_BGFG_MOG_NCOLORS;i++) 
    {
      lua_rawgeti(L,3,i+1);
      s->mean[i]=checknumber(L,4);
      lua_pop(L,1);
    }
    return 0;
}

static int CvGaussBGValues_match_sum(lua_State *L)
{
    lua_pushnumber(L,checkCvGaussBGValues(L,1)->match_sum);
    return 1;
}

static int CvGaussBGValues_nmatch_sum(lua_State *L)
{
    const char f_msg[]=CVGAUSSBGVALUES_NAME".match_sum=int";
    checkCvGaussBGValues(L,1)->match_sum=checkint(L,3);
    return 0;
}
static int CvGaussBGValues_weight(lua_State *L)
{
    lua_pushnumber(L,checkCvGaussBGValues(L,1)->weight);
    return 1;
}

static int CvGaussBGValues_nweight(lua_State *L)
{
    const char f_msg[]=CVGAUSSBGVALUES_NAME".weight=num";
    checkCvGaussBGValues(L,1)->weight=checknumber(L,3);
    return 0;
}
static const luacv_method CvGaussBGValues_v[]=
{
  {NULL,5,NULL,NULL},
  methodReg(match_sum,CvGaussBGValues),
  methodReg(mean,CvGaussBGValues),
  methodReg(variance,CvGaussBGValues),
  methodReg(weight,CvGaussBGValues),
};

makeIndexFunctions(CvGaussBGValues)
makeObjectCallback(CvGaussBGValues);
