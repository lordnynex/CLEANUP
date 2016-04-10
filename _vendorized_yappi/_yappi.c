/*

 yappi
 Yet Another Python Profiler

 Sumer Cip 2013

*/

#include "config.h"

#if !defined(HAVE_LONG_LONG)
#error "Yappi requires long longs!"
#endif

#ifdef IS_PY3K
#include "bytesobject.h"
#endif
#include "frameobject.h"
#include "callstack.h"
#include "hashtab.h"
#include "debug.h"
#include "timing.h"
#include "freelist.h"
#include "mem.h"

#ifdef IS_PY3K
PyDoc_STRVAR(_yappi__doc__, "Yet Another Python Profiler");
#endif

// linked list for holding callee/caller info in the pit
// we need to record the timing data on the pairs (parent, child)
typedef struct {
    unsigned int index;
    unsigned long callcount;
    unsigned long nonrecursive_callcount;   // how many times the child function is called non-recursively?
    long long tsubtotal;                    // the time that the child function spent excluding its children (include recursive parent-child calls)
    long long ttotal;                       // the total time that the child function spent
    struct _pit_children_info *next;
} _pit_children_info;

// module definitions
typedef struct {
    PyObject *name;
    PyObject *modname;
    unsigned long lineno;
    unsigned long callcount;
    unsigned long nonrecursive_callcount;   // the number of actual calls when the function is recursive.
    long long tsubtotal;                    // time function spent excluding its children (include recursive calls)
    long long ttotal;                       // the total time that a function spent
    unsigned int builtin;                   // 0 for normal, 1 for ccall
    unsigned int index;    
    _pit_children_info *children;
} _pit; // profile_item

typedef struct {
    _cstack *cs;
    _htab *rec_levels;
    long id;
    char *class_name;
    long long t0;                           // profiling start CPU time
    unsigned long sched_cnt;                // how many times this thread is scheduled
    long long t_paused;                     // when this context was last paused
    int paused;                             // 1 if currently paused
} _ctx; // context

typedef struct {
    int builtins;
    int multithreaded;
} _flag; // flags passed from yappi.start()

// globals
static PyObject *YappiProfileError;
static _htab *contexts;
static _htab *pits;
static _flag flags;
static _freelist *flpit;
static _freelist *flctx;
static int yappinitialized;
static unsigned int ycurfuncindex; // used for providing unique index for functions
static int yapphavestats;	// start() called at least once or stats cleared?
static int yapprunning;
static int paused;
static time_t yappstarttime;
static long long yappstarttick;
static long long yappstoptick;
static _ctx *prev_ctx;
static _ctx *current_ctx;
static PyObject *context_id_callback = NULL;
static PyObject *context_name_callback = NULL;
static PyObject *test_timings; // used for testing

// defines
#define UNINITIALIZED_STRING_VAL "N/A"
#ifdef IS_PY3K // string formatting helper functions compatible with with both 2.x and 3.x
#define PyStr_AS_CSTRING(s) PyBytes_AS_STRING(PyUnicode_AsUTF8String(s))
#else
#define PyStr_AS_CSTRING(s) PyString_AS_STRING(s)
#endif

// forwards
static _ctx * _profile_thread(PyThreadState *ts);

static PyObject * 
PyStr_FromFormat(const char *fmt, ...)
{
    PyObject* ret;
    va_list vargs;
    
    va_start(vargs, fmt);
#ifdef IS_PY3K
    ret = PyUnicode_FromFormatV(fmt, vargs);
#else
    ret = PyString_FromFormatV(fmt, vargs);
#endif
    va_end(vargs);
    return ret;
}

static PyObject * 
PyStr_FromString(const char *s)
{
    PyObject* ret;
    
#ifdef IS_PY3K
    ret = PyUnicode_FromString(s);
#else
    ret = PyString_FromString(s);
#endif
    return ret;
}

// module functions

static void
_log_err(unsigned int code)
{
    yerr("Internal Error. [%u]", code);
}

static _pit *
_create_pit(void)
{
    _pit *pit;

    pit = flget(flpit);
    if (!pit)
        return NULL;
    pit->callcount = 0;
    pit->nonrecursive_callcount = 0;
    pit->ttotal = 0;
    pit->tsubtotal = 0;
    pit->name = NULL;
    pit->modname = NULL;
    pit->lineno = 0;
    pit->builtin = 0;
    pit->index = ycurfuncindex++;
    pit->children = NULL;

    return pit;
}

static _ctx *
_create_ctx(void)
{
    _ctx *ctx;

    ctx = flget(flctx);
    if (!ctx)
        return NULL;
    ctx->cs = screate(100);
    if (!ctx->cs)
        return NULL;
    ctx->sched_cnt = 0;
    ctx->id = 0;
    ctx->class_name = NULL;
    ctx->t0 = tickcount();
    ctx->t_paused = 0;
    ctx->paused = 0;
    ctx->rec_levels = htcreate(HT_RLEVEL_SIZE);
    if (!ctx->rec_levels)
        return NULL;
    return ctx;
}

