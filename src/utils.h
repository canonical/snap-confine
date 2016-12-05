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
#include <stdlib.h>
#include <stdbool.h>

#ifndef CORE_LAUNCHER_UTILS_H
#define CORE_LAUNCHER_UTILS_H

__attribute__ ((noreturn))
    __attribute__ ((format(printf, 1, 2)))
void die(const char *fmt, ...);

__attribute__ ((format(printf, 1, 2)))
bool error(const char *fmt, ...);

__attribute__ ((format(printf, 1, 2)))
void debug(const char *fmt, ...);

void write_string_to_file(const char *filepath, const char *buf);

// snprintf version that dies on any error condition
__attribute__ ((format(printf, 3, 4)))
int must_snprintf(char *str, size_t size, const char *format, ...);

/**
 * Hint callback for sc_nonfatal_mkpath().
 *
 * The hint function is called for each subsequent segment of the path being
 * created.  The depth increments from 0 (representing / that is never
 * created), starting at 1 for the first directory that is being inspected or
 * created.
 *
 * At each depth the callback can modify directory mode, user and group
 * ownership of that path segment.
 *
 * The callback should return 0 on success. Non-zero return value causes
 * sc_nonfatal_mkpath() to fail and return (the actual code is propagated).
 **/
typedef int (*sc_mkpath_hint_fn) (int depth,
				  const char *path,
				  mode_t * mode_p, uid_t * uid_p,
				  gid_t * gid_p);

/**
 * Options structure for sc_nonfatal_mkpath().
 *
 * The mode describes permissions of all the directory segments.
 *
 * The uid and gid are user and group owner.
 *
 * The hint_fn callback can be safely omitted but if provided gets a chance to
 * modify mode, uid and gid of for each segment.
 *
 * The do_chown flag indicates that each directory segment should be fchownat()
 * to change the user and group owner as desired.
 **/
struct sc_mkpath_opts {
	mode_t mode;
	uid_t uid;
	gid_t gid;
	sc_mkpath_hint_fn hint_fn;
	bool do_chown;
};

/**
 * Safely create a given directory.
 *
 * NOTE: non-fatal functions don't die on errors. It is the responsibility of
 * the caller to call die() or handle the error appropriately.
 *
 * This function behaves like "mkdir -p" (recursive mkdir) with the exception
 * that each directory is carefully created in a way that avoids symlink
 * attacks. The preceding directory is kept openat(2) (along with O_DIRECTORY)
 * and the next directory is created using mkdirat(2), this sequence continues
 * while there are more directories to process.
 *
 * The mode and owner of each created directory can be controlled through the
 * options structure. It encodes the default values as well as allows the
 * caller to make fine-grained choices for each segment of the path, if
 * desired.
 *
 * The function returns a non-zero value in case of any error. If the error
 * originates from the hint callback then it is returned instead.
 **/
__attribute__ ((warn_unused_result))
int sc_nonfatal_mkpath(const char *const path,
		       const struct sc_mkpath_opts *opts);
#endif
