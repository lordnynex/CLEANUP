#include "CvLineIterator.h"
static int CvLineIterator_tostring(lua_State *L)
{
  CvLineIterator *line=checkCvLineIterator(L,1);
  lua_pushfstring(L,CVLINEITERATOR_NAME" object %p\n\tuchar=%p\n\terr=%d\n\tplus_delta=%d\n\tminus_delta=%d\n\tplus_step=%d\n\tminus_step=%d",line,line->ptr,line->err,line->plus_delta,line->minus_delta,line->plus_step,line->minus_step);

  return 1;
}
static int CvLineIterator_gc(lua_State *L)
{

  luacv_obj<CvLineIterator>*obj=(luacv_obj<CvLineIterator>*)luaL_checkudata(L,1,CVLINEITERATOR_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvLineIterator_nerr(lua_State *L)
{
  const char f_msg[]=CVLINEITERATOR_NAME".err=int";
  checkCvLineIterator(L,1)->err=checkint(L,3);
return 0;
}

static int CvLineIterator_err(lua_State *L)
{
  lua_pushnumber(L,checkCvLineIterator(L,1)->err);
return 1;
}

static int CvLineIterator_nplus_delta(lua_State *L)
{
  const char f_msg[]=CVLINEITERATOR_NAME".plus_delta=int";
  checkCvLineIterator(L,1)->plus_delta=checkint(L,3);
return 0;
}

static int CvLineIterator_plus_delta(lua_State *L)
{
  lua_pushnumber(L,checkCvLineIterator(L,1)->plus_delta);
return 1;
}

static int CvLineIterator_nminus_delta(lua_State *L)
{
  const char f_msg[]=CVLINEITERATOR_NAME".minus_delta=int";
  checkCvLineIterator(L,1)->minus_delta=checkint(L,3);
return 0;
}

static int CvLineIterator_minus_delta(lua_State *L)
{
  lua_pushnumber(L,checkCvLineIterator(L,1)->minus_delta);
return 1;
}


static int CvLineIterator_nplus_step(lua_State *L)
{
  const char f_msg[]=CVLINEITERATOR_NAME".plus_step=int";
  checkCvLineIterator(L,1)->plus_step=checkint(L,3);
return 0;
}

static int CvLineIterator_plus_step(lua_State *L)
{
  lua_pushnumber(L,checkCvLineIterator(L,1)->plus_step);
return 1;
}


static int CvLineIterator_nminus_step(lua_State *L)
{
  const char f_msg[]=CVLINEITERATOR_NAME".minus_step=int";
  checkCvLineIterator(L,1)->minus_step=checkint(L,3);
return 0;
}

static int CvLineIterator_minus_step(lua_State *L)
{
  lua_pushnumber(L,checkCvLineIterator(L,1)->minus_step);
return 1;
}

static int CvLineIterator_nptr(lua_State *L)
{
  const char f_msg[]=CVLINEITERATOR_NAME".ptr=userdata";
  checkCvLineIterator(L,1)->ptr=(uchar*)checkldata(L,3);
return 0;
}

static int CvLineIterator_ptr(lua_State *L)
{
  pushUserData(L,checkCvLineIterator(L,1)->ptr);
return 1;
}


static const luacv_method CvLineIterator_v[]=
{
  {NULL,7,NULL,NULL},
  methodReg(err,CvLineIterator),
  methodReg(minus_delta,CvLineIterator),
  methodReg(minus_step,CvLineIterator),
  methodReg(plus_delta,CvLineIterator),
  methodReg(plus_step,CvLineIterator),
  methodReg(ptr,CvLineIterator),
};

makeIndexFunctions(CvLineIterator)
makeObjectCallback(CvLineIterator);
