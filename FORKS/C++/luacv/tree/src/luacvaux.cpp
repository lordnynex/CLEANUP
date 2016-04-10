#include "luacvaux.h"

void luacv_objTableReg (lua_State *L,const luacv_var* var)
{
  for (;var->name;var++)
  {
      lua_pushstring(L,var->name); 
      lua_newtable(L);
      lua_pushstring(L,"name"); 
      lua_pushstring(L,var->name); 
      lua_rawset(L,-3);
      lua_pushstring(L,"size"); 
      lua_pushnumber(L,(int)var->value);
      lua_rawset(L,-3);
      lua_pushstring(L,"meta"); 
      luaL_getmetatable(L,var->name);
      lua_rawset(L,-3);
      lua_rawset(L,-3);
  }
}

void luacv_tableConvFuncReg (lua_State *L,const luaL_ConvFunc* var)
{
  for (;var->name;var++)
  {
      lua_pushstring(L,var->name); 
      lua_newtable(L);
      lua_pushstring(L,"name"); 
      lua_pushstring(L,var->name); 
      lua_rawset(L,-3);
      lua_pushstring(L,"value"); 
      lua_pushnumber(L,(int)var->len);
      lua_rawset(L,-3);
      lua_pushstring(L,"to"); 
      lua_pushcfunction(L,var->newindex);
      lua_rawset(L,-3);
      lua_pushstring(L,"from"); 
      lua_pushcfunction(L,var->index);
      lua_rawset(L,-3);
      lua_rawset(L,-3);
  }
}

void luacv_constReg (lua_State *L,const luacv_var* var)
{
  for (;var->name;var++)
  {
      lua_pushstring(L,var->name); 
      lua_pushnumber(L,var->value);
      lua_rawset(L,-3);
  }
}

int luacv_argerror (lua_State *L, int narg, const char *extramsg,const char *funcmsg) {
  lua_Debug ar;
  if (!lua_getstack(L, 0, &ar))  /* no stack frame? */
    return luaL_error(L, "bad argument #%d to '%s' [%s]",narg, funcmsg, extramsg);

  lua_getinfo(L, "n", &ar);
  if (strcmp(ar.namewhat, "method") == 0) {
    narg--;  /* do not count `self' */
    if (narg == 0)  /* error is in the self argument itself? */
      return luaL_error(L, "calling " LUA_QS " on bad self %s",ar.name, extramsg);
  }
  if (ar.name == NULL)
    ar.name = "?";
  return luaL_error(L, "bad argument #%d to '%s' [%s]",narg, (funcmsg!=NULL?funcmsg:LUA_QS), extramsg);
}

void *luacv_checkudata (lua_State *L, int ud, const char *tname,const char *funcparam, int metatable) {
  //void *p = lua_touserdata(L, ud);
  void *p=(void*)((luacv_obj<void>*)lua_touserdata(L,ud))->data;

  if (p != NULL) {  /* value is a userdata? */
    if (!metatable) return p;
    if (lua_getmetatable(L, ud)) {  /* does it have a metatable? */
      lua_getfield(L, LUA_REGISTRYINDEX, tname);  /* get correct metatable */
      if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
        lua_pop(L, 2);  /* remove both metatables */
        return p;
      }
    }
  }
  luacv_argerror(L,ud, lua_pushfstring(L, "%s expected, got %s",(metatable?tname:lua_typename(L,LUA_TUSERDATA)), luaL_typename(L, ud)),funcparam);
  return NULL;  /* to avoid warnings */
}


const char* luacv_checkstring(lua_State *L,int narg,const char *funcparam)
{
  const char *s=lua_tostring(L,narg);
  if (!s)  /* avoid extra test when d is not 0 */
    luacv_tag_error(L,narg,LUA_TSTRING,funcparam);
  return s;
}

lua_Integer luacv_checkint(lua_State *L,int narg,const char *funcparam)
{
  lua_Integer d = lua_tointeger(L, narg);
  if (d == 0 && !lua_isnumber(L, narg))  /* avoid extra test when d is not 0 */
    luacv_tag_error(L,narg,LUA_TNUMBER,funcparam);
  return d;
}

lua_Number luacv_checknumber(lua_State *L,int narg,const char *funcparam)
{
  lua_Number d = lua_tonumber(L, narg);
  if (d == 0 && !lua_isnumber(L, narg))  /* avoid extra test when d is not 0 */
    luacv_tag_error(L,narg,LUA_TNUMBER,funcparam);
  return d;
}

int luacv_checkmetaname(char **name,unsigned int len)
{
  int i_pos;
  char *pos=NULL,tmp[]="\0\0\0\0",*t_name=*name;
  if ((name!=NULL)&&(len>0))
    if (((pos=strrchr(t_name,'['))!=NULL)&&(t_name[len-1]==']'))
    {
      i_pos=pos-t_name; 
      strncpy(tmp,(const char*)((size_t)t_name+(i_pos+1)),len-i_pos-2);
      char *newname=(char*)luacv_alloc((i_pos+1));
      strncpy(newname,t_name,i_pos);
      newname[i_pos]=0;
      *name=newname;
      if (tmp[0]=='\0') return -1;
      return atoi(tmp); 
    }
  return 1;
}
int luacv_methodSearch(const char *pattern,const struct luacv_method * table)
{//table have to be alphabeticaly sorted
  if ((!table)||(!pattern)) return -1;
  struct luacv_method *var=(struct luacv_method*)table;
  unsigned char *s1=(unsigned char*)pattern,*s2=NULL;
  size_t left=1,middle,right=table[0].len,len=strlen(pattern);

  while(right!=(left+1))
  {//binary dijkstr's search
    middle=(left+right)/2; 
    if (*table[middle].name<=*s1) left=middle;
    else 
        right=middle;
  }

  for(var=(struct luacv_method*)&table[left];((s2=(unsigned char*)var->name)&&(*(s1=(unsigned char*)pattern)==*s2));var--)
  {//sequence search
    if (var->len!=len) continue;
    do
      if (!(*(++s1))) return var-table;
    while((*(s1))==*(++s2));
  }

  return -1;
}

