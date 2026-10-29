# Introduction

This is my foray into the world of wayland. It did surprise me seeing how much "external" things are required to even get a simple window running. For instance, in this simple example (not easy, mind you), just for the window without any input handling, I had to use both the core wayland protocols as well the xdg extension.

# Small Tutorial

Since this is a learning project, I think I would be in the wrong to not share the details of how this project works:

## Setting up

It should be easier for you if you are not on NixOS, but the only thing you will need is wayland-client, wayland-protocols and wayland-scanner.

The first one is self-explanatory. In this case client would be your window and the server would be your compositor (like hyprland, gnome, KDE, and so on)
The second one brings with it the xdg extension protocol "spec".
`wayland-scanner` is actually used to produce glue code since the wayland "spec" is written in XML and is not particular useful to us as is.

## General Patterns

You will notice that this is very object oriented. At the end of the day, you are just passing messages to wayland server that you are hosting (the compositor) from your client. Optimizing for message passing makes it very data oriented. Who knew?

You can expect each 'object' to have fields, callbacks and constants. Similar to an object in a higher level language.

You can expect `x_get_y` for fields, registering callbacks using `x_add_listener`.

All objects are singletons, so I will be using the type, rather than a name for all object.

## The recipe for a simple window

You need to:

1. Connect to the `wl_display` using the `wl_display_connect` function.
2. Get its `wl_registry` using `wl_display_get_registry`.
3. Register a listener (using `wl_registry_add_listener`) that will bind wayland objects (using `wl_registry_bind`) to your application state. Meaning you get direct access to other wayland objects advertised by your compositor like `wl_compositor`, `wl_shm` and of course, the `xdg_*` objects.
5. Use the bound `wl_compositor` to create a `wl_surface` using `wl_compositor_create_surface`
4. Implement a function that does the pong for a `xdg_wm_base` ping event.
6. Make a `xdg_surface` using the `xdg_wm_base` and `wl_surface` (`xdg_wm_base_get_xdg_surface`).
7. Make a buffer to store the pixels inside the surface. Use `wl_shm` to make a `wl_shm_pool`, which will be used to create a `wl_buffer`, all with the size of the screen times the bit depth. The `wl_shm_pool` is similar to a builder class and can be destroyed (`wl_shm_pool_destroy`) after the `wl_bufffer` has been created. Here you would also make a listener to destroy the `wl_buffer` once it has been released.
8. Attach the `wl_buffer` to the `wl_surface` (`wl_surface_attach`)
9. Make sure to commit the surface (`wl_surface_commit`). Any information change you perform to `wl_surface` doesn't reflect what's on screen until you commit.
10. Get the `xdg_toplevel` from the `xdg_surface` (`xdg_surface_get_toplevel`)
11. Use `xdg_toplevel` to configure your window (`xdg_toplevel_set_title`, `xdg_set_max_size`, `xdg_set_min_size`, etc.)
12. Loop so that the window doesn't close.


...And you have a window... that doesn't close.

If you want to implement that, you will have to `xdg_toplevel_add_listener` the close function to destroy the `xdg_toplevel`, `xdg_surface` and `wl_surface` and exit the loop using an application state variable keep track of whether the main loop has to exit.

## Final Thoughts

As someone who regularly uses language with more features, it really feels like this protocol could really benefit from `this` or `self`, functions as first class citizens and much more. Writing this in a imperative only language with a prohibitively non-trivial metaprogramming like C, feels very tedious.
