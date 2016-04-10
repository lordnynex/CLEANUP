#include "CvPOSITObject.h"

void pushCvPOSITObject(lua_State *L,CvPOSITObject *data)
{
  luacv_obj<void> *obj=(luacv_obj<void>*)lua_newuserdata(L,sizeof(luacv_obj<void>));
  obj->data=data;
  luaL_getmetatable(L,CVPOSITOBJECT_NAME);
  lua_setmetatable(L,-2);
}


static int CvPOSITObject_tostring(lua_State*L)
{
  lua_pushfstring(L,CVPOSITOBJECT_NAME" object %p\n",checkCvPOSITObject(L,1));
  return 1;
}

static int CvPOSITObject_gc(lua_State*L)
{
  luacv_obj<CvPOSITObject>*obj=(luacv_obj<CvPOSITObject>*)luaL_checkudata(L,1,CVPOSITOBJECT_NAME);
  if (obj->dealloc_data)
    cvReleasePOSITObject(&obj->data);

  return 0;
}

const luaL_Reg CvPOSITObject_m[]=
{
  {"__tostring",CvPOSITObject_tostring},
  {"__gc",CvPOSITObject_gc},
  {NULL,NULL}
};


