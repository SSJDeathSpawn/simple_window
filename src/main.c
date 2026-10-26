#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

static void registry_handle_global(void *data, struct wl_registry *registry,
                                   uint32_t name, const char *interface,
                                   uint32_t version) {
  if (strncmp(interface, "wl_", 3) != 0 && strncmp(interface, "xdg_", 4) != 0) {
    return;
  }

  printf("%s v%d, name: %d\n", interface, version, name);
}

static void registry_handle_global_remove(void *data,
                                          struct wl_registry *registry,
                                          uint32_t name) {}

static const struct wl_registry_listener registry_listener = {
    .global = registry_handle_global,
    .global_remove = registry_handle_global_remove};

int main() {
  printf("Hello world!\n");

  struct wl_display *display;
  if (!(display = wl_display_connect(NULL))) {
    perror("wl_display_connect");
    return 1;
  }

  struct wl_event_queue *event_queue;
  if (!(event_queue = wl_display_create_queue(display))) {
    perror("wl_display_create_queue");
    return 1;
  }

  struct wl_registry *registry;
  if (!(registry = wl_display_get_registry(display))) {
    perror("wl_display_get_registry");
    return 1;
  }
  printf("Advertising all registry objects:\n");
  wl_registry_add_listener(registry, &registry_listener, NULL);

  wl_display_roundtrip(display);

  wl_display_disconnect(display);

  return 0;
}
