#ifndef lrc_h
#define lrc_h
#include "llimits.h"
/*
 ** Union of all Lua values
 */
typedef union Value {
    struct GCObject *gc;    /* collectable objects */
    void *p;         /* light userdata */
    lua_CFunction f; /* light C functions */
    lua_Integer i;   /* integer numbers */
    lua_Number n;    /* float numbers */
} Value;


/*
 ** Tagged Values. This is the basic representation of values in Lua:
 ** an actual value plus a tag with its type.
 */

#define TValuefields	Value value_; lu_byte tt_

typedef struct TValue {
    TValuefields;
} TValue;

/*
 ** {==================================================================
 ** Collectable Objects
 ** ===================================================================
 */

/*
 ** Common Header for all collectable objects (in macro form, to be
 ** included in other objects)
 */
#define CommonHeader	union{struct GCObject *next; l_mem count;}; lu_byte tt; lu_byte marked


/* Common type for all collectable objects */
typedef struct GCObject {
    CommonHeader;
} GCObject;

void luaRC_deinit(lua_State* L);

l_mem luaRC_fix_object(lua_State* L, GCObject* o);
l_mem luaRC_addref_object(lua_State* L, GCObject* o);
l_mem luaRC_subref_object(lua_State* L, GCObject* o);

l_mem luaRC_fix(lua_State* L, TValue* d);
l_mem luaRC_addref(lua_State* L, TValue* d);
l_mem luaRC_subref(lua_State* L, TValue* d);

#define is_collectable(tt) (((tt == LUA_VLCL) || (tt == LUA_VCCL) || ((tt & 0x0f) >= LUA_TSTRING)))

/* Macros to set values */
#define setclCvalue_subref(L,obj,x) \
{ TValue *io = (obj); CClosure *x_ = (x); \
val_(io).gc = obj2gco(x_); settt_(io, ctb(LUA_VCCL)); \
luaRC_subref(L,io); checkliveness(L,io); }

/* Set nil to obj first, then subref it.
 * Subref may cause object release and GCTM execution
 * which in turn may check this tvalue.
 */
#define setnilvalue_subref(L,obj) \
{ TValue *i_v=(obj); TValue dup=*i_v; \
settt_(i_v,LUA_TNIL); luaRC_subref(L,&dup); }

#define setnvalue_subref(L,obj,x) \
{ TValue *i_v=(obj); TValue dup=*i_v; \
i_v->value_.n=(x); settt_(i_v,LUA_TNUMBER); \
luaRC_subref(L,&dup); }

#define setivalue_subref(L,obj,x) \
{ TValue *i_v=(obj); TValue dup=*i_v; \
i_v->value_.i=(x); settt_(i_v,LUA_VNUMINT); \
luaRC_subref(L,&dup); }

#define setfltvalue_subref(L,obj,x) \
{ TValue *i_v=(obj); TValue dup=*i_v; \
i_v->value_.n=(x); settt_(i_v,LUA_VNUMFLT); \
luaRC_subref(L,&dup); }


#define setpvalue_subref(L,obj,x) \
{ TValue *i_v=(obj); TValue dup=*i_v; \
i_v->value_.p=(x); settt_(i_v,LUA_TLIGHTUSERDATA); \
luaRC_subref(L,&dup); }

#define setsvalue_subref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
TValue dup=*i_v; i_v->value_.gc=i_o; settt_(i_v,LUA_TSTRING); \
luaRC_subref(L,&dup); \
checkliveness(G(L),i_v); }

#define setuvalue_subref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
TValue dup=*i_v; i_v->value_.gc=i_o;settt_(i_v,LUA_TUSERDATA); \
luaRC_subref(L,&dup); \
checkliveness(G(L),i_v); }

#define setthvalue_subref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
TValue dup=*i_v; i_v->value_.gc=i_o; settt_(i_v,LUA_TTHREAD);\
luaRC_subref(L,&dup); \
checkliveness(G(L),i_v); }

#define setclvalue_subref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
TValue dup=*i_v; i_v->value_.gc=i_o;settt_(i_v,LUA_TFUNCTION); \
luaRC_subref(L,&dup); \
checkliveness(G(L),i_v); }

#define sethvalue_subref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
TValue dup=*i_v; i_v->value_.gc=i_o; settt_(i_v,LUA_TTABLE); \
luaRC_subref(L,&dup); \
checkliveness(G(L),i_v); }

#define setptvalue_subref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
TValue dup=*i_v; i_v->value_.gc=i_o; settt_(i_v,LUA_TPROTO); \
luaRC_subref(L,&dup); \
checkliveness(G(L),i_v); }

#define setobj_subref(L,obj1,obj2) \
{ TValue *o2=(TValue *)(obj2); TValue *o1=(obj1); \
TValue dup=*o1; o1->value_ = o2->value_; settt_(o1,o2->tt_); \
luaRC_subref(L,&dup); \
checkliveness(G(L),o1); }

/* to new object */
#define setnilvalue_to_new(obj) (settt_(obj,LUA_TNIL))

#define setnvalue_to_new(obj,x) \
{ TValue *i_o=(obj); i_o->value_.n=(x); settt_(i_v,LUA_TNUMBER); }

#define setpvalue_to_new(obj,x) \
{ TValue *i_o=(obj); i_o->value_.p=(x); settt_(i_v,LUA_TLIGHTUSERDATA); }

#define setbvalue_to_new(obj,x) \
{ TValue *i_o=(obj); i_o->value_.b=(x); settt_(i_v,LUA_TBOOLEAN); }

#define setsvalue_to_new_addref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
i_v->value_.gc=i_o; settt_(i_v,LUA_TSTRING); \
luaRC_addref(L, i_v);checkliveness(G(L),i_v); }

#define setuvalue_to_new_addref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
i_v->value_.gc=i_o; settt_(i_v,LUA_TUSERDATA); \
luaRC_addref(L, i_v);checkliveness(G(L),i_v); }

#define setthvalue_to_new_addref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
i_v->value_.gc=i_o; settt_(i_v,LUA_TTHREAD); \
luaRC_addref(L, i_v);checkliveness(G(L),i_v); }

#define setclvalue_to_new_addref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
i_v->value_.gc=i_o; settt_(i_v,LUA_TFUNCTION); \
luaRC_addref(L, i_v);checkliveness(G(L),i_v); }

#define sethvalue_to_new_addref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
i_v->value_.gc=i_o; settt_(i_v,LUA_TTABLE); \
luaRC_addref(L, i_v); checkliveness(G(L),i_v); }

#define setptvalue_to_new_addref(L,obj,x) \
{ TValue *i_v=(obj); GCObject *i_o=cast(GCObject *, (x)); \
i_v->value_.gc=i_o; settt_(i_v,LUA_TPROTO); \
luaRC_addref(L, i_v);checkliveness(G(L),i_v); }

#define setobj_to_new_addref(L,obj1,obj2) \
{ TValue *o2=(TValue *)(obj2); TValue *o1=(obj1); \
o1->value_ = o2->value_; settt_(o1,o2->tt_); \
luaRC_addref(L, o2); checkliveness(G(L),o1); }



#endif
