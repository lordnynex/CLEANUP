#include "CvHaarStageClassifier.h"

static int CvHaarStageClassifier_tostring(lua_State *L)
{
  CvHaarStageClassifier *p=checkCvHaarStageClassifier(L,1);
  lua_pushfstring(L,CVHAARSTAGECLASSIFIER_NAME" object: %p\n\tcount=%d\n\tthreshold=%d\n\tclassifier="CVHAARCLASSIFIER_NAME"\n\tnext=%d\n\tchild=%d\n\tparent=%d",p,p->count,p->threshold,p->next,p->child,p->parent);
  return 1;
}

static int CvHaarStageClassifier_gc(lua_State *L)
{
  luacv_obj<CvHaarStageClassifier>*obj=(luacv_obj<CvHaarStageClassifier>*)luaL_checkudata(L,1,CVHAARSTAGECLASSIFIER_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvHaarStageClassifier_ncount(lua_State *L)
{
  const char f_msg[]=CVHAARSTAGECLASSIFIER_NAME".count=int";
  checkCvHaarStageClassifier(L,1)->count=checkint(L,3);
return 0;
}

static int CvHaarStageClassifier_count(lua_State *L)
{
  lua_pushnumber(L,checkCvHaarStageClassifier(L,1)->count);
return 1;
}
static int CvHaarStageClassifier_nthreshold(lua_State *L)
{
  const char f_msg[]=CVHAARSTAGECLASSIFIER_NAME".threshold=num";
  checkCvHaarStageClassifier(L,1)->threshold=(float)checknumber(L,3);
return 0;
}

static int CvHaarStageClassifier_threshold(lua_State *L)
{
  lua_pushnumber(L,checkCvHaarStageClassifier(L,1)->threshold);
return 1;
}

static int CvHaarStageClassifier_nclassifier(lua_State *L)
{
  checkCvHaarStageClassifier(L,1)->classifier=checkCvHaarClassifier(L,3);
return 0;
}

static int CvHaarStageClassifier_classifier(lua_State *L)
{
  pushCvHaarClassifier(L,checkCvHaarStageClassifier(L,1)->classifier);
return 1;
}

static int CvHaarStageClassifier_nnext(lua_State *L)
{
  const char f_msg[]=CVHAARSTAGECLASSIFIER_NAME".next=int";
  checkCvHaarStageClassifier(L,1)->next=checkint(L,3);
return 0;
}

static int CvHaarStageClassifier_next(lua_State *L)
{
  lua_pushnumber(L,checkCvHaarStageClassifier(L,1)->next);
return 1;
}

static int CvHaarStageClassifier_nchild(lua_State *L)
{
  const char f_msg[]=CVHAARSTAGECLASSIFIER_NAME".child=int";
  checkCvHaarStageClassifier(L,1)->child=checkint(L,3);
return 0;
}

static int CvHaarStageClassifier_child(lua_State *L)
{
  lua_pushnumber(L,checkCvHaarStageClassifier(L,1)->child);
return 1;
}

static int CvHaarStageClassifier_nparent(lua_State *L)
{
  const char f_msg[]=CVHAARSTAGECLASSIFIER_NAME".parent=int";
  checkCvHaarStageClassifier(L,1)->parent=checkint(L,3);
return 0;
}

static int CvHaarStageClassifier_parent(lua_State *L)
{
  lua_pushnumber(L,checkCvHaarStageClassifier(L,1)->parent);
return 1;
}

static const luacv_method CvHaarStageClassifier_v[]=
{
  {NULL,7,NULL,NULL},
  methodReg(count,CvHaarStageClassifier),
  methodReg(child,CvHaarStageClassifier),
  methodReg(classifier,CvHaarStageClassifier),
  methodReg(next,CvHaarStageClassifier),
  methodReg(parent,CvHaarStageClassifier),
  methodReg(threshold,CvHaarStageClassifier),
};

makeIndexFunctions(CvHaarStageClassifier)
makeObjectCallback(CvHaarStageClassifier);
