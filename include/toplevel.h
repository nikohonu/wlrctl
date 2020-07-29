#ifndef WLRCTL_TOPLEVEL_H
#define WLRCTL_TOPLEVEL_H

enum toplevel_attr {
	TOPLEVEL_ATTR_UNSPEC     = 0,
	TOPLEVEL_ATTR_APPID      = 1<<1,
	TOPLEVEL_ATTR_TITLE      = 1<<2,
	TOPLEVEL_ATTR_MAXIMIZED  = 1<<3,
	TOPLEVEL_ATTR_MINIMIZED  = 1<<4,
	TOPLEVEL_ATTR_ACTIVE     = 1<<5,
	TOPLEVEL_ATTR_FULLSCREEN = 1<<6,
};

enum toplevel_action {
	TOPLEVEL_ACTION_UNSPEC = 0,
	TOPLEVEL_ACTION_ACTIVATE,
	TOPLEVEL_ACTION_CLOSE,
	TOPLEVEL_ACTION_FIND,
	TOPLEVEL_ACTION_FULLSCREEN,
	TOPLEVEL_ACTION_LIST,
	TOPLEVEL_ACTION_MAXIMIZE,
	TOPLEVEL_ACTION_MINIMIZE,
};

struct toplevel_matchspec {
	unsigned int attrs;
	// toplevel attr
	struct wl_array app_ids;
	struct wl_array titles;
	// toplevel state
	bool maximized;
	bool minimized;
	bool active;
	bool fullscreen;
};

struct wlrctl_toplevel_command {
	enum toplevel_action action;
	struct toplevel_matchspec matchspec;
	struct wl_list toplevels;
	bool any;
	bool complete;
	struct wlrctl *state;
};


struct toplevel_data {
	char *app_id;
	char *title;
	struct wl_array state;
	struct wl_list link;
	struct wlrctl_toplevel_command *cmd;
	bool done;
};

void prepare_toplevel(struct wlrctl *state, int argc, char **argv);
void run_toplevel(struct wlrctl *state);
void stop_toplevel(struct wlrctl *state);
void destroy_toplevel(struct wlrctl *state);

#endif
