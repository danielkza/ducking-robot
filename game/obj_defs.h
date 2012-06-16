#ifndef CLS_DEFS_H
#define CLS_DEFS_H

#ifdef _MSC_VER
#define inline __inline
#endif

#define CLS_DEF_GETTER(eclass, type, member) \
	static inline type \
	eclass##_get_##member(eclass *obj) { return obj->member; }

#define CLS_DEF_SETTER(eclass, type, member) \
	static inline void \
	eclass##_set_##member(eclass *obj, const type val) { obj->member = val; }

#define CLS_DEF_ACCESSOR(eclass, type, member) \
	CLS_DEF_GETTER(eclass, type, member); \
	CLS_DEF_SETTER(eclass, type, member)


#define CLS_DEF_GETTER_INDIRECT(eclass, type, member) \
	static inline const type * \
	eclass##_get_##member(eclass *obj) { return &obj->member; }

#define CLS_DEF_SETTER_INDIRECT(eclass, type, member) \
	static inline void \
	eclass##_set_##member(eclass *obj, const type *val) { obj->member = *val; }

#define CLS_DEF_ACCESSOR_INDIRECT(eclass, type, member) \
	CLS_DEF_GETTER_INDIRECT(eclass, type, member); \
	CLS_DEF_SETTER_INDIRECT(eclass, type, member)


#define CLS_DEF_GET(eclass, member, obj) \
    eclass##_get_##member((eclass*)(obj))

#define CLS_DEF_SET(eclass, member, obj, ...) \
    eclass##_set_##member((eclass*)(obj), __VA_ARGS__)

#define CLS_DEF_CALL(eclass, method, obj, ...) \
    (*((eclass*)(obj))->m_##method)((eclass*)(obj), __VA_ARGS__) 

#endif
