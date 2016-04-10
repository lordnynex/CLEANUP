#include "CvEHMM.h"

static int CvEHMM_tostring(lua_State *L)
{
  CvEHMM *p=checkCvEHMM(L,1);
  lua_pushfstring(L,CVEHMM_NAME" object: %p\n\tnum level=%d\n\tnum num_states=%d\n\tnum[] transP=%p\n\tnum[] obsProb=%p",p,p->level,p->num_states,p->transP,p->obsProb);
  return 1;
}

static int CvEHMM_gc(lua_State *L)
{
  luacv_obj<CvEHMM>*obj=(luacv_obj<CvEHMM>*)luaL_checkudata(L,1,CVEHMM_NAME);
  if (obj->dealloc_data)
    cvRelease2DHMM(&(obj->data)); 
  return 0;
}

static int CvEHMM_nlevel(lua_State *L)
{
  const char f_msg[]=CVEHMM_NAME".level=int";
  checkCvEHMM(L,1)->level=checkint(L,3);
return 0;
}

static int CvEHMM_level(lua_State *L)
{
  lua_pushnumber(L,checkCvEHMM(L,1)->level);
return 1;
}

static int CvEHMM_nnum_states(lua_State *L)
{
  const char f_msg[]=CVEHMM_NAME".num_states=int";
  checkCvEHMM(L,1)->num_states=checkint(L,3);
return 0;
}

static int CvEHMM_num_states(lua_State *L)
{
  lua_pushnumber(L,checkCvEHMM(L,1)->num_states);
return 1;
}



static const luacv_method CvEHMM_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(level,CvEHMM),
  methodReg(num_states,CvEHMM),
};

makeIndexFunctions(CvEHMM)
makeObjectCallback(CvEHMM);
