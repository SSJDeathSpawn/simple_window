#include "state.h"
#include "xdg-shell-client-protocol.h"
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
