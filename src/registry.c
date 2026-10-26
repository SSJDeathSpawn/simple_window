#include "state.h"
#include "xdg-shell-client-protocol.h"
#include <stdio.h>
#include <string.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base,
                      uint32_t serial) {
  xdg_wm_base_pong(xdg_wm_base, serial);
}

static struct xdg_wm_base_listener wm_base_listener = {.ping =
                                                           xdg_wm_base_ping};

void registry_global(void *data, struct wl_registry *wl_registry, uint32_t name,
                     const char *interface, uint32_t version) {
  struct app_state *state = data;
  if (strcmp(interface, wl_compositor_interface.name) == 0) {
    state->compositor =
        wl_registry_bind(wl_registry, name, &wl_compositor_interface, version);
    printf("Bound wl_compositor!\n");
  } else if (strcmp(interface, wl_shm_interface.name) == 0) {
    state->shm =
        wl_registry_bind(wl_registry, name, &wl_shm_interface, version);
    printf("Bound wl_shm!\n");
  } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
    state->wm_base =
        wl_registry_bind(wl_registry, name, &xdg_wm_base_interface, version);
    xdg_wm_base_add_listener(state->wm_base, &wm_base_listener, NULL);
    printf("Bound xdg_wm_base!\n");
  }
}

void registry_global_remove(void *data, struct wl_registry *wl_registry,
                            uint32_t name) {}

struct wl_registry_listener wl_registry_listener = {
    .global = registry_global, .global_remove = registry_global_remove};
