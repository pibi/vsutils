/*
 * Copyright (C) 2006-2013 Sebastien Vincent.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * \file dbg.h
 * \brief Some routines to print debug message.
 * \author Sebastien Vincent
 * \date 2006-2013
 */

#ifndef VSUTILS_DBG_H
#define VSUTILS_DBG_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef __cplusplus
extern "C"
{ /* } */
#endif

#include <sys/types.h>

/**
 * \def DBG_ATTR_FILE
 * \brief Current file and line seperated with a comma.
 */
#define DBG_ATTR_FILE __FILE__, __LINE__

#if __STDC_VERSION__ >= 199901L /* C99 */
/**
 * \def DBG_ATTR_FUNC
 * \brief Current file and line seperated with a comma.
 */
#define DBG_ATTR_FUNC __func__, __LINE__

/**
 * \def DBG_ATTR
 * \brief Current file and line seperated with a comma.
 */
#define DBG_ATTR DBG_ATTR_FUNC
#else
/**
 * \def DBG_ATTR
 * \brief Current file and line seperated with a comma.
 */
#define DBG_ATTR DBG_ATTR_FILE
#endif

/**
 * \brief Print a debug message on stderr.
 * \param f filename.
 * \param line line number.
 * \param format format of the output (similary to printf param).
 * \param ... list of arguments.
 * \author Sebastien Vincent
 */
void dbg_print(const char* f, int line, const char* format, ...);

/**
 * \brief Print the content of a buffer in hexadecimal.
 * \param f filename.
 * \param line line number.
 * \param buf buffer to print.
 * \param len size of the buffer.
 * \param format format of the output (similary to printf param).
 * \param ... list of arguments.
 * \author Sebastien Vincent
 * \warning Remember to pass pointer when you cast an integer for buf param.
 */
void dbg_print_hexa(const char* f, int line, const char* buf, size_t len,
    const char* format, ...);

/**
 * \def debug
 * \brief Print a debug message.
 *
 * Use similary like a variadic macro: debug(DBG_ATTR, format, ...).
 * \warning Respect the use: debug(DBG_ATTR, format, ...).
 */
#ifndef NDEBUG
#define debug dbg_print
#else
#define debug(...)
#endif

/**
 * \def debug_hexa
 * \brief Print the content of a buffer in hexadecimal.
 *
 * Use similary like a variadic macro:
 * debug_print_hexa(DBG_ATTR, buf, buflen, format, ...).
 * \warning Respect the use: debug_hexa(DBG_ATTR, buf, buflen, ...).
 */
#define debug_hexa dbg_print_hexa

/**
 * If you want to have debug message on stderr when some pthread functions are
 * used, define DBG_THREAD_LOCK. It could be useful when debugging deadlocks or
 * other thread synchronization stuff.
 */
#ifdef DBG_THREAD_LOCK

/**
 * \def pthread_mutex_lock
 * \brief Print a debug message when pthread_mutex_lock function is used.
 * \param x thread id (pthread_t type).
 * \return 0 if success, a non nul value otherwise.
 */
#define pthread_mutex_lock(x) \
  do \
  { \
    dbg_print(DBG_ATTR, "MUTEX LOCK: [%x]\n", pthread_self()); \
    pthread_mutex_lock((x)); \
  }while(0)

/**
 * \def pthread_mutex_unlock
 * \brief Print a debug message when pthread_mutex_unlock function is used.
 * \param x thread id (pthread_t type).
 * \return 0 if success, a non nul value otherwise.
 */
#define pthread_mutex_unlock(x) \
  do \
  { \
    dbg_print(DBG_ATTR, "MUTEX UNLOCK: [%x]\n", pthread_self()); \
    pthread_mutex_unlock((x)); \
  }while(0)

/**
 * \def pthread_join
 * \brief Print a debug message when pthread_join function is used.
 * \param x thread id (pthread_t type).
 * \param r return value of thread is stored in r (void** type).
 * \return 0 if success, a non nul value otherwise.
 */
#define pthread_join(x, r) \
  do \
  {
    dbg_print(DBG_ATTR, "[%x] wait to JOIN Thread [%x]\n", pthread_self(), x); \
    pthread_join((x), (r)); \
    dbg_print(DBG_ATTR, "[%x] JOIN Thread [%x]\n", pthread_self(), x); \
  }while(0)

/**
 * \def pthread_exit
 * \brief Print a debug message when pthread_exit function is used.
 * \param x thread id (pthread_t type).
 */
#define pthread_exit(x) \
  do \
  { \
    dbg_print(DBG_ATTR, "EXIT Thread [%x]\n", pthread_self()); \
    pthread_exit((x)); \
  }while(0)

/**
 * \def pthread_cancel
 * \brief Print a debug message when pthread_exit function is used.
 * \param x thread id (pthread_t type).
 * \return 0 if success, a non nul value otherwise.
 */
#define pthread_cancel(x) \
  do \
  { \
    dbg_print(DBG_ATTR, "Cancel Thread [%x] by [%x]\n", x, pthread_self()); \
  }while(0)

#endif

#ifdef __cplusplus
}
#endif

#endif /* VSUTILS_DBG_H */

