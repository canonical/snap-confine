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

#ifndef SNAP_CONFINE_MOUNT_SUPPORT_NVIDIA_H
#define SNAP_CONFINE_MOUNT_SUPPORT_NVIDIA_H

// Bind mount the binary nvidia driver into /var/lib/snapd/lib/gl.
// The directory is mounted relative to the given rootfs_dir.
void sc_bind_mount_nvidia_driver(const char *rootfs_dir);

#endif