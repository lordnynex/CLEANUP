#include "CvSet.h"

static int CvSet_tostring(lua_State *L)
{
  CvSet *seq=checkCvSet(L,1);
  lua_pushfstring(L,CVSET_NAME" object %p\n\tflags=%d\n\theader_size=%d\n\th_prev="CVSEQ_NAME"\n\th_next="CVSEQ_NAME"\n\tv_prev="CVSEQ_NAME"\n\tv_next="CVSEQ_NAME"\n\ttotal=%d\n\telem_size=%d\n\tblock_max=%p\n\tptr=%p\n\tdelta_elems=%d\n\tstorage="CVMEMSTORAGE_NAME"\n\tfree_blocks="CVSEQBLOCK_NAME"\n\tfirst="CVSEQBLOCK_NAME"\n\tfree_elems="CVSETELEM_NAME"\n\tactive_count=%d",seq,seq->flags,seq->header_size,seq->total,seq->elem_size,seq->block_max,seq->ptr,seq->delta_elems,seq->active_count);
  return 1;
}
static int CvSet_gc(lua_State *L)
{

  luacv_obj<CvSet>*obj=(luacv_obj<CvSet>*)luaL_checkudata(L,1,CVSET_NAME);
  if (obj->dealloc_data)
    cvClearSet(obj->data);
  return 0;
}

static int CvSet_nflags(lua_State *L)
{
  const char f_msg[]=CVSET_NAME".flags=int";
  checkCvSet(L,1)->flags=checkint(L,3);
return 0;
}

static int CvSet_flags(lua_State *L)
{
  lua_pushnumber(L,checkCvSet(L,1)->flags);
return 1;
}

static int CvSet_nheader_size(lua_State *L)
{
  const char f_msg[]=CVSET_NAME".header_size=int";
  checkCvSet(L,1)->header_size=checkint(L,3);
return 0;
}

static int CvSet_header_size(lua_State *L)
{
  lua_pushnumber(L,checkCvSet(L,1)->header_size);
return 1;
}
static int CvSet_ntotal(lua_State *L)
{
  const char f_msg[]=CVSET_NAME".total=int";
  checkCvSet(L,1)->total=checkint(L,3);
return 0;
}

static int CvSet_total(lua_State *L)
{
  lua_pushnumber(L,checkCvSet(L,1)->total);
return 1;
}

static int CvSet_nelem_size(lua_State *L)
{
  const char f_msg[]=CVSET_NAME".elem_size=int";
  checkCvSet(L,1)->elem_size=checkint(L,3);
return 0;
}

static int CvSet_elem_size(lua_State *L)
{
  lua_pushnumber(L,checkCvSet(L,1)->elem_size);
return 1;
}

static int CvSet_ndelta_elems(lua_State *L)
{
  const char f_msg[]=CVSET_NAME".delta_elems=int";
  checkCvSet(L,1)->delta_elems=checkint(L,3);
return 0;
}

static int CvSet_delta_elems(lua_State *L)
{
  lua_pushnumber(L,checkCvSet(L,1)->delta_elems);
return 1;
}

static int CvSet_nactive_count(lua_State *L)
{
  const char f_msg[]=CVSET_NAME".active_count=int";
  checkCvSet(L,1)->active_count=checkint(L,3);
return 0;
}

static int CvSet_active_count(lua_State *L)
{
  lua_pushnumber(L,checkCvSet(L,1)->active_count);
return 1;
}

static int CvSet_nh_prev(lua_State *L)
{
  checkCvSet(L,1)->h_prev=checkCvSeq(L,3);
return 0;
}

static int CvSet_h_prev(lua_State *L)
{
  pushCvSeq(L,checkCvSet(L,1)->h_prev);
return 1;
}
static int CvSet_nh_next(lua_State *L)
{
  checkCvSet(L,1)->h_next=checkCvSeq(L,3);
return 0;
}

static int CvSet_h_next(lua_State *L)
{
  pushCvSeq(L,checkCvSet(L,1)->h_next);
return 1;
}

static int CvSet_nv_prev(lua_State *L)
{
  checkCvSet(L,1)->v_prev=checkCvSeq(L,3);
return 0;
}

static int CvSet_v_prev(lua_State *L)
{
  pushCvSeq(L,checkCvSet(L,1)->v_prev);
return 1;
}

static int CvSet_nv_next(lua_State *L)
{
  checkCvSet(L,1)->v_next=checkCvSeq(L,3);
return 0;
}

static int CvSet_v_next(lua_State *L)
{
  pushCvSeq(L,checkCvSet(L,1)->v_next);
return 1;
}

static int CvSet_nblock_max(lua_State *L)
{
  const char f_msg[]=CVSET_NAME".block_max=userdata";
  checkCvSet(L,1)->block_max=(schar*)checkldata(L,3);
return 0;
}

static int CvSet_block_max(lua_State *L)
{
  pushUserData(L,checkCvSet(L,1)->block_max);
return 1;
}

static int CvSet_nptr(lua_State *L)
{
  const char f_msg[]=CVSET_NAME".ptr=userdata";
  checkCvSet(L,1)->ptr=(schar*)checkldata(L,3);
return 0;
}

static int CvSet_ptr(lua_State *L)
{
  pushUserData(L,checkCvSet(L,1)->ptr);
return 1;
}

static int CvSet_nstorage(lua_State *L)
{
  checkCvSet(L,1)->storage=checkCvMemStorage(L,3);
return 0;
}

static int CvSet_storage(lua_State *L)
{
  pushCvMemStorage(L,checkCvSet(L,1)->storage);
return 1;
}

static int CvSet_nfree_blocks(lua_State *L)
{
  checkCvSet(L,1)->free_blocks=checkCvSeqBlock(L,3);
return 0;
}

static int CvSet_free_blocks(lua_State *L)
{
  pushCvSeqBlock(L,checkCvSet(L,1)->free_blocks);
return 1;
}


static int CvSet_nfirst(lua_State *L)
{
  checkCvSet(L,1)->first=checkCvSeqBlock(L,3);
return 0;
}

static int CvSet_first(lua_State *L)
{
  pushCvSeqBlock(L,checkCvSet(L,1)->first);
return 1;
}

static int CvSet_nfree_elems(lua_State *L)
{
  checkCvSet(L,1)->free_elems=checkCvSetElem(L,3);
return 0;
}

static int CvSet_free_elems(lua_State *L)
{
  pushCvSetElem(L,checkCvSet(L,1)->free_elems);
return 1;
}

static const luacv_method CvSet_v[]=
{
  {NULL,16,NULL,NULL},
  methodReg(active_count,CvSet),
  methodReg(block_max,CvSet),
  methodReg(delta_elems,CvSet),
  methodReg(elem_size,CvSet),
  methodReg(flags,CvSet),
  methodReg(free_blocks,CvSet),
  methodReg(free_elems,CvSet),
  methodReg(first,CvSet),
  methodReg(header_size,CvSet),
  methodReg(h_prev,CvSet),
  methodReg(h_next,CvSet),
  methodReg(ptr,CvSet),
  methodReg(storage,CvSet),
  methodReg(total,CvSet),
  methodReg(v_prev,CvSet),
  methodReg(v_next,CvSet),
};

makeIndexFunctions(CvSet)
makeObjectCallback(CvSet);
