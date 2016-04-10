#include "CvLatentSvmDetector.h"
static int CvLatentSvmDetector_tostring(lua_State *L)
{
  CvLatentSvmDetector *e=checkCvLatentSvmDetector(L,1);
  lua_pushfstring(L,CVLATENTSVMDETECTOR_NAME" object %p\n\t"CVLSVMFILTEROBJECT_NAME" filers=%p\n\tint num_filter=%d\n\tint num_components=%d\n\tint num_part_filters=%p\n\tnum b=%p\n\tnum score_threshold",e,e->filters,e->num_filters,e->num_components,e->num_part_filters,e->b,e->score_threshold);
  return 1;
}

static int CvLatentSvmDetector_gc(lua_State *L)
{
  luacv_obj<CvLatentSvmDetector>*obj=(luacv_obj<CvLatentSvmDetector>*)luaL_checkudata(L,1,CVLATENTSVMDETECTOR_NAME);
  if (obj->dealloc_data)
    cvReleaseLatentSvmDetector(&(obj->data)); 
  return 0;
}

static int CvLatentSvmDetector_num_filters(lua_State *L)
{
    lua_pushnumber(L,checkCvLatentSvmDetector(L,1)->num_filters);
    return 1;
}

static int CvLatentSvmDetector_nnum_filters(lua_State *L)
{
    const char f_msg[]=CVLATENTSVMDETECTOR_NAME".num_filters=int";
    checkCvLatentSvmDetector(L,1)->num_filters=checkint(L,3);
    return 0;
}
static int CvLatentSvmDetector_num_components(lua_State *L)
{
    lua_pushnumber(L,checkCvLatentSvmDetector(L,1)->num_components);
    return 1;
}

static int CvLatentSvmDetector_nnum_components(lua_State *L)
{
    const char f_msg[]=CVLATENTSVMDETECTOR_NAME".num_components=int";
    checkCvLatentSvmDetector(L,1)->num_components=checkint(L,3);
    return 0;
}

static int CvLatentSvmDetector_score_threshold(lua_State *L)
{
    lua_pushnumber(L,checkCvLatentSvmDetector(L,1)->score_threshold);
    return 1;
}

static int CvLatentSvmDetector_nscore_threshold(lua_State *L)
{
    const char f_msg[]=CVLATENTSVMDETECTOR_NAME".score_threshold=num";
    checkCvLatentSvmDetector(L,1)->score_threshold=(float)checknumber(L,3);
    return 0;
}

static int CvLatentSvmDetector_num_part_filters(lua_State *L)
{
    UNIMPL(L);
    return 1;
}

static int CvLatentSvmDetector_nnum_part_filters(lua_State *L)
{
    UNIMPL(L);
    return 1;
}
static int CvLatentSvmDetector_filters(lua_State *L)
{
    UNIMPL(L);
    return 1;
}

static int CvLatentSvmDetector_nfilters(lua_State *L)
{
    UNIMPL(L);
    return 1;
}
static int CvLatentSvmDetector_b(lua_State *L)
{
    UNIMPL(L);
    return 1;
}

static int CvLatentSvmDetector_nb(lua_State *L)
{
    UNIMPL(L);
    return 1;
}

static const luacv_method CvLatentSvmDetector_v[]=
{
  {NULL,7,NULL,NULL},
  methodReg(b,CvLatentSvmDetector),
  methodReg(filters,CvLatentSvmDetector),
  methodReg(num_filters,CvLatentSvmDetector),
  methodReg(num_components,CvLatentSvmDetector),
  methodReg(num_part_filters,CvLatentSvmDetector),
  methodReg(score_threshold,CvLatentSvmDetector),
};

makeIndexFunctions(CvLatentSvmDetector)
makeObjectCallback(CvLatentSvmDetector);
