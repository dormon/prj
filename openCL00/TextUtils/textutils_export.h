
#ifndef TEXTUTILS_EXPORT_H
#define TEXTUTILS_EXPORT_H

#ifdef TEXTUTILS_STATIC_DEFINE
#  define TEXTUTILS_EXPORT
#  define TEXTUTILS_NO_EXPORT
#else
#  ifndef TEXTUTILS_EXPORT
#    ifdef TextUtils_EXPORTS
        /* We are building this library */
#      define TEXTUTILS_EXPORT 
#    else
        /* We are using this library */
#      define TEXTUTILS_EXPORT 
#    endif
#  endif

#  ifndef TEXTUTILS_NO_EXPORT
#    define TEXTUTILS_NO_EXPORT 
#  endif
#endif

#ifndef TEXTUTILS_DEPRECATED
#  define TEXTUTILS_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef TEXTUTILS_DEPRECATED_EXPORT
#  define TEXTUTILS_DEPRECATED_EXPORT TEXTUTILS_EXPORT TEXTUTILS_DEPRECATED
#endif

#ifndef TEXTUTILS_DEPRECATED_NO_EXPORT
#  define TEXTUTILS_DEPRECATED_NO_EXPORT TEXTUTILS_NO_EXPORT TEXTUTILS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef TEXTUTILS_NO_DEPRECATED
#    define TEXTUTILS_NO_DEPRECATED
#  endif
#endif

#endif /* TEXTUTILS_EXPORT_H */
