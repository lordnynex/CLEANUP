#include "CvRect.h"
static int CvRect_tostring(lua_State *L)
{
  CvRect *rect=checkCvRect(L,1);
  lua_pushfstring(L,CVRECT_NAME" object %p\n\tx=%d\n\ty=%d\n\twidth=%d\n\theight=%d",rect,rect->x,rect->y,rect->width,rect->height);

  return 1;
}

static int CvRect_gc(lua_State *L)
{

  luacv_obj<CvRect>*obj=(luacv_obj<CvRect>*)luaL_checkudata(L,1,CVRECT_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvRect_nx(lua_State *L)
{
  const char f_msg[]=CVRECT_NAME".x=int";
  checkCvRect(L,1)->x=checkint(L,3);
return 0;
}

static int CvRect_x(lua_State *L)
{
  lua_pushnumber(L,checkCvRect(L,1)->x);
return 1;
}

static int CvRect_ny(lua_State *L)
{
  const char f_msg[]=CVRECT_NAME".y=int";
  checkCvRect(L,1)->y=checkint(L,3);
return 0;
}

static int CvRect_y(lua_State *L)
{
  lua_pushnumber(L,checkCvRect(L,1)->y);
return 1;
}
static int CvRect_nwidth(lua_State *L)
{
  const char f_msg[]=CVRECT_NAME".width=int";
  checkCvRect(L,1)->width=checkint(L,3);
return 0;
}

static int CvRect_width(lua_State *L)
{
  lua_pushnumber(L,checkCvRect(L,1)->width);
return 1;
}

static int CvRect_nheight(lua_State *L)
{
  const char f_msg[]=CVRECT_NAME".height=int";
  checkCvRect(L,1)->height=checkint(L,3);
return 0;
}

static int CvRect_height(lua_State *L)
{
  lua_pushnumber(L,checkCvRect(L,1)->height);
return 1;
}


static const luacv_method CvRect_v[]=
{
  {NULL,5,NULL,NULL},
  methodReg(height,CvRect),
  methodReg(width,CvRect),
  methodReg(x,CvRect),
  methodReg(y,CvRect),
};

makeIndexFunctions(CvRect)
makeObjectCallback(CvRect);
