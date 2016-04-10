#include "CvGLCM.h"

void pushCvGLCM(lua_State *L,CvGLCM *data)
{
  luacv_obj<void> *obj=(luacv_obj<void>*)lua_newuserdata(L,sizeof(luacv_obj<void>));
  obj->data=data;
  luaL_getmetatable(L,CVGLCM_NAME);
  lua_setmetatable(L,-2);
}


static int CvGLCM_tostring(lua_State*L)
{
  lua_pushfstring(L,CVGLCM_NAME" object %p\n",checkCvGLCM(L,1));
  return 1;
}

static int CvGLCM_gc(lua_State*L)
{
  luacv_obj<CvGLCM>*obj=(luacv_obj<CvGLCM>*)luaL_checkudata(L,1,CVGLCM_NAME);
  if (obj->dealloc_data)
    cvReleaseGLCM(&obj->data);

  return 0;
}

const luaL_Reg CvGLCM_m[]=
{
  {"__tostring",CvGLCM_tostring},
  {"__gc",CvGLCM_gc},
  {NULL,NULL}
};


