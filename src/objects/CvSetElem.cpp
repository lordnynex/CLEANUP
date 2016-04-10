#include "CvSetElem.h"

static int CvSetElem_tostring(lua_State *L)
{
  CvSetElem *elem=checkCvSetElem(L,1);
  lua_pushfstring(L,CVSETELEM_NAME" object %p\n\tflags=%d\n\tnext_free="CVSETELEM_NAME,elem,elem->flags);

  return 1;
}

static int CvSetElem_gc(lua_State *L)
{

  luacv_obj<CvSetElem>*obj=(luacv_obj<CvSetElem>*)luaL_checkudata(L,1,CVSETELEM_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}


static int CvSetElem_nflags(lua_State *L)
{
  const char f_msg[]=CVSETELEM_NAME".flags=int";
  checkCvSetElem(L,1)->flags=checkint(L,3);
return 0;
}

static int CvSetElem_flags(lua_State *L)
{
  lua_pushnumber(L,checkCvSetElem(L,1)->flags);
return 1;
}


static int CvSetElem_nnext_free(lua_State *L)
{
  checkCvSetElem(L,1)->next_free=checkCvSetElem(L,3);
return 0;
}

static int CvSetElem_next_free(lua_State *L)
{
  pushCvSetElem(L,checkCvSetElem(L,1)->next_free);
return 1;
}


static const luacv_method CvSetElem_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(flags,CvSetElem),
  methodReg(next_free,CvSetElem),
};

makeIndexFunctions(CvSetElem)
makeObjectCallback(CvSetElem);

