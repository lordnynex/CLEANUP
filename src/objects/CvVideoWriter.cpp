#include "CvVideoWriter.h"

void pushCvVideoWriter(lua_State *L,CvVideoWriter *data)
{
  luacv_obj<void> *obj=(luacv_obj<void>*)lua_newuserdata(L,sizeof(luacv_obj<void>));
  obj->data=data;
  luaL_getmetatable(L,CVVIDEOWRITER_NAME);
  lua_setmetatable(L,-2);
}


static int CvVideoWriter_tostring(lua_State*L)
{
  lua_pushfstring(L,CVVIDEOWRITER_NAME" object %p\n",checkCvVideoWriter(L,1));
  return 1;
}

static int CvVideoWriter_gc(lua_State*L)
{
  luacv_obj<CvVideoWriter>*obj=(luacv_obj<CvVideoWriter>*)luaL_checkudata(L,1,CVVIDEOWRITER_NAME);
  if (obj->dealloc_data)
    cvReleaseVideoWriter(&obj->data);
  return 0;
}

const luaL_Reg CvVideoWriter_m[]=
{
  {"__tostring",CvVideoWriter_tostring},
  {"__gc",CvVideoWriter_gc},
  {NULL,NULL}
};


