/**
 * \file netevt.h
 * \brief Network event manager.
 * \author Sebastien Vincent
 * \date 2014
 */

#ifndef VSUTILS_NETEVT_H
#define VSUTILS_NETEVT_H

#include <stdio.h>

#if !defined(_WIN32) && !defined(_WIN64)
#include <sys/types.h>
#include <sys/socket.h>
#else
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include "list.h"

/**
 * \def NETEVT_STATE_READ
 * \brief Event read state.
 */
#define NETEVT_STATE_READ 1

/**
 * \def NETEVT_STATE_WRITE
 * \brief Event write state.
 */
#define NETEVT_STATE_WRITE 2

/**
 * \def NETEVT_STATE_OTHER
 * \brief Event other state.
 */
#define NETEVT_STATE_OTHER 4

/**
 * \typedef netevt
 * \brief Opaque type to define network event manager.
 */
typedef struct netevt* netevt;

/**
 * \enum netevt_method
 * \brief Underlying method for event trigger.
 */
enum netevt_method
{
  NETEVT_AUTO, /**< Let system selects the best method. */
  NETEVT_SELECT, /**< POSIX "select" method. */
  NETEVT_POLL, /**< POSIX "poll" method. */
  NETEVT_EPOLL, /**< Linux "epoll" method. */
  NETEVT_KQUEUE /**< *BSD and Mac OS X "kqueue" method. */
};

/**
 * \struct netevt_socket
 * \brief Network socket.
 */
struct netevt_socket
{
  int sock; /**< Socket descriptor. */
  struct sockaddr_storage local; /**< Local address. */
  void* data; /**< User data. */
  struct list_head list; /**< For list management. */
};

/**
 * \struct netevt_event
 * \brief Network event.
 */
struct netevt_event
{
  struct netevt_socket socket; /**< Network socket. */
  int state; /**< Event state. */
};

/**
 * \struct netevt_impl
 * \brief Event method specific (select, poll, ...) implementation.
 */
struct netevt_impl
{
  /**
   * \brief Wait network events.
   */
  int (*wait)(struct netevt_impl* impl, netevt obj, int timeout,
      struct netevt_event* events, size_t nb_events);

  /**
   * \brief Add a socket.
   */
  int (*add_socket)(struct netevt_impl* impl, netevt obj,
      struct netevt_socket* sock, int event_mask);

  /**
   * \brief Remove a socket.
   */
  int (*remove_socket)(struct netevt_impl* impl, netevt obj,
      struct netevt_socket* sock);

  void* priv; /**< Private data for the implementation. */  
};

/**
 * \brief Returns whether or not the method is supported.
 * \param method method to test if supported.
 * \return 1 if method is supported, 0 otherwise.
 */
int netevt_is_method_supported(enum netevt_method method);

/**
 * \brief Create a new network event manager.
 * \param method method to use.
 * \return new network event manager or NULL if failure.
 */
struct netevt* netevt_new(enum netevt_method method);

/**
 * \brief Delete a network event manager.
 * \param obj pointer on a network event manager.
 */
void netevt_free(netevt* obj);

/**
 * \brief Add a socket to monitore by the manager.
 * \param obj network event manager.
 * \param sock socket descriptor.
 * \param event_mask combination of NETEVT_STATE_* flag (read, write, ...).
 * \param data user data.
 * \return 0 if success, -1 otherwise.
 */
int netevt_add_socket(netevt obj, int sock, int event_mask, void* data);

/**
 * \brief Remove a socket from the manager.
 * \param obj network event manager.
 * \param sock socket descriptor.
 * \return 0 if success, -1 otherwise.
 */
int netevt_remove_socket(netevt obj, int sock);

/**
 * \brief Remove all sockets.
 * \param obj network event manager.
 * \return 0 if success, -1 otherwise.
 */
int netevt_remove_all_sockets(netevt obj);

/**
 * \brief Wait for network events.
 * \param obj network event manager.
 * \param timeout timeout in second or -1 for infinite.
 * \param events allocated array of netevt_event.
 * \param nb_events number of elements in events array.
 * \return 0 if timeout, number of elements notified if success and -1 if error.
 */
int netevt_wait(netevt obj, int timeout, struct netevt_event* events, size_t nb_events);

/**
 * \brief Get number of sockets registered in the manager.
 * \param obj network event manager.
 * \return Number of socket registered if success, -1 otherwise.
 */
int netevt_get_nb_sockets(netevt obj);

/**
 * \brief Returns copy array of netevt_socket.
 * \param obj network event manager.
 * \param sockets_nb number of element in the returning array.
 * \return Valid netevt_socket array, NULL if no sockets or if memory problem (check errno).
 */
struct netevt_socket* netevt_get_sockets(netevt obj, size_t* sockets_nb);

/**
 * \brief Returns list of netevt_socket.
 * \param obj network event manager.
 * \return list of sockets. It is the internal list of the manager so do not
 * modify it.
 */
struct list_head* netevt_get_sockets_list(netevt obj);

/**
 * \brief Print in a file some information about network event manager.
 * \param obj network event manager.
 * \param output output file.
 */ 
void netevt_fprint_info(netevt obj, FILE* output);

/**
 * \brief Print in stdout some information about network event manager.
 * \param obj network event manager.
 */ 
void netevt_print_info(netevt obj);

#endif /* VSUTILS_NETEVT_H */

