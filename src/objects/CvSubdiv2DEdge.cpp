#include "CvSubdiv2DEdge.h"

static int CvSubdiv2DEdge_tostring(lua_State*L)
{
  lua_pushfstring(L,CVSUBDIV2DEDGE_NAME" object %p\n",checkCvSubdiv2DEdge(L,1));
  return 1;
}

static int CvSubdiv2DEdge_gc(lua_State*L)
{
  luacv_obj<CvSubdiv2DEdge>*obj=(luacv_obj<CvSubdiv2DEdge>*)luaL_checkudata(L,1,CVSUBDIV2DEDGE_NAME);
  if (obj->dealloc_data)
    luacv_free(&obj->data);

  return 0;
}

const luaL_Reg CvSubdiv2DEdge_m[]=
{
  {"__tostring",CvSubdiv2DEdge_tostring},
  {"__gc",CvSubdiv2DEdge_gc},
  {NULL,NULL}
};