char *
_call_context_name_callback(void)
{
    PyObject *callback_rc = NULL;
    char *rc = NULL;

    if (!context_name_callback) {
        return NULL;
    } else {
        callback_rc = PyObject_CallFunctionObjArgs(context_name_callback, NULL);
        if (!callback_rc) {
            PyErr_Print();
            goto error;
        }
        if (!PyString_Check(callback_rc)) {
            yerr("context name callback returned non-string");
            Py_DECREF(context_name_callback);
            goto error;
        }
        
        rc = PyStr_AS_CSTRING(callback_rc);
        Py_CLEAR(callback_rc);
    }
    return rc;

error:
    Py_XDECREF(callback_rc);
    Py_CLEAR(context_name_callback);  /* Don't use the callback again. */
    return NULL;
}

char *
_get_current_thread_class_name(void)
{
    char *rc = NULL;
    PyObject *mthr, *cthr, *tattr1, *tattr2;
    mthr = cthr = tattr1 = tattr2 = NULL;

    rc = _call_context_name_callback();
    if (rc) { return rc; }
    
    mthr = PyImport_ImportModuleNoBlock("threading"); // Requires Python 2.6.
    if (!mthr)
        goto err;
    cthr = PyObject_CallMethod(mthr, "currentThread", "");
    if (!cthr)
        goto err;
    tattr1 = PyObject_GetAttrString(cthr, "__class__");
    if (!tattr1)
        goto err;
    tattr2 = PyObject_GetAttrString(tattr1, "__name__");
    if (!tattr2)
        goto err;

    return PyStr_AS_CSTRING(tattr2);

err:
    Py_XDECREF(mthr);
    Py_XDECREF(cthr);
    Py_XDECREF(tattr1);
    Py_XDECREF(tattr2);
    return NULL;
}

static uintptr_t
_current_context_id(PyThreadState *ts)
{
    uintptr_t rc;
    PyObject *callback_rc;
    if (context_id_callback) {
        callback_rc = PyObject_CallFunctionObjArgs(context_id_callback, NULL);
        if (!callback_rc) {
            PyErr_Print();
            goto error;
        }
        rc = (uintptr_t)PyLong_AsLong(callback_rc);
        Py_DECREF(callback_rc);
        if (PyErr_Occurred()) {
            yerr("context id callback returned non-integer");
            goto error;
        }
        return rc;
    } else {
        // Use thread_id instead of ts pointer, because when we create/delete many threads, some
        // of them do not show up in the thread_stats, because ts pointers are recycled in the VM.
        // Also, we do not want to delete thread stats unless clear_stats() is called explicitly.
        // We rely on the OS to give us unique thread ids, this time.
        // thread_id -> long
        return (uintptr_t)ts->thread_id;
    }
    
error:
    PyErr_Clear();
    // don't use callback again
    Py_XDECREF(context_id_callback);
    context_id_callback = NULL;
    return 0;
}

static _ctx *
_thread2ctx(PyThreadState *ts)
{
    _hitem *it;
    it = hfind(contexts, _current_context_id(ts));
    if (!it) {
        // callback functions in some circumtances, can be called before the context entry is not
        // created. (See issue 21). To prevent this problem we need to ensure the context entry for
        // the thread is always available here.
        return _profile_thread(ts);
    }
    return (_ctx *)it->val;
}

// the pit will be cleared by the relevant freelist. we do not free it here.
// we only DECREF the CodeObject or the MethodDescriptive string.
static void
_del_pit(_pit *pit)
{
    _pit_children_info *it,*next;
    it = pit->children;
    while(it) {
        next = (_pit_children_info *)it->next;
        yfree(it);
        it = next;
    }
    pit->children = NULL;
    Py_XDECREF(pit->name);
    Py_XDECREF(pit->modname);
}

