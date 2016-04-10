#include "CvContour.h"

static int CvContour_tostring(lua_State *L)
{
  CvContour *seq=checkCvContour(L,1);
  lua_pushfstring(L,CVCONTOUR_NAME" object %p\n\tflags=%d\n\theader_size=%d\n\th_prev="CVSEQ_NAME"\n\th_next="CVSEQ_NAME"\n\tv_prev="CVSEQ_NAME"\n\tv_next="CVSEQ_NAME"\n\ttotal=%d\n\telem_size=%d\n\tblock_max=%p\n\tptr=%p\n\tdelta_elems=%d\n\tstorage="CVMEMSTORAGE_NAME"\n\tfree_blocks="CVSEQBLOCK_NAME"\n\tfirst="CVSEQBLOCK_NAME"\n\trect="CVRECT_NAME"\n\tcolor=%d\n\treserved={%d %d %d}",seq,seq->flags,seq->header_size,seq->total,seq->elem_size,seq->block_max,seq->ptr,seq->delta_elems,seq->color,seq->reserved[0],seq->reserved[1],seq->reserved[2]);
  return 1;
}

static int CvContour_gc(lua_State *L)
{

  luacv_obj<CvContour>*obj=(luacv_obj<CvContour>*)luaL_checkudata(L,1,CVCONTOUR_NAME);
  if (obj->dealloc_data)
    cvClearSeq((CvSeq*)obj->data);
  return 0;
}

static int CvContour_nblock_max(lua_State *L)
{
  const char f_msg[]=CVCONTOUR_NAME".block_max=userdata";
  checkCvContour(L,1)->block_max=(schar*)checkldata(L,3);
  return 0;
}

static int CvContour_block_max(lua_State *L)
{
  pushUserData(L,checkCvContour(L,1)->block_max);
  return 1;
}

static int CvContour_ncolor(lua_State *L)
{
  const char f_msg[]=CVCONTOUR_NAME".color=int";
  checkCvContour(L,1)->color=checkint(L,3);
  return 0;
}

static int CvContour_color(lua_State *L)
{
  lua_pushnumber(L,checkCvContour(L,1)->color);
  return 1;
}

static int CvContour_ndelta_elems(lua_State *L)
{
  const char f_msg[]=CVCONTOUR_NAME".delta_elems=int";
  checkCvContour(L,1)->delta_elems=checkint(L,3);
  return 0;
}

static int CvContour_delta_elems(lua_State *L)
{
  lua_pushnumber(L,checkCvContour(L,1)->delta_elems);
  return 1;
}

static int CvContour_nelem_size(lua_State *L)
{
  const char f_msg[]=CVCONTOUR_NAME".elem_size=int";
  checkCvContour(L,1)->elem_size=checkint(L,3);
  return 0;
}

static int CvContour_elem_size(lua_State *L)
{
  lua_pushnumber(L,checkCvContour(L,1)->elem_size);
  return 1;
}

static int CvContour_nflags(lua_State *L)
{
  const char f_msg[]=CVCONTOUR_NAME".flags=int";
  checkCvContour(L,1)->flags=checkint(L,3);
  return 0;
}

static int CvContour_flags(lua_State *L)
{
  lua_pushnumber(L,checkCvContour(L,1)->flags);
  return 1;
}

static int CvContour_nfree_blocks(lua_State *L)
{
  checkCvContour(L,1)->free_blocks=checkCvSeqBlock(L,3);
  return 0;
}

static int CvContour_free_blocks(lua_State *L)
{
  pushCvSeqBlock(L,checkCvContour(L,1)->free_blocks);
  return 1;
}

static int CvContour_nfirst(lua_State *L)
{
  checkCvContour(L,1)->first=checkCvSeqBlock(L,3);
  return 0;
}

static int CvContour_first(lua_State *L)
{
  pushCvSeqBlock(L,checkCvContour(L,1)->first);
  return 1;
}

static int CvContour_nheader_size(lua_State *L)
{
  const char f_msg[]=CVCONTOUR_NAME".header_size=int";
  checkCvContour(L,1)->header_size=checkint(L,3);
  return 0;
}

static int CvContour_header_size(lua_State *L)
{
  lua_pushnumber(L,checkCvContour(L,1)->header_size);
  return 1;
}

