#include "constants.h"
#include "state.h"
#include "xdg-shell-client-protocol.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

extern struct wl_registry_listener wl_registry_listener_global;
extern struct wl_registry_listener wl_registry_listener;
extern struct xdg_surface_listener surface_listener;
extern struct xdg_toplevel_listener toplevel_listener;

int main(int argc, char **argv) {
  argc--;
  argv++;

  struct wl_display *display = wl_display_connect(NULL);
  struct wl_registry *registry = wl_display_get_registry(display);

  bool debug = false;
  if (argc > 0 && strncmp(argv[0], "-v", 2) == 0) {
    printf("Debug Mode on!\n");
    debug = true;
  } else {
    printf("Running in Normal mode\n");
    printf("Try debug mode by appending '-v'\n");
  }

  struct app_state state = {.display = display,
                            .registry = registry,
                            .running = true,
                            .verbose = debug};

  /* Communicate globals */
  printf("All communicated protocols from the compositor:\n");
  wl_registry_add_listener(registry, &wl_registry_listener_global, &state);
  wl_display_roundtrip(display);
  wl_registry_destroy(registry);

  printf("\n\n");

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
  xdg_toplevel_add_listener(state.toplevel, &toplevel_listener, &state);

  wl_surface_commit(state.wl_surface);

  while (state.running && wl_display_dispatch(state.display) != -1) {
  }

  wl_registry_destroy(state.registry);
  wl_display_disconnect(state.display);

  return 0;
}
