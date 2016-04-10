#include "CvBox2D.h"

static int CvBox2D_tostring(lua_State *L)
{
  CvBox2D *box=checkCvBox2D(L,1);
  lua_pushfstring(L,CVBOX2D_NAME" object %p\n\t"CVPOINT2D32F_NAME" center={ [x]=>%f, [y]=>%f }\n\t"CVSIZE2D32F_NAME" size={ [width]=>%f, [height]=>%f }\n\tnum angle=%f",box,box->center.x,box->center.y,box->size.width,box->size.height,box->angle);
  return 1;
}

static int CvBox2D_gc(lua_State *L)
{
  luacv_obj<CvBox2D>*obj=(luacv_obj<CvBox2D>*)luaL_checkudata(L,1,CVBOX2D_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvBox2D_nangle(lua_State *L)
{
  const char f_msg[]=CVBOX2D_NAME".angle=num";
  checkCvBox2D(L,1)->angle=(float)checknumber(L,3);
return 0;
}

static int CvBox2D_angle(lua_State *L)
{
  lua_pushnumber(L,checkCvBox2D(L,1)->angle);
return 1;
}

static int CvBox2D_ncenter(lua_State *L)
{
  checkCvBox2D(L,1)->center=*checkCvPoint2D32f(L,3);
return 0;
}

static int CvBox2D_center(lua_State *L)
{
  pushCvPoint2D32f(L,&checkCvBox2D(L,1)->center);
return 1;
}
static int CvBox2D_nsize(lua_State *L)
{
  checkCvBox2D(L,1)->size=*checkCvSize2D32f(L,3);
return 0;
}

static int CvBox2D_size(lua_State *L)
{
  pushCvSize2D32f(L,&checkCvBox2D(L,1)->size);
return 1;
}

static const luacv_method CvBox2D_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(angle,CvBox2D),
  methodReg(center,CvBox2D),
  methodReg(size,CvBox2D),
};

makeIndexFunctions(CvBox2D)
makeObjectCallback(CvBox2D);

