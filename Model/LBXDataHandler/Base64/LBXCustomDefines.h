//
// LBXCustomDefines.h
//
//  Copyright 2008 Google Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License"); you may not
//  use this file except in compliance with the License.  You may obtain a copy
//  of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
//  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
//  License for the specific language governing permissions and limitations under
//  the License.
//

// ============================================================================

#include <AvailabilityMacros.h>
#include <TargetConditionals.h>

#if TARGET_OS_IPHONE
#include <Availability.h>
#endif //  TARGET_OS_IPHONE

// Not all MAC_OS_X_VERSION_10_X macros defined in past SDKs
#ifndef MAC_OS_X_VERSION_10_5
#define MAC_OS_X_VERSION_10_5 1050
#endif
#ifndef MAC_OS_X_VERSION_10_6
#define MAC_OS_X_VERSION_10_6 1060
#endif
#ifndef MAC_OS_X_VERSION_10_7
#define MAC_OS_X_VERSION_10_7 1070
#endif

// Not all __IPHONE_X macros defined in past SDKs
#ifndef __IPHONE_3_0
#define __IPHONE_3_0 30000
#endif
#ifndef __IPHONE_3_1
#define __IPHONE_3_1 30100
#endif
#ifndef __IPHONE_3_2
#define __IPHONE_3_2 30200
#endif
#ifndef __IPHONE_4_0
#define __IPHONE_4_0 40000
#endif
#ifndef __IPHONE_4_3
#define __IPHONE_4_3 40300
#endif
#ifndef __IPHONE_5_0
#define __IPHONE_5_0 50000
#endif

// ----------------------------------------------------------------------------
// CPP symbols that can be overridden in a prefix to control how the toolbox
// is compiled.
// ----------------------------------------------------------------------------


// By setting the LBXCustom_CONTAINERS_VALIDATION_FAILED_LOG and
// LBXCustom_CONTAINERS_VALIDATION_FAILED_ASSERT macros you can control what happens
// when a validation fails. If you implement your own validators, you may want
// to control their internals using the same macros for consistency.
#ifndef LBXCustom_CONTAINERS_VALIDATION_FAILED_ASSERT
#define LBXCustom_CONTAINERS_VALIDATION_FAILED_ASSERT 0
#endif

// Give ourselves a consistent way to do inlines.  Apple's macros even use
// a few different actual definitions, so we're based off of the foundation
// one.
#if !defined(LBXCustom_INLINE)
#if (defined (__GNUC__) && (__GNUC__ == 4)) || defined (__clang__)
#define LBXCustom_INLINE static __inline__ __attribute__((always_inline))
#else
#define LBXCustom_INLINE static __inline__
#endif
#endif

// Give ourselves a consistent way of doing externs that links up nicely
// when mixing objc and objc++
#if !defined (LBXCustom_EXTERN)
#if defined __cplusplus
#define LBXCustom_EXTERN extern "C"
#define LBXCustom_EXTERN_C_BEGIN extern "C" {
#define LBXCustom_EXTERN_C_END }
#else
#define LBXCustom_EXTERN extern
#define LBXCustom_EXTERN_C_BEGIN
#define LBXCustom_EXTERN_C_END
#endif
#endif

// Give ourselves a consistent way of exporting things if we have visibility
// set to hidden.
#if !defined (LBXCustom_EXPORT)
#define LBXCustom_EXPORT __attribute__((visibility("default")))
#endif

// Give ourselves a consistent way of declaring something as unused. This
// doesn't use __unused because that is only supported in gcc 4.2 and greater.
#if !defined (LBXCustom_UNUSED)
#define LBXCustom_UNUSED(x) ((void)(x))
#endif

// _LBXCustomDevLog & _LBXCustomDevAssert
//
// _LBXCustomDevLog & _LBXCustomDevAssert are meant to be a very lightweight shell for
// developer level errors.  This implementation simply macros to NSLog/NSAssert.
// It is not intended to be a general logging/reporting system.
//
// Please see http://code.google.com/p/google-toolbox-for-mac/wiki/DevLogNAssert
// for a little more background on the usage of these macros.
//
//    _LBXCustomDevLog           log some error/problem in debug builds
//    _LBXCustomDevAssert        assert if conditon isn't met w/in a method/function
//                           in all builds.
//
// To replace this system, just provide different macro definitions in your
// prefix header.  Remember, any implementation you provide *must* be thread
// safe since this could be called by anything in what ever situtation it has
// been placed in.
//

