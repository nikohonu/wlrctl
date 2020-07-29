# wlrctl

wlrctl is a command line utility for miscellaneous wlroots Wayland extensions.

At this time, wlrctl supports the foreign-toplevel-mangement (window/toplevel command),
virtual-keyboard (keyboard command), and virtual-pointer (pointer command) protocols.

## Installation

There is an AUR package for wlrctl [here][aur-wlrctl].

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

[aur-wlrctl]: https://aur.archlinux.org/packages/wlrctl
