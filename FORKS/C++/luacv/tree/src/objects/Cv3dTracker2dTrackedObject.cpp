#include "Cv3dTracker2dTrackedObject.h"

static int Cv3dTracker2dTrackedObject_tostring(lua_State *L)
{
  Cv3dTracker2dTrackedObject *box=checkCv3dTracker2dTrackedObject(L,1);
  lua_pushfstring(L,CV3DTRACKER2DTRACKEDOBJECT_NAME" object %p\n\tint id=%d\n\t"CVPOINT2D32F_NAME" p={ [x]=>%f, [y]=>%f }",box,box->id,box->p.x,box->p.y);
  return 1;
}

static int Cv3dTracker2dTrackedObject_gc(lua_State *L)
{
  luacv_obj<Cv3dTracker2dTrackedObject>*obj=(luacv_obj<Cv3dTracker2dTrackedObject>*)luaL_checkudata(L,1,CV3DTRACKER2DTRACKEDOBJECT_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int Cv3dTracker2dTrackedObject_nid(lua_State *L)
{
  const char f_msg[]=CV3DTRACKER2DTRACKEDOBJECT_NAME".id=int";
  checkCv3dTracker2dTrackedObject(L,1)->id=checkint(L,3);
return 0;
}

static int Cv3dTracker2dTrackedObject_id(lua_State *L)
{
  lua_pushnumber(L,checkCv3dTracker2dTrackedObject(L,1)->id);
return 1;
}

static int Cv3dTracker2dTrackedObject_np(lua_State *L)
{
  checkCv3dTracker2dTrackedObject(L,1)->p=*checkCvPoint2D32f(L,3);
return 0;
}

static int Cv3dTracker2dTrackedObject_p(lua_State *L)
{
  pushCvPoint2D32f(L,&checkCv3dTracker2dTrackedObject(L,1)->p);
return 1;
}

static const luacv_method Cv3dTracker2dTrackedObject_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(id,Cv3dTracker2dTrackedObject),
  methodReg(p,Cv3dTracker2dTrackedObject),
};

makeIndexFunctions(Cv3dTracker2dTrackedObject)
makeObjectCallback(Cv3dTracker2dTrackedObject);