// We only define the simple macros if nothing else has defined this.
#ifndef _LBXCustomDevLog

#ifdef DEBUG
#define _LBXCustomDevLog(...) NSLog(__VA_ARGS__)
#else
#define _LBXCustomDevLog(...) do { } while (0)
#endif

#endif // _LBXCustomDevLog

#ifndef _LBXCustomDevAssert
// we directly invoke the NSAssert handler so we can pass on the varargs
// (NSAssert doesn't have a macro we can use that takes varargs)
#if !defined(NS_BLOCK_ASSERTIONS)
#define _LBXCustomDevAssert(condition, ...)                                       \
do {                                                                      \
if (!(condition)) {                                                     \
[[NSAssertionHandler currentHandler]                                  \
handleFailureInFunction:[NSString stringWithUTF8String:__PRETTY_FUNCTION__] \
file:[NSString stringWithUTF8String:__FILE__]  \
lineNumber:__LINE__                                  \
description:__VA_ARGS__];                             \
}                                                                       \
} while(0)
#else // !defined(NS_BLOCK_ASSERTIONS)
#define _LBXCustomDevAssert(condition, ...) do { } while (0)
#endif // !defined(NS_BLOCK_ASSERTIONS)

#endif // _LBXCustomDevAssert

// _LBXCustomCompileAssert
// _LBXCustomCompileAssert is an assert that is meant to fire at compile time if you
// want to check things at compile instead of runtime. For example if you
// want to check that a wchar is 4 bytes instead of 2 you would use
// _LBXCustomCompileAssert(sizeof(wchar_t) == 4, wchar_t_is_4_bytes_on_OS_X)
// Note that the second "arg" is not in quotes, and must be a valid processor
// symbol in it's own right (no spaces, punctuation etc).

// Wrapping this in an #ifndef allows external groups to define their own
// compile time assert scheme.
#ifndef _LBXCustomCompileAssert
// We got this technique from here:
// http://unixjunkie.blogspot.com/2007/10/better-compile-time-asserts_29.html

#define _LBXCustomCompileAssertSymbolInner(line, msg) _LBXCustomCOMPILEASSERT ## line ## __ ## msg
#define _LBXCustomCompileAssertSymbol(line, msg) _LBXCustomCompileAssertSymbolInner(line, msg)
#define _LBXCustomCompileAssert(test, msg) \
typedef char _LBXCustomCompileAssertSymbol(__LINE__, msg) [ ((test) ? 1 : -1) ]
#endif // _LBXCustomCompileAssert

// ----------------------------------------------------------------------------
// CPP symbols defined based on the project settings so the LBXCustom code has
// simple things to test against w/o scattering the knowledge of project
// setting through all the code.
// ----------------------------------------------------------------------------

// Provide a single constant CPP symbol that all of LBXCustom uses for ifdefing
// iPhone code.
#if TARGET_OS_IPHONE // iPhone SDK
// For iPhone specific stuff
#define LBXCustom_IPHONE_SDK 1
#if TARGET_IPHONE_SIMULATOR
#define LBXCustom_IPHONE_SIMULATOR 1
#else
#define LBXCustom_IPHONE_DEVICE 1
#endif  // TARGET_IPHONE_SIMULATOR
// By default, LBXCustom has provided it's own unittesting support, define this
// to use the support provided by Xcode, especially for the Xcode4 support
// for unittesting.
#ifndef LBXCustom_IPHONE_USE_SENTEST
#define LBXCustom_IPHONE_USE_SENTEST 0
#endif
#else
// For MacOS specific stuff
#define LBXCustom_MACOS_SDK 1
#endif

