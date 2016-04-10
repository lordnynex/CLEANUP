#include "IplTileInfo.h"

static int IplTileInfo_tostring(lua_State*L)
{
  lua_pushfstring(L,IPLTILEINFO_NAME" object %p\n",checkIplTileInfo(L,1));
  return 1;
}

static int IplTileInfo_gc(lua_State*L)
{

  luacv_obj<IplTileInfo>*obj=(luacv_obj<IplTileInfo>*)luaL_checkudata(L,1,IPLTILEINFO_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

const luaL_Reg IplTileInfo_m[]=
{
  {"__tostring",IplTileInfo_tostring},
  {"__gc",IplTileInfo_gc},
  {NULL,NULL}
};
