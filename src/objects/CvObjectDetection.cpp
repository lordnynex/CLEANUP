#include "CvObjectDetection.h"

static int CvObjectDetection_tostring(lua_State *L)
{
  CvObjectDetection *o=checkCvObjectDetection(L,1);
  lua_pushfstring(L,CVOBJECTDETECTION_NAME" object %p\n\t"CVRECT_NAME" rect={ [x]=>%d, [y]=>%d, [width]=>%d, [height]=>%d }\n\tnum score=%f",o,o->rect.x,o->rect.y,o->rect.width,o->rect.height,o->score);
  return 1;
}

static int CvObjectDetection_gc(lua_State *L)
{
  luacv_obj<CvObjectDetection>*obj=(luacv_obj<CvObjectDetection>*)luaL_checkudata(L,1,CVOBJECTDETECTION_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvObjectDetection_nscore(lua_State *L)
{
  const char f_msg[]=CVOBJECTDETECTION_NAME".score=num";
  checkCvObjectDetection(L,1)->score=(float)checknumber(L,3);
return 0;
}

static int CvObjectDetection_score(lua_State *L)
{
  lua_pushnumber(L,checkCvObjectDetection(L,1)->score);
return 1;
}

static int CvObjectDetection_nrect(lua_State *L)
{
  checkCvObjectDetection(L,1)->rect=*checkCvRect(L,3);
return 0;
}

static int CvObjectDetection_rect(lua_State *L)
{
  pushCvRect(L,&checkCvObjectDetection(L,1)->rect);
return 1;
}


static const luacv_method CvObjectDetection_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(rect,CvObjectDetection),
  methodReg(score,CvObjectDetection),
};

makeIndexFunctions(CvObjectDetection)
makeObjectCallback(CvObjectDetection);

