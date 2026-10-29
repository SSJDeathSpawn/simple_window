#include "state.h"
#include "xdg-shell-client-protocol.h"
#include <stdbool.h>
#include <stdio.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

extern struct wl_buffer *get_buffer(struct app_state *state);

void xdg_configure(void *data, struct xdg_surface *xdg_surface,
                   uint32_t serial) {
  struct app_state *state = data;
  xdg_surface_ack_configure(xdg_surface, serial);

  struct wl_buffer *buffer = get_buffer(state);
  wl_surface_attach(state->wl_surface, buffer, 0, 0);
  wl_surface_commit(state->wl_surface);
}

struct xdg_surface_listener surface_listener = {.configure = xdg_configure};

void xdg_close(void *data, struct xdg_toplevel *xdg_toplevel) {
  struct app_state *state = data;
  xdg_toplevel_destroy(state->toplevel);
  xdg_surface_destroy(state->xdg_surface);
  wl_surface_destroy(state->wl_surface);
  state->running = false;
}

static void xdg_toplevel_configure(void *data,
                                   struct xdg_toplevel *xdg_toplevel,
                                   int32_t width, int32_t height,
                                   struct wl_array *states) {}

static void xdg_toplevel_configure_bounds(void *data,
                                          struct xdg_toplevel *xdg_toplevel,
                                          int32_t width, int32_t height) {}

static void xdg_toplevel_wm_capabilities(void *data,
                                         struct xdg_toplevel *xdg_toplevel,
                                         struct wl_array *capabilities) {}

struct xdg_toplevel_listener toplevel_listener = {
    .close = xdg_close,
    .configure = xdg_toplevel_configure,
    .configure_bounds = xdg_toplevel_configure_bounds,
    .wm_capabilities = xdg_toplevel_wm_capabilities};
