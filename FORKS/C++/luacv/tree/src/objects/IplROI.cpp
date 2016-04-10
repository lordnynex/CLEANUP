#include "IplROI.h"

static int IplROI_tostring(lua_State *L)
{
  IplROI *roi=checkIplROI(L,1);
  lua_pushfstring(L,IPLROI_NAME" object %p\n\tcoi=%d\n\txOffset=%d\n\tyOffset=%d\n\twidth=%d\n\theight=%d",roi,roi->coi,roi->xOffset,roi->yOffset,roi->width,roi->height);

  return 1;
}

static int IplROI_gc(lua_State *L)
{

  luacv_obj<IplROI>*obj=(luacv_obj<IplROI>*)luaL_checkudata(L,1,IPLROI_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}
static int IplROI_ncoi(lua_State *L)
{
  const char f_msg[]=IPLROI_NAME".coi=int";
  checkIplROI(L,1)->coi=checkint(L,3);
return 0;
}

static int IplROI_coi(lua_State *L)
{
  lua_pushnumber(L,checkIplROI(L,1)->coi);
return 1;
}


static int IplROI_nxOffset(lua_State *L)
{
  const char f_msg[]=IPLROI_NAME".xOffset=int";
  checkIplROI(L,1)->xOffset=checkint(L,3);
return 0;
}

static int IplROI_xOffset(lua_State *L)
{
  lua_pushnumber(L,checkIplROI(L,1)->xOffset);
return 1;
}


static int IplROI_nyOffset(lua_State *L)
{
  const char f_msg[]=IPLROI_NAME".yOffset=int";
  checkIplROI(L,1)->yOffset=checkint(L,3);
return 0;
}

static int IplROI_yOffset(lua_State *L)
{
  lua_pushnumber(L,checkIplROI(L,1)->yOffset);
return 1;
}


static int IplROI_nwidth(lua_State *L)
{
  const char f_msg[]=IPLROI_NAME".width=int";
  checkIplROI(L,1)->width=checkint(L,3);
return 0;
}

static int IplROI_width(lua_State *L)
{
  lua_pushnumber(L,checkIplROI(L,1)->width);
return 1;
}


static int IplROI_nheight(lua_State *L)
{
  const char f_msg[]=IPLROI_NAME".height=int";
  checkIplROI(L,1)->height=checkint(L,3);
return 0;
}

static int IplROI_height(lua_State *L)
{
  lua_pushnumber(L,checkIplROI(L,1)->height);
return 1;
}


static const luacv_method IplROI_v[]=
{
  {NULL,6,NULL,NULL},
  methodReg(coi,IplROI),
  methodReg(height,IplROI),
  methodReg(width,IplROI),
  methodReg(xOffset,IplROI),
  methodReg(yOffset,IplROI),
};

makeIndexFunctions(IplROI)
makeObjectCallback(IplROI);
