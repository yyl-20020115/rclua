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
#define CommonHeader	struct GCObject *next; lu_byte tt; lu_byte marked


/* Common type for all collectable objects */
typedef struct GCObject {
  CommonHeader;
} GCObject;




int luaRC_set_main_lua_State(lua_State* lua_State);

lua_State* luaRC_get_main_lua_State(void);


void luaRC_ensure_deinit(void);

int luaRC_addref(lua_State* L, GCObject* o);

int luaRC_settt_(TValue* o, lu_byte t);

int luaRC_set_enable_rc(int enable);
int luaRC_get_enable_rc(void);

#endif
