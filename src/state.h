#ifndef STATE_H
#include "xdg-shell-client-protocol.h"
#include <stdbool.h>
#include <wayland-client.h>

struct app_state {
  bool running;
  bool verbose;
  struct wl_display *display;
  struct wl_registry *registry;
  struct wl_shm *shm;
  struct wl_compositor *compositor;
  struct xdg_wm_base *wm_base;

  struct wl_surface *wl_surface;
  struct xdg_surface *xdg_surface;
  struct xdg_toplevel *toplevel;
};

#endif
