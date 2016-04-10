#include "CvSeqWriter.h"
static int CvSeqWriter_tostring(lua_State *L)
{
  CvSeqWriter *seq=checkCvSeqWriter(L,1);
  lua_pushfstring(L,CVSEQWRITER_NAME" object %p\n\theader_size=%d\n\tseq="CVSEQ_NAME"\n\tblock="CVSEQBLOCK_NAME"\n\tptr=%p\n\tblock_min=%p\n\tblock_max=%p",seq,seq->header_size,seq->ptr,seq->block_min,seq->block_max);

  return 1;
}

static int CvSeqWriter_gc(lua_State *L)
{

  luacv_obj<CvSeqWriter>*obj=(luacv_obj<CvSeqWriter>*)luaL_checkudata(L,1,CVSEQWRITER_NAME);
  if (obj->dealloc_data)
    luacv_free(&(obj->data)); 
  return 0;
}

static int CvSeqWriter_nheader_size(lua_State *L)
{
  const char f_msg[]=CVSEQWRITER_NAME".header_size=int";
  checkCvSeqWriter(L,1)->header_size=checkint(L,3);
return 0;
}

static int CvSeqWriter_header_size(lua_State *L)
{
  lua_pushnumber(L,checkCvSeqWriter(L,1)->header_size);
return 1;
}

static int CvSeqWriter_nseq(lua_State *L)
{
  checkCvSeqWriter(L,1)->seq=checkCvSeq(L,3);
return 0;
}

static int CvSeqWriter_seq(lua_State *L)
{
  pushCvSeq(L,checkCvSeqWriter(L,1)->seq);
return 1;
}

static int CvSeqWriter_nblock(lua_State *L)
{
  checkCvSeqWriter(L,1)->block=checkCvSeqBlock(L,3);
return 0;
}

static int CvSeqWriter_block(lua_State *L)
{
  pushCvSeqBlock(L,checkCvSeqWriter(L,1)->block);
return 1;
}

static int CvSeqWriter_nptr(lua_State *L)
{
  const char f_msg[]=CVSEQWRITER_NAME".ptr=userdata";
  checkCvSeqWriter(L,1)->ptr=(schar*)checkldata(L,3);
return 0;
}

static int CvSeqWriter_ptr(lua_State *L)
{
  pushUserData(L,checkCvSeqWriter(L,1)->ptr);
return 1;
}

static int CvSeqWriter_nblock_min(lua_State *L)
{
  const char f_msg[]=CVSEQWRITER_NAME".block_min=userdata";
  checkCvSeqWriter(L,1)->block_min=(schar*)checkldata(L,3);
return 0;
}

static int CvSeqWriter_block_min(lua_State *L)
{
  pushUserData(L,checkCvSeqWriter(L,1)->block_min);
return 1;
}

static int CvSeqWriter_nblock_max(lua_State *L)
{
  const char f_msg[]=CVSEQWRITER_NAME".block_max=userdata";
  checkCvSeqWriter(L,1)->block_max=(schar*)checkldata(L,3);
return 0;
}

static int CvSeqWriter_block_max(lua_State *L)
{
  pushUserData(L,checkCvSeqWriter(L,1)->block_max);
return 1;
}
static const luacv_method CvSeqWriter_v[]=
{
  {NULL,7,NULL,NULL},
  methodReg(block,CvSeqWriter),
  methodReg(block_min,CvSeqWriter),
  methodReg(block_max,CvSeqWriter),
  methodReg(header_size,CvSeqWriter),
  methodReg(ptr,CvSeqWriter),
  methodReg(seq,CvSeqWriter),
};

makeIndexFunctions(CvSeqWriter)
makeObjectCallback(CvSeqWriter);