static int CvContour_nh_prev(lua_State *L)
{
  checkCvContour(L,1)->h_prev=checkCvSeq(L,3);
  return 0;
}

static int CvContour_h_prev(lua_State *L)
{
  pushCvSeq(L,checkCvContour(L,1)->h_prev);
  return 1;
}

static int CvContour_nh_next(lua_State *L)
{
  checkCvContour(L,1)->h_next=checkCvSeq(L,3);
  return 0;
}

static int CvContour_h_next(lua_State *L)
{
  pushCvSeq(L,checkCvContour(L,1)->h_next);
  return 1;
}

static int CvContour_nptr(lua_State *L)
{
  const char f_msg[]=CVCONTOUR_NAME".ptr=userdata";
  checkCvContour(L,1)->ptr=(schar*)checkldata(L,3);
  return 0;
}

static int CvContour_ptr(lua_State *L)
{
  pushUserData(L,checkCvContour(L,1)->ptr);
  return 1;
}


static int CvContour_nstorage(lua_State *L)
{
  checkCvContour(L,1)->storage=checkCvMemStorage(L,3);
  return 0;
}

static int CvContour_storage(lua_State *L)
{
  pushCvMemStorage(L,checkCvContour(L,1)->storage);
  return 1;
}

static int CvContour_nrect(lua_State *L)
{
  checkCvContour(L,1)->rect=*checkCvRect(L,3);
  return 0;
}

static int CvContour_rect(lua_State *L)
{
  pushCvRect(L,&checkCvContour(L,1)->rect);
  return 1;
}

static int CvContour_nreserved(lua_State *L)
{
  const char f_msg[]=CVCONTOUR_NAME".reserved=int[3]";
  if ((!lua_istable(L,3))||(lua_objlen(L,3)!=3)) luaL_error(L,f_msg);
  int *res=checkCvContour(L,1)->reserved;
  for(int i=0;i<3;i++)
  {
     lua_rawgeti(L,3,i+1);
     res[i]=checkint(L,4);
     lua_pop(L,1);
  }

  return 0;
}

static int CvContour_reserved(lua_State *L)
{
  int *res=checkCvContour(L,1)->reserved;
  lua_newtable(L);
  for(int i=0;i<3;i++)
  {
      lua_pushnumber(L,res[i]);
      lua_rawseti(L,-2,i+1);
  }
  return 1;
}

static int CvContour_ntotal(lua_State *L)
{
  const char f_msg[]=CVCONTOUR_NAME".total=int";
  checkCvContour(L,1)->total=checkint(L,3);
  return 0;
}

static int CvContour_total(lua_State *L)
{
  lua_pushnumber(L,checkCvContour(L,1)->total);
  return 1;
}

static int CvContour_nv_prev(lua_State *L)
{
  checkCvContour(L,1)->v_prev=checkCvSeq(L,3);
  return 0;
}

static int CvContour_v_prev(lua_State *L)
{
  pushCvSeq(L,checkCvContour(L,1)->v_prev);
  return 1;
}

static int CvContour_nv_next(lua_State *L)
{
  checkCvContour(L,1)->v_next=checkCvSeq(L,3);
  return 0;
}

static int CvContour_v_next(lua_State *L)
{
  pushCvSeq(L,checkCvContour(L,1)->v_next);
  return 1;
}


static const luacv_method CvContour_v[]=
{
  {NULL,18,NULL,NULL},
  methodReg(block_max,CvContour),
  methodReg(color,CvContour),
  methodReg(delta_elems,CvContour),
  methodReg(elem_size,CvContour),
  methodReg(first,CvContour),
  methodReg(flags,CvContour),
  methodReg(free_blocks,CvContour),
  methodReg(header_size,CvContour),
  methodReg(h_prev,CvContour),
  methodReg(h_next,CvContour),
  methodReg(ptr,CvContour),
  methodReg(rect,CvContour),
  methodReg(reserved,CvContour),
  methodReg(storage,CvContour),
  methodReg(total,CvContour),
  methodReg(v_prev,CvContour),
  methodReg(v_next,CvContour),
};

makeIndexFunctions(CvContour)
makeObjectCallback(CvContour);
