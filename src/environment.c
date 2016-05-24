/*
 * Copyright (C) 2016 Canonical Ltd
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
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char *env_profile_dir = "/var/lib/snapd/environment/";

void setenv_from_line(char *line)
{
	const char *name = NULL;
	const char *value = NULL;
	debug("reading line: '%s'", line);

	if (strlen(line) == 0)
		return;

	// kill final \n
	char *nl = strchr(line, '\n');
	if (nl != NULL)
		*nl = 0;

	// find first "="
	char *t = strchr(line, '=');
	if (t == NULL)
		return;

	// before the "=" is the env key
	*t = 0;
	name = line;

	// after the "=" is the env value
	// note that there is always a t+1 because fgets() gives us a final \0
	if (*(t + 1) == 0)
		return;
	value = t + 1;

	debug("setenv: '%s'='%s'", name, value);
	if (setenv(name, value, 0) != 0)
		die("setenv failed");
}

void apply_environment_file(const char *env_tag)
{
	debug("apply_environment_file %s", env_tag);

	if (secure_getenv("SNAPPY_LAUNCHER_ENVIRONMENT_DIR") != NULL)
		env_profile_dir =
		    secure_getenv("SNAPPY_LAUNCHER_ENVIRONMENT_DIR");

	char env_path[512];	// ought to be enough for everybody
	must_snprintf(env_path, sizeof(env_path), "%s/%s", env_profile_dir,
		      env_tag);

	char buf[1024];		// LD_LIBRARY_PATH can get long
	debug("reading '%s'", env_path);
	FILE *f = fopen(env_path, "r");
	if (f == NULL) {
		return;
	}
	while (fgets(buf, sizeof(buf), f) != NULL) {
		setenv_from_line(buf);
	}
	fclose(f);
}
