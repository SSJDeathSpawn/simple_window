#include "state.h"
#include <stdio.h>
#include <string.h>
#include <wayland-client.h>

static void registry_spit_globals(void *data, struct wl_registry *wl_registry,
                                  uint32_t name, const char *interface,
                                  uint32_t version) {
  struct app_state *state = data;
  if (!state->verbose && strncmp(interface, "wl_", 3) != 0 &&
      strncmp(interface, "xdg_", 4) != 0)
    return;
  printf("%s: v%d\n", interface, version);
}

static void registry_global_remove(void *data, struct wl_registry *wl_registry,
                                   uint32_t name) {}

struct wl_registry_listener wl_registry_listener_global = {
    .global = registry_spit_globals, .global_remove = registry_global_remove};
