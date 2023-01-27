
#ifndef VNX_BASE_EXPORT_H
#define VNX_BASE_EXPORT_H

#ifdef VNX_BASE_STATIC_DEFINE
#  define VNX_BASE_EXPORT
#  define VNX_BASE_NO_EXPORT
#else
#  ifndef VNX_BASE_EXPORT
#    ifdef vnx_base_EXPORTS
        /* We are building this library */
#      define VNX_BASE_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VNX_BASE_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VNX_BASE_NO_EXPORT
#    define VNX_BASE_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VNX_BASE_DEPRECATED
#  define VNX_BASE_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef VNX_BASE_DEPRECATED_EXPORT
#  define VNX_BASE_DEPRECATED_EXPORT VNX_BASE_EXPORT VNX_BASE_DEPRECATED
#endif

#ifndef VNX_BASE_DEPRECATED_NO_EXPORT
#  define VNX_BASE_DEPRECATED_NO_EXPORT VNX_BASE_NO_EXPORT VNX_BASE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VNX_BASE_NO_DEPRECATED
#    define VNX_BASE_NO_DEPRECATED
#  endif
#endif

#endif /* VNX_BASE_EXPORT_H */