// Some of our own availability macros
#if LBXCustom_MACOS_SDK
#define LBXCustom_AVAILABLE_ONLY_ON_IPHONE UNAVAILABLE_ATTRIBUTE
#define LBXCustom_AVAILABLE_ONLY_ON_MACOS
#else
#define LBXCustom_AVAILABLE_ONLY_ON_IPHONE
#define LBXCustom_AVAILABLE_ONLY_ON_MACOS UNAVAILABLE_ATTRIBUTE
#endif

// Provide a symbol to include/exclude extra code for GC support.  (This mainly
// just controls the inclusion of finalize methods).
#ifndef LBXCustom_SUPPORT_GC
#if LBXCustom_IPHONE_SDK
// iPhone never needs GC
#define LBXCustom_SUPPORT_GC 0
#else
// We can't find a symbol to tell if GC is supported/required, so best we
// do on Mac targets is include it if we're on 10.5 or later.
#if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5
#define LBXCustom_SUPPORT_GC 0
#else
#define LBXCustom_SUPPORT_GC 1
#endif
#endif
#endif

// To simplify support for 64bit (and Leopard in general), we provide the type
// defines for non Leopard SDKs
#if !(MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5)
// NSInteger/NSUInteger and Max/Mins
#ifndef NSINTEGER_DEFINED
#if __LP64__ || NS_BUILD_32_LIKE_64
typedef long NSInteger;
typedef unsigned long NSUInteger;
#else
typedef int NSInteger;
typedef unsigned int NSUInteger;
#endif
#define NSIntegerMax    LONG_MAX
#define NSIntegerMin    LONG_MIN
#define NSUIntegerMax   ULONG_MAX
#define NSINTEGER_DEFINED 1
#endif  // NSINTEGER_DEFINED
// CGFloat
#ifndef CGFLOAT_DEFINED
#if defined(__LP64__) && __LP64__
// This really is an untested path (64bit on Tiger?)
typedef double CGFloat;
#define CGFLOAT_MIN DBL_MIN
#define CGFLOAT_MAX DBL_MAX
#define CGFLOAT_IS_DOUBLE 1
#else /* !defined(__LP64__) || !__LP64__ */
typedef float CGFloat;
#define CGFLOAT_MIN FLT_MIN
#define CGFLOAT_MAX FLT_MAX
#define CGFLOAT_IS_DOUBLE 0
#endif /* !defined(__LP64__) || !__LP64__ */
#define CGFLOAT_DEFINED 1
#endif // CGFLOAT_DEFINED
#endif  // MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5

// Some support for advanced clang static analysis functionality
// See http://clang-analyzer.llvm.org/annotations.html
#ifndef __has_feature      // Optional.
#define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif

#ifndef NS_RETURNS_RETAINED
#if __has_feature(attribute_ns_returns_retained)
#define NS_RETURNS_RETAINED __attribute__((ns_returns_retained))
#else
#define NS_RETURNS_RETAINED
#endif
#endif

#ifndef NS_RETURNS_NOT_RETAINED
#if __has_feature(attribute_ns_returns_not_retained)
#define NS_RETURNS_NOT_RETAINED __attribute__((ns_returns_not_retained))
#else
#define NS_RETURNS_NOT_RETAINED
#endif
#endif

#ifndef CF_RETURNS_RETAINED
#if __has_feature(attribute_cf_returns_retained)
#define CF_RETURNS_RETAINED __attribute__((cf_returns_retained))
#else
#define CF_RETURNS_RETAINED
#endif
#endif

#ifndef CF_RETURNS_NOT_RETAINED
#if __has_feature(attribute_cf_returns_not_retained)
#define CF_RETURNS_NOT_RETAINED __attribute__((cf_returns_not_retained))
#else
#define CF_RETURNS_NOT_RETAINED
#endif
#endif

#ifndef NS_CONSUMED
#if __has_feature(attribute_ns_consumed)
#define NS_CONSUMED __attribute__((ns_consumed))
#else
#define NS_CONSUMED
#endif
#endif

#ifndef CF_CONSUMED
#if __has_feature(attribute_cf_consumed)
#define CF_CONSUMED __attribute__((cf_consumed))
#else
#define CF_CONSUMED
#endif
#endif

