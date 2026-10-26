#include "constants.h"
#include "state.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wayland-client-protocol.h>
#include <wayland-client.h>

extern int allocate_shm_file(size_t size);

void buffer_release(void *data, struct wl_buffer *wl_buffer) {
  wl_buffer_destroy(wl_buffer);
}

struct wl_buffer_listener buffer_listener = {.release = buffer_release};

struct wl_buffer *get_buffer(struct app_state *state) {
  int fd;
  if ((fd = allocate_shm_file(SIZE)) == -1) {
    return NULL;
  }

  uint32_t *data = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (data == MAP_FAILED) {
    close(fd);
    return NULL;
  }

  struct wl_shm_pool *pool = wl_shm_create_pool(state->shm, fd, SIZE);
  struct wl_buffer *buffer = wl_shm_pool_create_buffer(
      pool, 0, WIDTH, HEIGHT, STRIDE, WL_SHM_FORMAT_XRGB8888);

  wl_shm_pool_destroy(pool);
  close(fd);
  // memset(data, 0, SIZE);
  for (int y = 0; y < HEIGHT; ++y) {
    for (int x = 0; x < WIDTH; ++x) {
      data[y * WIDTH + x] = 0xFF0E1D47;
    }
  }

  munmap(data, SIZE);
  wl_buffer_add_listener(buffer, &buffer_listener, NULL);
  return buffer;
}
