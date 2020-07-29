#define _GNU_SOURCE
#include <assert.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wayland-client.h>
#include <xkbcommon/xkbcommon.h>
#include "common.h"
#include "keyboard.h"
#include "util.h"

#include "virtual-keyboard-unstable-v1-client-protocol.h"

extern const char keymap_ascii_raw[];

static void
get_keymap(struct wlrctl_keyboard_command *cmd)
{
	int size = strlen(keymap_ascii_raw) + 1;

	int fd = memfd_create("keymap", 0);
	ftruncate(fd, size);
	void *keymap_data =
		mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	strcpy(keymap_data, keymap_ascii_raw);

	munmap(keymap_data, size);
	cmd->keymap.format = WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1;
	cmd->keymap.fd = fd;
	cmd->keymap.size = size;
}

static void
send_key(struct zwp_virtual_keyboard_v1 *kbd, char c)
{
	zwp_virtual_keyboard_v1_key(kbd, timestamp(), c - 8, WL_KEYBOARD_KEY_STATE_PRESSED);
	zwp_virtual_keyboard_v1_key(kbd, timestamp(), c - 8, WL_KEYBOARD_KEY_STATE_RELEASED);
}

static void
keyboard_type(struct wlrctl_keyboard_command *cmd, const char *text)
{
	int len = strlen(text);
	for (int i = 0; i < len; i++) {
		send_key(cmd->device, text[i]);
	}
}

static void
complete_keyboard(void *data, struct wl_callback *callback, uint32_t serial)
{
	struct wlrctl *state = data;
	wl_callback_destroy(callback);
	state->running = false;
	destroy_keyboard(state);
}

static struct wl_callback_listener completed_listener = {
	.done = complete_keyboard
};

static enum keyboard_action
parse_action(const char *action)
{
	static const struct token actions[] = {
		{"type", KEYBOARD_ACTION_TYPE},
		{NULL, KEYBOARD_ACTION_UNSPEC}
	};
	return matchtok(actions, action);
}

static bool
is_ascii(const char str[])
{
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] < 0) {
			return false;
		}
	}
	return true;
}

void
prepare_keyboard(struct wlrctl *state, int argc, char *argv[])
{
	struct wlrctl_keyboard_command *cmd =
		calloc(1, sizeof (struct wlrctl_keyboard_command));
	assert(cmd);

	if (argc == 0) {
		die("Missing keyboard action\n");
	}

	char *action = argv[0];
	cmd->action = parse_action(action);

	switch (cmd->action) {
	case KEYBOARD_ACTION_TYPE:
		if (argc < 2) {
			die("Missing text to type!\n");
		} else if (argc == 2) {
			if (is_ascii(argv[1])) {
				cmd->text = strdup(argv[1]);
			} else {
				die("Only ascii strings are currently supported\n");
			}
		} else {
			die("Extra argument: '%s'\n", argv[2]);
		}
		break;
	case KEYBOARD_ACTION_UNSPEC:
		die("Unknown keyboard action: '%s'\n", action);
		break;
	}

	cmd->state = state;
	state->cmd = cmd;
}

void
run_keyboard(struct wlrctl *state)
{
	struct wlrctl_keyboard_command *cmd = state->cmd;

	cmd->device =
	zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(
		state->vkbd_mgr, state->seat
	);

	cmd->xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
	get_keymap(cmd);

	zwp_virtual_keyboard_v1_keymap(cmd->device,
		cmd->keymap.format, cmd->keymap.fd, cmd->keymap.size
	);
	close(cmd->keymap.fd);

	switch (cmd->action) {
	case KEYBOARD_ACTION_TYPE:
		zwp_virtual_keyboard_v1_modifiers(cmd->device, 0, 0, 0, 0);
		keyboard_type(cmd, cmd->text);
		wl_display_flush(state->display);
		break;
	default:
		break;
	}

	struct wl_callback *callback = wl_display_sync(state->display);
	wl_callback_add_listener(callback, &completed_listener, state);
}

void destroy_keyboard(struct wlrctl *state)
{
	struct wlrctl_keyboard_command *cmd = state->cmd;
	zwp_virtual_keyboard_v1_destroy(cmd->device);
	free(cmd);
}
