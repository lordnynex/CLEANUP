#include "CvFont.h"
static int CvFont_tostring(lua_State *L)
{
  CvFont *font=checkCvFont(L,1);
  lua_pushfstring(L,CVFONT_NAME" object %p\n\tfont_face=%d\n\tascii=%p\n\tgreek=%p\n\tcyrillic=%p\n\thscale=%f\n\tvscale=%f\n\tshear=%f\n\tthickness=%d\n\tdx=%p\n\tline_type=%d",font,font->font_face,font->ascii,font->greek,font->cyrillic,font->hscale,font->vscale,font->shear,font->thickness,font->dx,font->line_type);

  return 1;
}

static int CvFont_gc(lua_State *L)
{

  luacv_obj<CvFont>*obj=(luacv_obj<CvFont>*)luaL_checkudata(L,1,CVFONT_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvFont_ascii(lua_State *L)
{
  pushUserData(L,(void*)checkCvFont(L,1)->ascii);
  return 1;
}

static int CvFont_nascii(lua_State *L)
{
  UNIMPL(L);
  return 0;
}
static int CvFont_greek(lua_State *L)
{
  pushUserData(L,(void*)checkCvFont(L,1)->greek);
  return 1;
}

static int CvFont_ngreek(lua_State *L)
{
  UNIMPL(L);
  return 0;
}
static int CvFont_cyrillic(lua_State *L)
{
  pushUserData(L,(void*)checkCvFont(L,1)->cyrillic);
  return 1;
}

static int CvFont_ncyrillic(lua_State *L)
{
  UNIMPL(L);
  return 0;
}

static int CvFont_font_face(lua_State *L)
{
  lua_pushnumber(L,checkCvFont(L,1)->font_face);
  return 1;
}

static int CvFont_nfont_face(lua_State *L)
{
  const char f_msg[]=CVFONT_NAME".font_face=int";
  checkCvFont(L,1)->font_face=checkint(L,3);
  return 0;
}
static int CvFont_thickness(lua_State *L)
{
  lua_pushnumber(L,checkCvFont(L,1)->thickness);
  return 1;
}

static int CvFont_nthickness(lua_State *L)
{
  const char f_msg[]=CVFONT_NAME".thickness=int";
  checkCvFont(L,1)->thickness=checkint(L,3);
  return 0;
}
static int CvFont_line_type(lua_State *L)
{
  lua_pushnumber(L,checkCvFont(L,1)->line_type);
  return 1;
}

static int CvFont_nline_type(lua_State *L)
{
  const char f_msg[]=CVFONT_NAME".line_type=int";
  checkCvFont(L,1)->line_type=checkint(L,3);
  return 0;
}

static int CvFont_hscale(lua_State *L)
{
  lua_pushnumber(L,checkCvFont(L,1)->hscale);
  return 1;
}

static int CvFont_nhscale(lua_State *L)
{
  const char f_msg[]=CVFONT_NAME".hscale=num";
  checkCvFont(L,1)->hscale=(float)checknumber(L,3);
  return 0;
}
static int CvFont_vscale(lua_State *L)
{
  lua_pushnumber(L,checkCvFont(L,1)->vscale);
  return 1;
}

static int CvFont_nvscale(lua_State *L)
{
  const char f_msg[]=CVFONT_NAME".vscale=num";
  checkCvFont(L,1)->vscale=(float)checknumber(L,3);
  return 0;
}

static int CvFont_shear(lua_State *L)
{
  lua_pushnumber(L,checkCvFont(L,1)->shear);
  return 1;
}

static int CvFont_nshear(lua_State *L)
{
  const char f_msg[]=CVFONT_NAME".shear=num";
  checkCvFont(L,1)->shear=(float)checknumber(L,3);
  return 0;
}
static int CvFont_dx(lua_State *L)
{
  lua_pushnumber(L,checkCvFont(L,1)->dx);
  return 1;
}

static int CvFont_ndx(lua_State *L)
{
  const char f_msg[]=CVFONT_NAME".dx=num";
  checkCvFont(L,1)->dx=(float)checknumber(L,3);
  return 0;
}

static const luacv_method CvFont_v[]=
{
  {NULL,11,NULL,NULL},
   methodReg(ascii,CvFont),
   methodReg(cyrillic,CvFont),
   methodReg(dx,CvFont),
   methodReg(font_face,CvFont),
   methodReg(greek,CvFont),
   methodReg(hscale,CvFont),
   methodReg(line_type,CvFont),
   methodReg(shear,CvFont),
   methodReg(thickness,CvFont),
   methodReg(vscale,CvFont),
};

makeIndexFunctions(CvFont)
makeObjectCallback(CvFont);
