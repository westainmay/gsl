#ifndef GSL_CONFIG_H 
#define GSL_CONFIG_H 
#ifndef NO_INLINE
#if (defined(__GNUC__) && !defined(__STRICT_ANSI__)) || defined (__cplusplus)
#define HAVE_INLINE 1
#endif 
#else
#undef HAVE_INLINE
#endif /* NO_INLINE */
#endif /* GSL_CONFIG_H */
