/*
 * Copyright (C) 2015 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "config.h"
#include "user-support.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils.h"

void setup_user_data(uid_t uid, gid_t gid)
{
	const char *user_data = getenv("SNAP_USER_DATA");
	if (user_data == NULL)
		return;
	// Only support absolute paths.
	if (user_data[0] != '/') {
		die("user data directory must be an absolute path");
	}
	// Describe how the created directory should look like.
	int hint_SNAP_USER_DATA(int depth, const char *segment,
				mode_t * mode_p, uid_t * uid_p, gid_t * gid_p) {
		switch (depth) {
		case 0:
			// That's /
			break;
		case 1:
			// that's /home
			break;
		case 2:
			/// that's /home/$LOGNAME
			break;
		case 3:
			// that's /home/$LOGNAME/snap
			// NOTE: fall-through
		case 4:
			// that's /home/$LOGNAME/snap/$SNAP_NAME
			// NOTE: fall-through
		case 5:
			// that's /home/$LOGNAME/snap/$SNAP_NAME/$SNAP_REVISION
			*uid_p = uid;
			*gid_p = gid;
			*mode_p = 0755;
			break;
		default:
			die("unexpected SNAP_USER_DATA segment: %s at depth %d",
			    segment, depth);
		}
		return 0;
	}
	struct sc_mkpath_opts opts = {
		.mode = 0755,
		.uid = uid,
		.gid = gid,
		.hint_fn = hint_SNAP_USER_DATA,
		.do_chown = true,
	};
	debug("creating user data directory: %s", user_data);
	if (sc_nonfatal_mkpath(user_data, &opts) < 0) {
		die("cannot create user data directory: %s", user_data);
	};
}

void setup_user_xdg_runtime_dir(uid_t uid, gid_t gid)
{
	debug("considering creating $XDG_RUNTIME_DIR for the snap session");
	const char *xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
	debug("XDG_RUNTIME_DIR is %s", xdg_runtime_dir);
	if (xdg_runtime_dir == NULL) {
		debug("XDG_RUNTIME_DIR is not set");
		return;
	}
	// Only support absolute paths.
	if (xdg_runtime_dir[0] != '/') {
		die("XDG_RUNTIME_DIR must be an absolute path");
	}

	int hint_XDG_RUNTIME_DIR(int depth, const char *segment,
				 mode_t * mode_p, uid_t * uid_p,
				 gid_t * gid_p) {
		switch (depth) {
		case 0:
			// That's /
			break;
		case 1:
			// That's /run
			*uid_p = 0;
			*gid_p = 0;
			break;
		case 2:
			// That's /run/user
			break;
		case 3:
			// That's /run/user/user/$(id -u)
			*uid_p = uid;
			*gid_p = gid;
			*mode_p = 0700;
			break;
		case 4:
			// That's /run/user/$(id -u)/snap.$SNAP_NAME
			*uid_p = uid;
			*gid_p = gid;
			break;
		default:
			die("unexpected XDR_RUNTIME_DIR segment: %s at depth %d", segment, depth);
		}
		return 0;
	}
	struct sc_mkpath_opts opts = {
		.mode = 0755,
		.uid = uid,
		.gid = gid,
		.hint_fn = hint_XDG_RUNTIME_DIR,
		.do_chown = true,
	};
	debug("creating user XDG_RUNTIME_DIR directory: %s", xdg_runtime_dir);
	if (sc_nonfatal_mkpath(xdg_runtime_dir, &opts) < 0) {
		die("cannot create user XDG_RUNTIME_DIR directory: %s",
		    xdg_runtime_dir);
	}
}
