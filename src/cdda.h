/*                                                     -*- linux-c -*-
    Copyright (C) 2004 Tom Szilagyi

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    $Id$
*/

#ifndef _CDDA_H
#define _CDDA_H

#include <config.h>

#ifdef HAVE_CDDA

#include <gtk/gtk.h>

#ifdef HAVE_CDDB
#define _TMP_HAVE_CDDB 1
#undef HAVE_CDDB
#endif /* HAVE_CDDB */
#include <cdio/cdio.h>
#include <cdio/paranoia.h>
#ifdef HAVE_CDDB
#undef HAVE_CDDB
#endif /* HAVE_CDDB */
#ifdef _TMP_HAVE_CDDB
#define HAVE_CDDB 1
#undef _TMP_HAVE_CDDB
#endif /* _TMP_HAVE_CDDB */

#include "common.h"

#define CDDA_DRIVES_MAX 16
#define CDDA_MAXLEN 256

typedef struct {
	int n_tracks;
	lsn_t toc[100];
	char record_name[MAXLEN];
	char artist_name[MAXLEN];
} cdda_disc_t;

typedef struct {
	CdIo_t * cdio;
	int media_changed;
	int is_used; /* drive under use should not be scanned */
	char device_path[CDDA_MAXLEN];
	cdda_disc_t disc;
} cdda_drive_t;

void cdda_scanner_start(void);
void cdda_scanner_stop(void);

void cdda_timeout_start(void);
void cdda_timeout_stop(void);

void create_cdda_node(void);
void insert_cdda_drive_node(char * device_path);
void remove_cdda_drive_node(char * device_path);
void refresh_cdda_drive_node(char * device_path);

void cdda_drive_info(char * device_path);

cdda_drive_t * cdda_get_drive_by_spec_device_path(char * device_path);

#endif /* HAVE_CDDA */

#endif /* _CDDA_H */
