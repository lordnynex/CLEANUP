#include "CvFeatureTree.h"

void pushCvFeatureTree(lua_State *L,CvFeatureTree *data)
{
  luacv_obj<void> *obj=(luacv_obj<void>*)lua_newuserdata(L,sizeof(luacv_obj<void>));
  obj->data=data;
  luaL_getmetatable(L,CVFEATURETREE_NAME);
  lua_setmetatable(L,-2);
}


static int CvFeatureTree_tostring(lua_State*L)
{
  lua_pushfstring(L,CVFEATURETREE_NAME" object %p\n",checkCvFeatureTree(L,1));
  return 1;
}

static int CvFeatureTree_gc(lua_State*L)
{
  luacv_obj<CvFeatureTree>*obj=(luacv_obj<CvFeatureTree>*)luaL_checkudata(L,1,CVFEATURETREE_NAME);
  if (obj->dealloc_data)
    cvReleaseFeatureTree(obj->data);

  return 0;
}

const luaL_Reg CvFeatureTree_m[]=
{
  {"__tostring",CvFeatureTree_tostring},
  {"__gc",CvFeatureTree_gc},
  {NULL,NULL}
};


