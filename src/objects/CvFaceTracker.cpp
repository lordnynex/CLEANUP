#include "CvFaceTracker.h"

void pushCvFaceTracker(lua_State *L,CvFaceTracker *data)
{
  luacv_obj<void> *obj=(luacv_obj<void>*)lua_newuserdata(L,sizeof(luacv_obj<void>));
  obj->data=data;
  luaL_getmetatable(L,CVFACETRACKER_NAME);
  lua_setmetatable(L,-2);
}


static int CvFaceTracker_tostring(lua_State*L)
{
  lua_pushfstring(L,CVFACETRACKER_NAME" object %p\n",checkCvFaceTracker(L,1));
  return 1;
}

static int CvFaceTracker_gc(lua_State*L)
{
  luacv_obj<CvFaceTracker>*obj=(luacv_obj<CvFaceTracker>*)luaL_checkudata(L,1,CVFACETRACKER_NAME);
  if (obj->dealloc_data)
    cvReleaseFaceTracker(&obj->data);

  return 0;
}

const luaL_Reg CvFaceTracker_m[]=
{
  {"__tostring",CvFaceTracker_tostring},
  {"__gc",CvFaceTracker_gc},
  {NULL,NULL}
};


