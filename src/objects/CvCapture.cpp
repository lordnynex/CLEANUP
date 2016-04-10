#include "CvCapture.h"

void pushCvCapture(lua_State *L,CvCapture *data)
{
  luacv_obj<void> *obj=(luacv_obj<void>*)lua_newuserdata(L,sizeof(luacv_obj<void>));
  obj->data=data;
  luaL_getmetatable(L,CVCAPTURE_NAME);
  lua_setmetatable(L,-2);
}


static int CvCapture_tostring(lua_State*L)
{
  lua_pushfstring(L,CVCAPTURE_NAME" object %p\n",checkCvCapture(L,1));
  return 1;
}

static int CvCapture_gc(lua_State*L)
{
  luacv_obj<CvCapture>*obj=(luacv_obj<CvCapture>*)luaL_checkudata(L,1,CVCAPTURE_NAME);
  if (obj->dealloc_data)
    cvReleaseCapture(&obj->data);

  return 0;
}

const luaL_Reg CvCapture_m[]=
{
  {"__tostring",CvCapture_tostring},
  {"__gc",CvCapture_gc},
  {NULL,NULL}
};