#ifndef NS_CONSUMES_SELF
#if __has_feature(attribute_ns_consumes_self)
#define NS_CONSUMES_SELF __attribute__((ns_consumes_self))
#else
#define NS_CONSUMES_SELF
#endif
#endif

// Defined on 10.6 and above.
#ifndef NS_FORMAT_ARGUMENT
#define NS_FORMAT_ARGUMENT(A)
#endif

// Defined on 10.6 and above.
#ifndef NS_FORMAT_FUNCTION
#define NS_FORMAT_FUNCTION(F,A)
#endif

// Defined on 10.6 and above.
#ifndef CF_FORMAT_ARGUMENT
#define CF_FORMAT_ARGUMENT(A)
#endif

// Defined on 10.6 and above.
#ifndef CF_FORMAT_FUNCTION
#define CF_FORMAT_FUNCTION(F,A)
#endif

#ifndef LBXCustom_NONNULL
#define LBXCustom_NONNULL(x) __attribute__((nonnull(x)))
#endif

#ifdef __OBJC__

// Declared here so that it can easily be used for logging tracking if
// necessary. See LBXCustomUnitTestDevLog.h for details.
@class NSString;
LBXCustom_EXTERN void _LBXCustomUnitTestDevLog(NSString *format, ...);

// Macro to allow you to create NSStrings out of other macros.
// #define FOO foo
// NSString *fooString = LBXCustom_NSSTRINGIFY(FOO);
#if !defined (LBXCustom_NSSTRINGIFY)
#define LBXCustom_NSSTRINGIFY_INNER(x) @#x
#define LBXCustom_NSSTRINGIFY(x) LBXCustom_NSSTRINGIFY_INNER(x)
#endif

// Macro to allow fast enumeration when building for 10.5 or later, and
// reliance on NSEnumerator for 10.4.  Remember, NSDictionary w/ FastEnumeration
// does keys, so pick the right thing, nothing is done on the FastEnumeration
// side to be sure you're getting what you wanted.
#ifndef LBXCustom_FOREACH_OBJECT
#if TARGET_OS_IPHONE || !(MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_5)
#define LBXCustom_FOREACH_ENUMEREE(element, enumeration) \
for (element in enumeration)
#define LBXCustom_FOREACH_OBJECT(element, collection) \
for (element in collection)
#define LBXCustom_FOREACH_KEY(element, collection) \
for (element in collection)
#else
#define LBXCustom_FOREACH_ENUMEREE(element, enumeration) \
for (NSEnumerator *_ ## element ## _enum = enumeration; \
(element = [_ ## element ## _enum nextObject]) != nil; )
#define LBXCustom_FOREACH_OBJECT(element, collection) \
LBXCustom_FOREACH_ENUMEREE(element, [collection objectEnumerator])
#define LBXCustom_FOREACH_KEY(element, collection) \
LBXCustom_FOREACH_ENUMEREE(element, [collection keyEnumerator])
#endif
#endif

// ============================================================================

// To simplify support for both Leopard and Snow Leopard we declare
// the Snow Leopard protocols that we need here.
#if !defined(LBXCustom_10_6_PROTOCOLS_DEFINED) && !(MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6)
#define LBXCustom_10_6_PROTOCOLS_DEFINED 1
@protocol NSConnectionDelegate
@end
@protocol NSAnimationDelegate
@end
@protocol NSImageDelegate
@end
@protocol NSTabViewDelegate
@end
#endif  // !defined(LBXCustom_10_6_PROTOCOLS_DEFINED) && !(MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6)

// LBXCustom_SEL_STRING is for specifying selector (usually property) names to KVC
// or KVO methods.
// In debug it will generate warnings for undeclared selectors if
// -Wunknown-selector is turned on.
// In release it will have no runtime overhead.
#ifndef LBXCustom_SEL_STRING
#ifdef DEBUG
#define LBXCustom_SEL_STRING(selName) NSStringFromSelector(@selector(selName))
#else
#define LBXCustom_SEL_STRING(selName) @#selName
#endif  // DEBUG
#endif  // LBXCustom_SEL_STRING

#endif // __OBJC__