static _pit *
_ccode2pit(void *cco)
{
    PyCFunctionObject *cfn;
    _hitem *it;
    PyObject *mod;
    char *modname;
    PyObject *name;

    cfn = cco;
    // Issue #15:
    // Hashing cfn to the pits table causes different object methods
    // to be hashed into the same slot. Use cfn->m_ml for hashing the
    // Python C functions.
    it = hfind(pits, (uintptr_t)cfn->m_ml);
    if (!it) {
        _pit *pit = _create_pit();
        if (!pit)
            return NULL;
        if (!hadd(pits, (uintptr_t)cfn->m_ml, (uintptr_t)pit))
            return NULL;

        pit->builtin = 1;

        // get module name
        modname = NULL;
        mod = cfn->m_module;
#ifdef IS_PY3K
        if (mod && PyUnicode_Check(mod)) {      
#else
        if (mod && PyString_Check(mod)) {
#endif
            modname = PyStr_AS_CSTRING(mod);
        } else if (mod && PyModule_Check(mod)) {
            modname = (char *)PyModule_GetName(mod);
        } 
        
        if (modname == NULL) {
            PyErr_Clear();
            modname = "__builtin__";
        }
        
        pit->modname = PyStr_FromString(modname);
        pit->lineno = 0;
        
        // built-in method?
        if (cfn->m_self != NULL) {
            name = PyStr_FromString(cfn->m_ml->ml_name);
            if (name != NULL) {
                PyObject *mo = _PyType_Lookup((PyTypeObject *)PyObject_Type(cfn->m_self), name);
                Py_XINCREF(mo);
                Py_DECREF(name);
                if (mo != NULL) {
                    pit->name = PyObject_Repr(mo);
                    Py_DECREF(mo);
                    return pit;
                }
            }
            PyErr_Clear();
        }
        pit->name = PyStr_FromString(cfn->m_ml->ml_name);
        return pit;
    }
    return ((_pit *)it->val);
}

// maps the PyCodeObject to our internal pit item via hash table.
static _pit *
_code2pit(PyFrameObject *fobj)
{
    _hitem *it;
    PyCodeObject *cobj;
    _pit *pit;

    cobj = fobj->f_code;
    it = hfind(pits, (uintptr_t)cobj);
    if (it) {
        return ((_pit *)it->val);
    }

    pit = _create_pit();
    if (!pit)
        return NULL;
    if (!hadd(pits, (uintptr_t)cobj, (uintptr_t)pit))
        return NULL;

    pit->name = NULL;
    pit->modname = PyStr_FromString(PyStr_AS_CSTRING(cobj->co_filename));    
    pit->lineno = cobj->co_firstlineno;

    PyFrame_FastToLocals(fobj);
    if (cobj->co_argcount) {
        const char *firstarg = PyStr_AS_CSTRING(PyTuple_GET_ITEM(cobj->co_varnames, 0));
        PyObject *self, *cls, *name;

        if (strcmp(firstarg, "self") ||
            !fobj->f_locals ||
            !(self = PyDict_GetItemString(fobj->f_locals, "self")) || /* borrowed ref */
            !(cls = PyObject_GetAttrString(self, "__class__")) /* new ref */
           ) {
            goto out;
        }
        name = PyObject_GetAttrString(cls, "__name__"); /* new ref */
        Py_DECREF(cls);
        if (!name) {
            goto out;
        }
        pit->name = PyStr_FromFormat("%s.%s", PyStr_AS_CSTRING(name), PyStr_AS_CSTRING(cobj->co_name)); /* new ref */
        Py_DECREF(name);
    }

out:
    if (!pit->name) {
        pit->name = PyStr_FromString(PyStr_AS_CSTRING(cobj->co_name));
    }

    PyFrame_LocalsToFast(fobj, 0);

    return pit;
}

static _pit *
_get_frame(void)
{
    _cstackitem *ci;
    
    ci = shead(current_ctx->cs);
    if (!ci) {
        return NULL;
    }
    return ci->ckey;
}

static _cstackitem *
_push_frame(_pit *cp)
{
    return spush(current_ctx->cs, cp);
}

static _pit *
_pop_frame(void)
{
    _cstackitem *ci;
    
    ci = spop(current_ctx->cs);
    if (!ci) {
        return NULL;
    }
    return ci->ckey;
}

static _pit_children_info *
_get_child_info(_pit *parent, _pit *current)
{
    _pit_children_info *citem;
    
    citem = parent->children;    
    while(citem) { 
        if (citem->index == current->index) {
            break;
        }
        citem = (_pit_children_info *)citem->next;
    }
    return citem;
}

static _pit_children_info *
_add_child_info(_pit *parent, _pit *current)
{
    _pit_children_info *newci;
  
    newci = ymalloc(sizeof(_pit_children_info));
    newci->index = current->index;
    newci->callcount = 0;
    newci->nonrecursive_callcount = 0;
    newci->ttotal = 0;
    newci->tsubtotal = 0;
    newci->next = (struct _pit_children_info *)parent->children;
    parent->children = (_pit_children_info *)newci;
    
    return newci;
}

static long
get_rec_level(uintptr_t key)
{
    _hitem *it;
    
    it = hfind(current_ctx->rec_levels, key);
    if (!it) {
        _log_err(1);
        return -1; // should not happen
    }
    return it->val;
}

static int
incr_rec_level(uintptr_t key)
{
    _hitem *it;
    
    it = hfind(current_ctx->rec_levels, key);
    if (it) {
        it->val++;
    } else {
        if (!hadd(current_ctx->rec_levels, key, 1))
        {
            _log_err(2);
            return 0; // should not happen
        }
    }    
    return 1;
}

static int
decr_rec_level(uintptr_t key)
{
    _hitem *it;
    uintptr_t v;
    
    it = hfind(current_ctx->rec_levels, key);
    if (it) {
        v = it->val--;  /*supress warning -- it is safe to cast long vs pointers*/
        if (v == 0)
        {
            hfree(current_ctx->rec_levels, it);
        }
    } else {
        _log_err(3);
        return 0; // should not happen 
    }  
    return 1;
}

static long long
_get_frame_elapsed(void)
{
    _cstackitem *ci;
    _pit *cp;    
    long long result;
    
    ci = shead(current_ctx->cs);
    if (!ci) {
        return 0LL;
    }
    cp = ci->ckey;
    
    if (test_timings) { 
        //printf("name:%s_%d \r\n", PyStr_AS_CSTRING(cp->name), rlevel);
        uintptr_t rlevel = get_rec_level((uintptr_t)cp);        
        PyObject *tval = PyDict_GetItem(test_timings, 
            PyStr_FromFormat("%s_%d", PyStr_AS_CSTRING(cp->name), rlevel));
        if (tval) {            
            result = PyLong_AsLong(tval);
        } else {
            result = DEFAULT_TEST_ELAPSED_TIME;
        }
        
    } else {
        result = tickcount() - ci->t0;
    }
    
    return result;
}

void
_pause_ctx(_ctx *ctx)
{
    if (ctx && !ctx->paused) {
        ctx->t_paused = tickcount();
        ctx->paused = 1;
    }
}

void
_resume_ctx(_ctx *ctx)
{
    int i;
    if (ctx && ctx->paused) {
        // advance the start time for each frame in this context's call stack
        // by the duration for which this context was paused
        long long dur = tickcount() - ctx->t_paused;
        for (i = 0; i <= ctx->cs->head; i++) {
            ctx->cs->_items[i].t0 += dur;
        }
        
        // advance the start time for the whole context by the pause duration
        ctx->t0 += dur;
        
        // no longer paused
        ctx->paused = 0;
    }
}
    
static void
_call_enter(PyObject *self, PyFrameObject *frame, PyObject *arg, int ccall)
{
    _pit *cp,*pp;    
    _cstackitem *ci;
    _pit_children_info *pci;
    
    if (ccall) {
        cp = _ccode2pit((PyCFunctionObject *)arg);
    } else {
        cp = _code2pit(frame);
    }

    // something went wrong. No mem, or another error. we cannot find
    // a corresponding pit. just run away:)
    if (!cp) {
        _log_err(4);
        return;
    }
    
    // create/update children info if we have a valid parent
    pp = _get_frame();    
    if (pp) {
        pci = _get_child_info(pp, cp);    
        if(!pci)
        {
            pci = _add_child_info(pp, cp);
        }    
        pci->callcount++;
        incr_rec_level((uintptr_t)pci);
    }
    
    ci = _push_frame(cp);
    if (!ci) { // runaway! (defensive)
        _log_err(5);
        return;
    }

    ci->t0 = tickcount();
    cp->callcount++;
    incr_rec_level((uintptr_t)cp);
}

static void
_call_leave(PyObject *self, PyFrameObject *frame, PyObject *arg, int ccall)
{
    long long elapsed;    
    _pit *cp, *pp, *ppp;    
    _pit_children_info *pci,*ppci;
            
    elapsed = _get_frame_elapsed();    
    
    // leaving a frame while callstack is empty? 
    cp = _pop_frame();
    if (!cp)
    {
        return;
    }
    
    // is this the last function in the callstack?
    pp = _pop_frame();
    if (!pp) {
        cp->ttotal += elapsed;
        cp->tsubtotal += elapsed;
        cp->nonrecursive_callcount++;
        decr_rec_level((uintptr_t)cp);
        return;
    }
    
    // get children info
    pci = _get_child_info(pp, cp);    
    if(!pci)
    {
        _log_err(6);
        return; // defensive
    }
    // a calls b. b's elapsed time is subtracted from a's tsub and a adds its own elapsed it is leaving.
    pp->tsubtotal -= elapsed;
    cp->tsubtotal += elapsed;    
        
    // a calls b calls c. child c's elapsed time is subtracted from child b's tsub and child b adds its
    // own elapsed when it is leaving
    ppp = _get_frame();
    if (ppp) {
        ppci = _get_child_info(ppp, pp);    
        if(!ppci)
        {
            _log_err(7);
            return;
        }            
        ppci->tsubtotal -= elapsed;
    }
    pci->tsubtotal += elapsed;
    
    // wait for the top-level function/parent/child to update timing values accordingly.              
    if (get_rec_level((uintptr_t)cp) == 1) {
        cp->ttotal += elapsed;
        cp->nonrecursive_callcount++;
        pci->nonrecursive_callcount++;
    }
    
    if (get_rec_level((uintptr_t)pci) == 1) {
        pci->ttotal += elapsed;
    }
    
    decr_rec_level((uintptr_t)pci);
    decr_rec_level((uintptr_t)cp);
    
    if (!_push_frame(pp)) {
        _log_err(8);
        return; //defensive
    }
}

// context will be cleared by the free list. we do not free it here.
// we only free the context call stack.
static void
_del_ctx(_ctx * ctx)
{
    sdestroy(ctx->cs);
    htdestroy(ctx->rec_levels);
}

static int
_yapp_callback(PyObject *self, PyFrameObject *frame, int what,
               PyObject *arg)
{
    PyObject *last_type, *last_value, *last_tb;
        
    PyErr_Fetch(&last_type, &last_value, &last_tb);
    
    // get current ctx
    current_ctx = _thread2ctx(frame->f_tstate);
    if (!current_ctx) {
        _log_err(9);
        goto finally;
    }
    
    // update ctx stats
    if (prev_ctx != current_ctx) {
        current_ctx->sched_cnt++;
        if (context_id_callback && get_timing_clock_type() == CPU_CLOCK) {
            _pause_ctx(prev_ctx);
            _resume_ctx(current_ctx);
        }
    }
    prev_ctx = current_ctx;
    if (!current_ctx->class_name)
    {
        current_ctx->class_name = _get_current_thread_class_name();
    }
    
        
    switch (what) {
    case PyTrace_CALL:
        _call_enter(self, frame, arg, 0);
        break;
    case PyTrace_RETURN: // either normally or with an exception
        _call_leave(self, frame, arg, 0);
        break;

#ifdef PyTrace_C_CALL	// not defined in Python <= 2.3
    case PyTrace_C_CALL:
        if (PyCFunction_Check(arg))
            _call_enter(self, frame, arg, 1); // set ccall to true
        break;

    case PyTrace_C_RETURN:
    case PyTrace_C_EXCEPTION:
        if (PyCFunction_Check(arg))
            _call_leave(self, frame, arg, 1);
        break;
#endif
    default:
        break;
    }

    goto finally;
    
finally:
    if (last_type) {
        PyErr_Restore(last_type, last_value, last_tb);
    }
    return 0;
}


static _ctx *
_profile_thread(PyThreadState *ts)
{
    uintptr_t ctx_id;
    _ctx *ctx;
    _hitem *it;

    ctx = _create_ctx();
    if (!ctx) {
        return NULL;
    }
    
    ctx_id = _current_context_id(ts);
    it = hfind(contexts, ctx_id);
    if (!it) {
        if (!hadd(contexts, ctx_id, (uintptr_t)ctx)) {
            _del_ctx(ctx);
            if (!flput(flctx, ctx)) {
                _log_err(10);
            }
            _log_err(11);
            return NULL;
        }
    }

    ts->use_tracing = 1;
    ts->c_profilefunc = _yapp_callback;
    ctx->id = ctx_id;

    return ctx;
}

static _ctx*
_unprofile_thread(PyThreadState *ts)
{
    ts->use_tracing = 0;
    ts->c_profilefunc = NULL;
    return NULL; //dummy return for enum_threads() func. prototype
}

static void
_ensure_thread_profiled(PyThreadState *ts)
{
    PyThreadState *p = NULL;

    for (p=ts->interp->tstate_head ; p != NULL; p = p->next) {
        if (ts->c_profilefunc != _yapp_callback) {
            _profile_thread(ts);
        }
    }
}

static void
_enum_threads(_ctx* (*f) (PyThreadState *))
{
    PyThreadState *p = NULL;
    for (p=PyThreadState_GET()->interp->tstate_head ; p != NULL; p = p->next) {
        f(p);
    }
}

static int
_init_profiler(void)
{
    // already initialized? only after clear_stats() and first time, this flag
    // will be unset.
    if (!yappinitialized) {
        contexts = htcreate(HT_CTX_SIZE);
        if (!contexts)
            return 0;
        pits = htcreate(HT_PIT_SIZE);
        if (!pits)
            return 0;
        flpit = flcreate(sizeof(_pit), FL_PIT_SIZE);
        if (!flpit)
            return 0;
        flctx = flcreate(sizeof(_ctx), FL_CTX_SIZE);
        if (!flctx)
            return 0;
        yappinitialized = 1;
        current_ctx = NULL;
        prev_ctx = NULL;
        ycurfuncindex = 0;
    }
    return 1;
}

static PyObject*
profile_event(PyObject *self, PyObject *args)
{
    char *ev;
    PyObject *arg;
    PyObject *event;
    PyFrameObject * frame;

    if (!PyArg_ParseTuple(args, "OOO", &frame, &event, &arg)) {
        return NULL;
    }
    
    if (flags.multithreaded) {
        _ensure_thread_profiled(PyThreadState_GET());
    }
    
    ev = PyStr_AS_CSTRING(event);

    if (strcmp("call", ev)==0)
        _yapp_callback(self, frame, PyTrace_CALL, arg);
    else if (strcmp("return", ev)==0)
        _yapp_callback(self, frame, PyTrace_RETURN, arg);
    else if (strcmp("c_call", ev)==0)
        _yapp_callback(self, frame, PyTrace_C_CALL, arg);
    else if (strcmp("c_return", ev)==0)
        _yapp_callback(self, frame, PyTrace_C_RETURN, arg);
    else if (strcmp("c_exception", ev)==0)
        _yapp_callback(self, frame, PyTrace_C_EXCEPTION, arg);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
start(PyObject *self, PyObject *args)
{
    if (yapprunning) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    if (!PyArg_ParseTuple(args, "ii", &flags.builtins, &flags.multithreaded))
        return NULL;

    if (!_init_profiler()) {
        PyErr_SetString(YappiProfileError, "profiler cannot be initialized.");
        return NULL;
    }
    
    if (flags.multithreaded) {
        _enum_threads(&_profile_thread);
    } else {
        _ensure_thread_profiled(PyThreadState_GET());
    }
    
    yapprunning = 1;
    yapphavestats = 1;
    time (&yappstarttime);
    yappstarttick = tickcount();
    
    Py_INCREF(Py_None);
    return Py_None;
}


static long long
_calc_cumdiff(long long a, long long b)
{
    long long r;

    r = a - b;
    if (r < 0)
        return 0;
    return r;
}

static int
_pitenumdel(_hitem *item, void *arg)
{
    _del_pit((_pit *)item->val);
    return 0;
}

static int
_ctxenumdel(_hitem *item, void *arg)
{
    _del_ctx(((_ctx *)item->val) );
    return 0;
}

static PyObject*
stop(PyObject *self, PyObject *args)
{
    if (!yapprunning) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    _enum_threads(&_unprofile_thread);

    yapprunning = 0;
    yappstoptick = tickcount();

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
clear_stats(PyObject *self, PyObject *args)
{    
    if (!yapphavestats) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    
    henum(pits, _pitenumdel, NULL);
    htdestroy(pits);
    henum(contexts, _ctxenumdel, NULL);
    htdestroy(contexts);
    
    fldestroy(flpit);
    fldestroy(flctx);
    yappinitialized = 0;
    yapphavestats = 0;
    Py_XDECREF(test_timings);
    test_timings = NULL;

// check for mem leaks if DEBUG_MEM is specified
#ifdef DEBUG_MEM
    YMEMLEAKCHECK();
#endif

    Py_INCREF(Py_None);
    return Py_None;
}

// normalizes the time count if test_timing is not set.
static double
_normt(long long tickcount)
{
    if (!test_timings) {
        return tickcount * tickfactor();
    }
    return (double)tickcount;
}

static int
_ctxenumstat(_hitem *item, void *arg)
{
    PyObject *efn;
    char *tcname;
    _ctx *ctx;
    long long t_end, cumdiff;
    PyObject *exc;
    
    ctx = (_ctx *)item->val;
    
    if(ctx->sched_cnt == 0) {
        // we return here because if sched_cnt is zero, then this means not any single function
        // executed in the context of this thread. We do not want to show any thread stats for this case especially
        // because of the following case: start()/lots of MT calls/stop()/clear_stats()/start()/get_thread_stats()
        // still returns the threads from the previous cleared session. That is because Python VM does not free them
        // in the second start() call, we enumerate the active threads from the threading module and they are still there.
        // second invocation of test_start_flags() generates this situation.
        return 0;
    }
    
    tcname = ctx->class_name;
    if (tcname == NULL)
        tcname = UNINITIALIZED_STRING_VAL;
    efn = (PyObject *)arg;

    t_end = ctx->paused ? ctx->t_paused : tickcount();
    cumdiff = _calc_cumdiff(t_end, ctx->t0);
    
    exc = PyObject_CallFunction(efn, "((skfk))", tcname, ctx->id, 
        cumdiff * tickfactor(), ctx->sched_cnt);
    if (!exc) {
        PyErr_Print();
        return 1; // abort enumeration
    }

    return 0;
}

static PyObject*
enum_thread_stats(PyObject *self, PyObject *args)
{
    PyObject *enumfn;

    if (!yapphavestats) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    if (!PyArg_ParseTuple(args, "O", &enumfn)) {
        PyErr_SetString(YappiProfileError, "invalid param to enum_thread_stats");
        return NULL;
    }

    if (!PyCallable_Check(enumfn)) {
        PyErr_SetString(YappiProfileError, "enum function must be callable");
        return NULL;
    }

    henum(contexts, _ctxenumstat, enumfn);

    Py_INCREF(Py_None);
    return Py_None;
}

static int
_pitenumstat(_hitem *item, void * arg)
{
    PyObject *efn;
    _pit *pt;
    PyObject *exc;
    PyObject *children;
    _pit_children_info *pci;

    pt = (_pit *)item->val;
    // do not show builtin pits if specified
    if  ((!flags.builtins) && (pt->builtin)) {
        return 0;
    }

    efn = (PyObject *)arg;

    // convert children function index list to PyList
    children = PyList_New(0);
    pci = pt->children;
    while(pci) {
        // normalize tsubtotal. tsubtotal being negative is an expected situation.
        if (pci->tsubtotal < 0) {
            pci->tsubtotal = 0;
        }        
        PyList_Append(children, Py_BuildValue("Ikkff", pci->index, pci->callcount, 
                pci->nonrecursive_callcount, _normt(pci->ttotal), _normt(pci->tsubtotal) ));
        pci = (_pit_children_info *)pci->next;
    }
    // normalize tsubtotal. tsubtotal being negative is an expected situation.
    if (pt->tsubtotal < 0) {
        pt->tsubtotal = 0;
    }
    exc = PyObject_CallFunction(efn, "((OOkkkIffIO))", pt->name, pt->modname, pt->lineno, pt->callcount,
                        pt->nonrecursive_callcount, pt->builtin, _normt(pt->ttotal), _normt(pt->tsubtotal), 
                        pt->index, children);
    if (!exc) {
        PyErr_Print();
        return 1; // abort enumeration
    }
    return 0;
}

static PyObject*
enum_func_stats(PyObject *self, PyObject *args)
{
    PyObject *enumfn;

    if (!yapphavestats) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    if (!PyArg_ParseTuple(args, "O", &enumfn)) {
        PyErr_SetString(YappiProfileError, "invalid param to enum_func_stats");
        return NULL;
    }

    if (!PyCallable_Check(enumfn)) {
        PyErr_SetString(YappiProfileError, "enum function must be callable");
        return NULL;
    }

    henum(pits, _pitenumstat, enumfn);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
is_running(PyObject *self, PyObject *args)
{
    return Py_BuildValue("i", yapprunning);
}

static PyObject *
get_mem_usage(PyObject *self, PyObject *args)
{
    return Py_BuildValue("l", ymemusage());
}

static PyObject *
set_context_id_callback(PyObject *self, PyObject *args)
{
    PyObject* new_callback;
    if (!PyArg_ParseTuple(args, "O", &new_callback)) {
        return NULL;
    }
    
    if (new_callback == Py_None) {
        Py_CLEAR(context_id_callback);
        Py_RETURN_NONE;
    } else if (!PyCallable_Check(new_callback)) {
        PyErr_SetString(PyExc_TypeError, "callback should be a function.");
        return NULL;
    }
    Py_XDECREF(context_id_callback);
    Py_INCREF(new_callback);
    context_id_callback = new_callback;
    Py_RETURN_NONE;
}
    
static PyObject *
set_context_name_callback(PyObject *self, PyObject *args)
{
    PyObject* new_callback;
    if (!PyArg_ParseTuple(args, "O", &new_callback)) {
        return NULL;
    }
    
    if (new_callback == Py_None) {
        Py_CLEAR(context_name_callback);
        Py_RETURN_NONE;
    } else if (!PyCallable_Check(new_callback)) {
        PyErr_SetString(PyExc_TypeError, "callback should be a function.");
        return NULL;
    }
    Py_XDECREF(context_name_callback);
    Py_INCREF(new_callback);
    context_name_callback = new_callback;
    Py_RETURN_NONE;
}

static PyObject *
set_test_timings(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "O", &test_timings)) {
        return NULL;
    }
    
    if (!PyDict_Check(test_timings))
    {
        PyErr_SetString(YappiProfileError, "timings should be dict.");
        return NULL;
    }
    Py_INCREF(test_timings);
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
set_clock_type(PyObject *self, PyObject *args)
{
    int clock_type;
    
    if (!PyArg_ParseTuple(args, "i", &clock_type)) {
        return NULL;
    }
    
    // return silently if same clock_type
    if (clock_type == get_timing_clock_type())
    {
        Py_INCREF(Py_None);
        return Py_None;
    }
    
    if (yapphavestats) {
        PyErr_SetString(YappiProfileError, "clock type cannot be changed previous stats are available. clear the stats first.");
        return NULL;
    }
    
    if (!set_timing_clock_type(clock_type)) {
        PyErr_SetString(YappiProfileError, "Invalid clock type.");
        return NULL;
    }   
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
get_clock_type(PyObject *self, PyObject *args)
{
    PyObject *type,*api,*result,*resolution;
    clock_type_t clk_type;

    result = PyDict_New();
    
    clk_type = get_timing_clock_type();
    if (clk_type == WALL_CLOCK) {
        type = Py_BuildValue("s", "wall");
#if defined(_WINDOWS)
        api = Py_BuildValue("s", "queryperformancecounter");
        resolution = Py_BuildValue("s", "100ns");    
#else
        api = Py_BuildValue("s", "gettimeofday");
        resolution = Py_BuildValue("s", "100ns");
#endif
    }  else {
        type = Py_BuildValue("s", "cpu");
#if defined(USE_CLOCK_TYPE_GETTHREADTIMES)
        api = Py_BuildValue("s", "getthreadtimes");
        resolution = Py_BuildValue("s", "100ns");
#elif defined(USE_CLOCK_TYPE_THREADINFO)
        api = Py_BuildValue("s", "threadinfo");
        resolution = Py_BuildValue("s", "1000ns");
#elif defined(USE_CLOCK_TYPE_CLOCKGETTIME)
        api = Py_BuildValue("s", "clockgettime");
        resolution = Py_BuildValue("s", "1ns");
#elif defined(USE_CLOCK_TYPE_RUSAGE)
        api = Py_BuildValue("s", "getrusage");
        resolution = Py_BuildValue("s", "1000ns");
#endif
    }
    
    PyDict_SetItemString(result, "type", type);
    PyDict_SetItemString(result, "api", api);
    PyDict_SetItemString(result, "resolution", resolution);

    return result;
}

static PyObject*
get_start_flags(PyObject *self, PyObject *args)
{
    PyObject *result;
    
    if (!yapphavestats) {
        Py_INCREF(Py_None);
        return Py_None;
    }

    result = PyDict_New();
    PyDict_SetItemString(result, "profile_builtins", Py_BuildValue("i", flags.builtins));
    PyDict_SetItemString(result, "profile_multithread", Py_BuildValue("i", flags.multithreaded));
    
    return result;
}

static PyObject*
_pause(PyObject *self, PyObject *args)
{
    if (yapprunning)
    {
        paused = 1;
        stop(self, NULL);
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject*
_resume(PyObject *self, PyObject *args)
{
    if (paused)
    {
        paused = 0;        
        start(self, Py_BuildValue("ii", flags.builtins, flags.multithreaded));
    }
    
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef yappi_methods[] = {
    {"start", start, METH_VARARGS, NULL},
    {"stop", stop, METH_VARARGS, NULL},
    {"enum_func_stats", enum_func_stats, METH_VARARGS, NULL},
    {"enum_thread_stats", enum_thread_stats, METH_VARARGS, NULL},
    {"clear_stats", clear_stats, METH_VARARGS, NULL},
    {"is_running", is_running, METH_VARARGS, NULL},
    {"get_clock_type", get_clock_type, METH_VARARGS, NULL},
    {"set_clock_type", set_clock_type, METH_VARARGS, NULL},
    {"get_mem_usage", get_mem_usage, METH_VARARGS, NULL},
    {"set_context_id_callback", set_context_id_callback, METH_VARARGS, NULL},
    {"set_context_name_callback", set_context_name_callback, METH_VARARGS, NULL},
    {"_get_start_flags", get_start_flags, METH_VARARGS, NULL}, // for internal usage.
    {"_set_test_timings", set_test_timings, METH_VARARGS, NULL}, // for internal usage.
    {"_profile_event", profile_event, METH_VARARGS, NULL}, // for internal usage.
    {"_pause", _pause, METH_VARARGS, NULL}, // for internal usage.
    {"_resume", _resume, METH_VARARGS, NULL}, // for internal usage.
    {NULL, NULL}      /* sentinel */
};

#ifdef IS_PY3K
static struct PyModuleDef _yappi_module = {
        PyModuleDef_HEAD_INIT,
        "_yappi",
        _yappi__doc__,
        -1,
        yappi_methods,
        NULL,
        NULL,
        NULL,
        NULL
};
#endif

PyMODINIT_FUNC
#ifdef IS_PY3K
PyInit__yappi(void)
#else
init_GreenletProfiler_yappi(void)
#endif
{
    PyObject *m, *d;

#ifdef IS_PY3K
    m = PyModule_Create(&_yappi_module);
    if (m == NULL)
        return NULL;
#else
    m = Py_InitModule("_GreenletProfiler_yappi",  yappi_methods);
    if (m == NULL)
        return;
#endif

    d = PyModule_GetDict(m);
    YappiProfileError = PyErr_NewException("_yappi.error", NULL, NULL);
    PyDict_SetItemString(d, "error", YappiProfileError);

    // init the profiler memory and internal constants
    yappinitialized = 0;
    yapphavestats = 0;
    yapprunning = 0;
    paused = 0;
    flags.builtins = 0;
    flags.multithreaded = 0;
    test_timings = NULL;
    
    if (!_init_profiler()) {
        PyErr_SetString(YappiProfileError, "profiler cannot be initialized.");
#ifdef IS_PY3K
        return NULL;
#else
        return;
#endif
    }

#ifdef IS_PY3K
    return m;
#endif
}
