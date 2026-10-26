#include "constants.h"
#include "state.h"
#include "xdg-shell-client-protocol.h"
#include <stdio.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

extern struct wl_registry_listener wl_registry_listener_global;
extern struct wl_registry_listener wl_registry_listener;
extern struct xdg_surface_listener surface_listener;

int main() {
  struct wl_display *display = wl_display_connect(NULL);
  struct wl_registry *registry = wl_display_get_registry(display);

  /* Communicate globals */
  wl_registry_add_listener(registry, &wl_registry_listener_global, NULL);
  wl_display_roundtrip(display);
  wl_registry_destroy(registry);

  printf("\n\n\n");

  struct app_state state = {.display = display, .registry = registry};
  registry = wl_display_get_registry(display);
  /* Bind all objects to state */
  wl_registry_add_listener(registry, &wl_registry_listener, &state);
  wl_display_roundtrip(display);

  state.wl_surface = wl_compositor_create_surface(state.compositor);
  state.xdg_surface =
      xdg_wm_base_get_xdg_surface(state.wm_base, state.wl_surface);
  xdg_surface_add_listener(state.xdg_surface, &surface_listener, &state);
  state.toplevel = xdg_surface_get_toplevel(state.xdg_surface);
  xdg_toplevel_set_title(state.toplevel, "Test Window");
  xdg_toplevel_set_max_size(state.toplevel, WIDTH, HEIGHT);
  xdg_toplevel_set_min_size(state.toplevel, WIDTH, HEIGHT);

  wl_surface_commit(state.wl_surface);

  while (wl_display_dispatch(state.display)) {
  }

  return 0;
}
