# wlrctl

wlrctl is a command line utility for miscellaneous wlroots Wayland extensions.

At this time, wlrctl supports the foreign-toplevel-mangement (window/toplevel command),
virtual-keyboard (keyboard command), and virtual-pointer (pointer command) protocols.

## Warning

At the time of this writing, the release version of wlroots has serious session breaking
bugs and crashes related to these protocols. If you want to use wlrctl with wlroots/sway
you will need a wlroots 0.12+ or sway 1.6+ version.

## Installation

There is an AUR package for wlrctl [here][aur-wlrctl].
And an openSUSE package [here][os-wlrctl].

Otherwise, build with meson/ninja e.g.

    $ meson setup --prefix=/usr/local build
	$ ninja -C build install

## Features and Examples

wlrctl is still experimental, and has just a few basic features.
Check the man page wlrctl(1) for full details.

Some example uses are:

    $ wlrctl keyboard type 'Hello, world!'

... to type some text using a virtual keyboard.

    $ wlrctl pointer move 50 -70

... to move the cursor 50 pixels right and 70 pixels up.

    $ wlrctl window focus firefox || swaymsg exec firefox

... to focus firefox if it is running, otherwise start firefox.

    $ wlrctl toplevel waitfor mpv state:fullscreen && makoctl dismiss

... to dismiss desktop notifications when mpv becomes fullscreen


## Contributing

You can send patches to the [mailing list][list-wlrctl] or submit an issue on the
[issue tracker][todo-wlrctl].

[aur-wlrctl]: https://aur.archlinux.org/packages/wlrctl
[os-wlrctl]: https://build.opensuse.org/package/show/X11:Wayland/wlrctl
[todo-wlrctl]: https://todo.sr.ht/~brocellous/wlrctl
[list-wlrctl]: https://lists.sr.ht/~brocellous/public-inbox
