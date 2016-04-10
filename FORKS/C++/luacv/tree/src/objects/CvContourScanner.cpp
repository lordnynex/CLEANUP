#include "CvContourScanner.h"

void pushCvContourScanner(lua_State *L,CvContourScanner *data)
{
  luacv_obj<void> *obj=(luacv_obj<void>*)lua_newuserdata(L,sizeof(luacv_obj<void>));
  obj->data=data;
  luaL_getmetatable(L,CVCONTOURSCANNER_NAME);
  lua_setmetatable(L,-2);
}


static int CvContourScanner_tostring(lua_State*L)
{
  lua_pushfstring(L,CVCONTOURSCANNER_NAME" object %p\n",checkCvContourScanner(L,1));
  return 1;
}

static int CvContourScanner_gc(lua_State*L)
{
  luacv_obj<CvContourScanner>*obj=(luacv_obj<CvContourScanner>*)luaL_checkudata(L,1,CVCONTOURSCANNER_NAME);
  if (obj->dealloc_data)
    luacv_free(&obj->data);

  return 0;
}

const luaL_Reg CvContourScanner_m[]=
{
  {"__tostring",CvContourScanner_tostring},
  {"__gc",CvContourScanner_gc},
  {NULL,NULL}
};


