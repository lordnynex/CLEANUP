#include "CvLSHOperations.h"

void pushCvLSHOperations(lua_State *L,CvLSHOperations *data)
{
  luacv_obj<void> *obj=(luacv_obj<void>*)lua_newuserdata(L,sizeof(luacv_obj<void>));
  obj->data=data;
  luaL_getmetatable(L,CVLSHOPERATIONS_NAME);
  lua_setmetatable(L,-2);
}


static int CvLSHOperations_tostring(lua_State*L)
{
  lua_pushfstring(L,CVLSHOPERATIONS_NAME" object %p\n",checkCvLSHOperations(L,1));
  return 1;
}

static int CvLSHOperations_gc(lua_State*L)
{
  luacv_obj<CvLSHOperations>*obj=(luacv_obj<CvLSHOperations>*)luaL_checkudata(L,1,CVLSHOPERATIONS_NAME);
  if (obj->dealloc_data)
    luacv_free(&obj->data);

  return 0;
}

const luaL_Reg CvLSHOperations_m[]=
{
  {"__tostring",CvLSHOperations_tostring},
  {"__gc",CvLSHOperations_gc},
  {NULL,NULL}
};


