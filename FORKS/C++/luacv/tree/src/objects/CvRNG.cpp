#include "CvRNG.h"

static int CvRNG_tostring(lua_State *L)
{
  CvRNG *rng=checkCvRNG(L,1);
  lua_pushfstring(L,CVRNG_NAME" object %p %d",rng,*rng);
  return 1;
}

static int CvRNG_gc(lua_State *L)
{

  luacv_obj<CvRNG>*obj=(luacv_obj<CvRNG>*)luaL_checkudata(L,1,CVRNG_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvRNG_nrng(lua_State *L)
{
  *checkCvRNG(L,1)=*checkCvRNG(L,3);
return 0;
}

static int CvRNG_rng(lua_State *L)
{
  pushCvRNG(L,checkCvRNG(L,1));
return 1;
}

static const luacv_method CvRNG_v[]=
{
  {NULL,2,NULL,NULL},
  methodReg(rng,CvRNG),
};

makeIndexFunctions(CvRNG)
makeObjectCallback(CvRNG);
