#include "Cv3dTrackerTrackedObject.h"

static int Cv3dTrackerTrackedObject_tostring(lua_State *L)
{
  Cv3dTrackerTrackedObject *box=checkCv3dTrackerTrackedObject(L,1);
  lua_pushfstring(L,CV3DTRACKERTRACKEDOBJECT_NAME" object %p\n\tint id=%d\n\t"CVPOINT3D32F_NAME" p={ [x]=>%f, [y]=>%f, [z]=>%f }",box,box->id,box->p.x,box->p.y,box->p.z);
  return 1;
}

static int Cv3dTrackerTrackedObject_gc(lua_State *L)
{
  luacv_obj<Cv3dTrackerTrackedObject>*obj=(luacv_obj<Cv3dTrackerTrackedObject>*)luaL_checkudata(L,1,CV3DTRACKERTRACKEDOBJECT_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int Cv3dTrackerTrackedObject_nid(lua_State *L)
{
  const char f_msg[]=CV3DTRACKERTRACKEDOBJECT_NAME".id=int";
  checkCv3dTrackerTrackedObject(L,1)->id=checkint(L,3);
return 0;
}

static int Cv3dTrackerTrackedObject_id(lua_State *L)
{
  lua_pushnumber(L,checkCv3dTrackerTrackedObject(L,1)->id);
return 1;
}

static int Cv3dTrackerTrackedObject_np(lua_State *L)
{
  checkCv3dTrackerTrackedObject(L,1)->p=*checkCvPoint3D32f(L,3);
return 0;
}

static int Cv3dTrackerTrackedObject_p(lua_State *L)
{
  pushCvPoint3D32f(L,&checkCv3dTrackerTrackedObject(L,1)->p);
return 1;
}

static const luacv_method Cv3dTrackerTrackedObject_v[]=
{
  {NULL,3,NULL,NULL},
  methodReg(id,Cv3dTrackerTrackedObject),
  methodReg(p,Cv3dTrackerTrackedObject),
};

makeIndexFunctions(Cv3dTrackerTrackedObject)
makeObjectCallback(Cv3dTrackerTrackedObject);

