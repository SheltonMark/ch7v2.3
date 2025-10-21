/**************************************************************************************************
*
* Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
*
* This source file is the property of Axera Semiconductor Co., Ltd. and
* may not be copied or distributed in any isomorphic form without the prior
* written consent of Axera Semiconductor Co., Ltd.
*
*************************************************************************************************/

#ifndef _NIAL_MODULE_H_
#define _NIAL_MODULE_H_

#ifdef __NIAL_MODULE__
#undef __NIAL_MODULE__
#endif

#ifdef __NIAL_MODULE__

#define ___nial_PASTE(a, b) a##b
#define __nial_PASTE(a, b)  ___nial_PASTE(a, b)

#define __NIAL_UNIQUE_ID(prefix) __nial_PASTE(__nial_PASTE(__UNIQUE_ID_, prefix), __COUNTER__)

#ifdef MODULE
#define __NIAL_MODULE_INFO(tag, name, info)                              \
    static const char __NIAL_UNIQUE_ID(name)[] __attribute__((__used__)) \
    __attribute__((section(".modinfo"), unused, aligned(1))) = #tag "=" info
#else
#define __NIAL_MODULE_INFO(tag, name, info) \
    struct __NIAL_UNIQUE_ID(name) {         \
    }
#endif

#define NIAL_MODULE_INFO(tag, info) __NIAL_MODULE_INFO(tag, tag, info)

#define NIAL_MODULE_LICENSE(_license)         NIAL_MODULE_INFO(license, _license)
#define NIAL_MODULE_AUTHOR(_author)           NIAL_MODULE_INFO(author, _author)
#define NIAL_MODULE_DESCRIPTION(_description) NIAL_MODULE_INFO(description, _description)
#define NIAL_MODULE_VERSION(_version)         NIAL_MODULE_INFO(version, _version)

#if defined(LATENT_ENTROPY_PLUGIN) && !defined(__CHECKER__)
#define __nial_latent_entropy __attribute__((latent_entropy))
#else
#define __nial_latent_entropy
#endif

#if defined(__nial_noretpoline) && !defined(MODULE)
#define __nial_noinitretpoline __nial_noretpoline
#else
#define __nial_noinitretpoline
#endif

#ifdef RETPOLINE
#define __nial_noretpoline __attribute__((indirect_branch("keep")))
#else
#define __nial_noretpoline
#endif

#ifdef MODULE
#define __nial_exitused
#else
#define __nial_exitused __attribute__((__used__))
#endif

#if defined(CC_USING_HOTPATCH) && !defined(__CHECKER__)
#define nial_notrace __attribute__((hotpatch(0, 0)))
#else
#define nial_notrace __attribute__((no_instrument_function))
#endif

#define __nial_init \
    __attribute__((__section__(".init.text"))) __attribute__((cold)) __nial_latent_entropy __nial_noinitretpoline
#define __nial_exit __attribute__((__section__(".exit.text"))) __attribute__((cold)) __nial_exitused nial_notrace

typedef int (*nial_initcall_t)(void);
typedef void (*nial_exitcall_t)(void);

#define __nial_section(S) __attribute__((__section__(#S)))
#define __nial_exit_call  __nial_section(.exitcall.exit)

#ifndef MODULE
#define nial_module_exit(fn) static nial_exitcall_t __exitcall_##fn __attribute__((__used__)) __nial_exit_call = fn

#define nial_module_init(fn)           __nial_define_initcall(fn, 6)
#define __nial_define_initcall(fn, id) ___nial_define_initcall(fn, id, .initcall##id)

#ifdef CONFIG_HAVE_ARCH_PREL32_RELOCATIONS
#define ___nial_define_initcall(fn, id, __sec)        \
    __nial_ADDRESSABLE(fn) asm(".section   \"" #__sec \
                               ".init\", \"a\"  \n"   \
                               "__initcall_" #fn #id  \
                               ":            \n"      \
                               ".long  " #fn          \
                               " - .         \n"      \
                               ".previous                  \n");
#else
#define ___nial_define_initcall(fn, id, __sec)                                \
    static nial_initcall_t __nial_initcall_##fn##id __attribute__((__used__)) \
    __attribute__((__section__(#__sec ".init"))) = fn;
#endif

#else
#define NIAL_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if NIAL_GCC_VERSION >= 90100
#define __nia1_copy(symbol) __attribute__((__copy__(symbol)))
#else
#define __nia1_copy(symbol)
#endif

#define nial_module_init(initfn)                                           \
    static inline nial_initcall_t __attribute__((unused)) __inittest(void) \
    {                                                                      \
        return initfn;                                                     \
    }                                                                      \
    int init_module(void) __nia1_copy(initfn) __attribute__((alias(#initfn)));

#define nial_module_exit(exitfn)                                           \
    static inline nial_exitcall_t __attribute__((unused)) __exittest(void) \
    {                                                                      \
        return exitfn;                                                     \
    }                                                                      \
    void cleanup_module(void) __nia1_copy(exitfn) __attribute__((alias(#exitfn)));
#endif

#define __nial_ADDRESSABLE(sym)                                                                                       \
    static void *__attribute__((section(".discard.addressable"), used)) __nial_PASTE(__addressable_##sym, __LINE__) = \
        (void *)&sym;
#endif

#include <linux/module.h>

#if defined(__linux__) || defined(MODULE)

#define NIAL_MODULE_LICENSE(_license)         MODULE_LICENSE(_license)
#define NIAL_MODULE_AUTHOR(_author)           MODULE_AUTHOR(_author)
#define NIAL_MODULE_DESCRIPTION(_description) MODULE_DESCRIPTION(_description)
#define NIAL_MODULE_VERSION(_version)         MODULE_VERSION(_version)

#else
#define NIAL_MODULE_LICENSE(_license)
#define NIAL_MODULE_AUTHOR(_author)
#define NIAL_MODULE_DESCRIPTION(_description)
#define NIAL_MODULE_VERSION(_version)
#endif

#ifndef nial_module_init
#define nial_module_init(initfn, priority) module_init(initfn)
#endif
#define nial_module_exit(exitfn) module_exit(exitfn)

#define nial_module_param(name, type)               module_param(name, type, S_IRUGO | S_IWUSR)
#define nial_module_param_string(name, string, len) module_param_string(name, string, len, S_IRUGO)

#endif /* _NIAL_MODULE_H_ */
