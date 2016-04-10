#include "CvTermCriteria.h"

static int CvTermCriteria_tostring(lua_State *L)
{
  CvTermCriteria *term=checkCvTermCriteria(L,1);
  lua_pushfstring(L,CVTERMCRITERIA_NAME" object %p\n\ttype=%d\n\tmax_iter=%d\n\tepsilon=%d",term,term->type,term->max_iter,term->epsilon);

  return 1;
}

static int CvTermCriteria_gc(lua_State *L)
{

  luacv_obj<CvTermCriteria>*obj=(luacv_obj<CvTermCriteria>*)luaL_checkudata(L,1,CVTERMCRITERIA_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvTermCriteria_ntype(lua_State *L)
{
  const char f_msg[]=CVTERMCRITERIA_NAME".type=int";
  checkCvTermCriteria(L,1)->type=checkint(L,3);
return 0;
}

static int CvTermCriteria_type(lua_State *L)
{
  lua_pushnumber(L,checkCvTermCriteria(L,1)->type);
return 1;
}

static int CvTermCriteria_nmax_iter(lua_State *L)
{
  const char f_msg[]=CVTERMCRITERIA_NAME".max_iter=int";
  checkCvTermCriteria(L,1)->max_iter=checkint(L,3);
return 0;
}

static int CvTermCriteria_max_iter(lua_State *L)
{
  lua_pushnumber(L,checkCvTermCriteria(L,1)->max_iter);
return 1;
}

static int CvTermCriteria_nepsilon(lua_State *L)
{
  const char f_msg[]=CVTERMCRITERIA_NAME".epsilon=num";
  checkCvTermCriteria(L,1)->epsilon=checknumber(L,3);
return 0;
}

static int CvTermCriteria_epsilon(lua_State *L)
{
  lua_pushnumber(L,checkCvTermCriteria(L,1)->epsilon);
return 1;
}


static const luacv_method CvTermCriteria_v[]=
{
  {NULL,4,NULL,NULL},
  methodReg(epsilon,CvTermCriteria),
  methodReg(max_iter,CvTermCriteria),
  methodReg(type,CvTermCriteria),

};

makeIndexFunctions(CvTermCriteria)
makeObjectCallback(CvTermCriteria);


