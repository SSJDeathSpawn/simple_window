#define _GNU_SOURCE
#include <inttypes.h>
#include <linux/memfd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>
#include <wayland-client.h>

static void format_callback(void *data, struct wl_shm *registry,
                            uint32_t format) {
  printf("Format: 0x%" PRIX32 "\n", format);
}

const struct wl_shm_listener shm_listener = {.format = format_callback};

int allocate_shm_file(size_t size) {
  int fd = memfd_create("wayland-shm", MFD_CLOEXEC);
  if (fd < 0)
    return -1;

  if (ftruncate(fd, size) < 0) {
    close(fd);
    return -1;
  }

  return fd;
}
