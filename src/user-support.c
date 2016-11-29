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

#include <errno.h>
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

	debug("creating user data directory: %s", user_data);
	if (sc_nonfatal_mkpath(user_data, 0755) < 0) {
		die("cannot create user data directory: %s", user_data);
	};
	// TODO: we should chown everything below $HOME
	if (lchown(user_data, uid, gid) != 0) {
		die("could not chown user data directory");
	}
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

	debug("creating user XDG_RUNTIME_DIR directory: %s", xdg_runtime_dir);
	errno = 0;
	if (sc_nonfatal_mkpath(xdg_runtime_dir, 0755) < 0) {
		die("cannot create user XDG_RUNTIME_DIR directory: %s",
		    xdg_runtime_dir);
	}
	// if the call above didn't do anything (EEXIST) then just return.
	if (errno == EEXIST) {
		debug("XDG_RUNTIME_DIR was created earlier");
		return;
	}
	// change mode and ownership of the created directory
	debug("changing mode of XDG_RUNTIME_DIR to 0700");
	if (chmod(xdg_runtime_dir, 0700) != 0) {
		die("cannot change mode of XDG_RUNTIME_DIR to 0700");
	}
	debug("changing ownership of XDG_RUNTIME_DIR to %d.%d", uid, gid);
	if (lchown(xdg_runtime_dir, uid, gid) != 0) {
		die("cannot change ownership of XDG_RUNTIME_DIR to %d.%d", uid,
		    gid);
	}
}
