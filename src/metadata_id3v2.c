/*                                                     -*- linux-c -*-
    Copyright (C) 2007 Tom Szilagyi

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

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <glib.h>

#include "common.h"
#include "utils.h"
#include "i18n.h"
#include "metadata.h"
#include "metadata_api.h"
#include "metadata_id3v2.h"


char *
meta_id3v2_apic_type_to_string(int type) {

	switch (type) {
	case 0x00: return _("Other");
	case 0x01: return _("File icon (32x32 PNG)");
	case 0x02: return _("File icon (other)");
	case 0x03: return _("Front cover");
	case 0x04: return _("Back cover");
	case 0x05: return _("Leaflet page");
	case 0x06: return _("Album image");
	case 0x07: return _("Lead artist/performer");
	case 0x08: return _("Artist/performer");
	case 0x09: return _("Conductor");
	case 0x0a: return _("Band/orchestra");
	case 0x0b: return _("Composer");
	case 0x0c: return _("Lyricist/text writer");
	case 0x0d: return _("Recording location/studio");
	case 0x0e: return _("During recording");
	case 0x0f: return _("During performance");
	case 0x10: return _("Movie/video screen capture");
	case 0x11: return _("A large, coloured fish");
	case 0x12: return _("Illustration");
	case 0x13: return _("Band/artist logotype");
	case 0x14: return _("Publisher/studio logotype");
	default: return NULL;
	}
}


/* strlen() for ID3v2 strings.
 *   enc={0,3} -> terminated by one null-byte
 *   enc={1,2} -> terminated by two null-bytes
 * buf is traversed to a maximum of buf[maxlen].
 */
int
meta_id3v2_strlen(unsigned char * buf, int maxlen, int enc) {

	int i;
	for (i = 0; i < maxlen; i++) {
		if (buf[i] == '\0') {
			if ((enc == 0) || (enc == 3)) {
				return i;
			}
			if ((enc == 1) || (enc == 2)) {
				if ((i > 0) && (buf[i-1] == '\0')) {
					return i;
				}
			}
		}
	}
	return maxlen;
}


u_int32_t
meta_id3v2_read_int(unsigned char * buf) {

	return ((((u_int32_t)buf[0]) << 24) |
		(((u_int32_t)buf[1]) << 16) |
		(((u_int32_t)buf[2]) << 8) |
		  (u_int32_t)buf[3]);
}


u_int32_t
meta_id3v2_read_synchsafe_int(unsigned char * buf) {

	return (((u_int32_t)(buf[0] & 0x7f) << 21) |
		((u_int32_t)(buf[1] & 0x7f) << 14) |
		((u_int32_t)(buf[2] & 0x7f) << 7) |
		 (u_int32_t)(buf[3] & 0x7f));
}


int
un_unsynch(unsigned char * buf, int len) {

	int i;
	for (i = 0; i < len-1; i++) {
		if ((buf[i] == 0xff) && (buf[i+1] == 0x00)) {
			memmove(buf+i+1, buf+i+2, len-i-2);
			--len;
		}
	}
	return len;
}


char *
meta_id3v2_to_utf8(unsigned char enc, unsigned char * buf, int len) {

	char * str = NULL;
	char * from;
	GError * error = NULL;

	switch (enc) {
	case 0x00: from = "iso-8859-1"; break;
	case 0x01: from = "ucs-2"; break;
	case 0x02: from = "utf-16be"; break;
	case 0x03: from = "utf-8"; break;
	default:
		fprintf(stderr, "meta_id3v2_to_utf8: invalid enc = %d\n", enc);
		return NULL;
	}

	str = g_convert_with_fallback((char *)buf, len, "utf-8", from,
				      "?", NULL, NULL, &error);
	if (str != NULL) {
		return str;
	} else {
		fprintf(stderr, "meta_id3v2_to_utf8: error converting '%s': %s\n",
			buf, error->message);
		g_clear_error(&error);
		return NULL;
	}
}


void
meta_parse_id3v2_txxx(metadata_t * meta, unsigned char * buf, int len) {

	int type;
	int len1;
	char * val1;
	char * val2;

	type = meta_frame_type_from_embedded_name(META_TAG_ID3v2, "TXXX");
	len1 = meta_id3v2_strlen(buf+11, len-1, buf[10]);
	val1 = meta_id3v2_to_utf8(buf[10], buf+11, len1);
	val2 = meta_id3v2_to_utf8(buf[10], buf+12+len1, len-len1-2);

	if ((val1 != NULL) && (val2 != NULL)) {
		metadata_add_frame_from_keyval(meta, META_TAG_ID3v2, val1, val2);
	}
	if (val1 != NULL) {
		g_free(val1);
	}
	if (val2 != NULL) {
		g_free(val2);
	}
}


void
meta_parse_id3v2_t___(metadata_t * meta, unsigned char * buf, int len) {

	char frame_id[5];
	int type;
	char * val;

	memcpy(frame_id, buf, 4);
	frame_id[4] = '\0';

	/* support the reading of some ID3v2.3 fields by mapping them to
	   the respective ID3v2.4 frame */
	if ((strcmp(frame_id, "TDAT") == 0) ||
	    (strcmp(frame_id, "TIME") == 0) ||
	    (strcmp(frame_id, "TRDA") == 0) ||
	    (strcmp(frame_id, "TYER") == 0)) {
		strcpy(frame_id, "TDRC");
	} else if (strcmp(frame_id, "TORY") == 0) {
		strcpy(frame_id, "TDOR");
	}

	type = meta_frame_type_from_embedded_name(META_TAG_ID3v2, frame_id);
	val = meta_id3v2_to_utf8(buf[10], buf+11, len-1);
	if (val != NULL) {
		metadata_add_frame_from_keyval(meta, META_TAG_ID3v2, frame_id, val);
		g_free(val);
	}
}


void
meta_parse_id3v2_comm(metadata_t * meta, unsigned char * buf, int len) {

	char enc = buf[10];
	char lang[4];
	int len1;
	char * descr;
	char * comment = NULL;

	memcpy(lang, buf+11, 3);
	lang[3] = '\0';
	len1 = meta_id3v2_strlen(buf+14, len-4, enc);
	if (len1 > len - 4) {
		len1 = len-4;
		fprintf(stderr, "warning: COMM description field too large, truncating\n");
	}
	descr = meta_id3v2_to_utf8(enc, buf+14, len1);

	if (len - len1 - 4 > 0) {
		comment = meta_id3v2_to_utf8(enc, buf+15+len1, len-len1-5);
	}

	if (descr != NULL) {
		g_free(descr);
	}
	if (comment != NULL) {
		metadata_add_frame_from_keyval(meta, META_TAG_ID3v2, "COMM", comment);
		g_free(comment);
	}
}


void
meta_parse_id3v2_apic(metadata_t * meta, unsigned char * buf, int len) {

	char enc = buf[10];
	char * mime_type;
	char * descr;
	char pic_type;
	int len1;
	int len2;

	len1 = meta_id3v2_strlen(buf+11, len-1, enc);
	if (len1 > len-11) {
		len1 = len-11;
		fprintf(stderr, "warning: APIC mime-type field too large, truncating\n");
	}

	mime_type = meta_id3v2_to_utf8(0x0/*ascii*/, buf+11, len1);
	pic_type = buf[12+len1];
	len2 = meta_id3v2_strlen(buf+13+len1, len-13-len1, enc);
	descr = meta_id3v2_to_utf8(enc, buf+13+len1, len-len1-13);

	if ((mime_type != NULL) && (descr != NULL)) {
		meta_frame_t * frame = meta_frame_new();
		frame->tag = META_TAG_ID3v2;
		frame->type = META_FIELD_APIC;
		frame->field_name = strdup(mime_type);
		frame->field_val = strdup(descr);
		frame->int_val = pic_type;
		frame->length = len - (4+len1+len2);
		frame->data = malloc(frame->length);
		if (frame->data == NULL) {
			fprintf(stderr, "meta_parse_id3v2_apic: malloc error\n");
			return;
		}
		memcpy(frame->data, buf+14+len1+len2, frame->length);
		metadata_add_frame(meta, frame);
	}

	if (mime_type != NULL) {
		g_free(mime_type);
	}
	if (descr != NULL) {
		g_free(descr);
	}
}


int
meta_parse_id3v2_frame(metadata_t * meta, unsigned char * buf, int len,
		       int version, int unsynch_all) {

	char frame_id[5];
	int frame_size;
	int pay_len;

	/* detect padding/footer, consume rest of payload */
	if ((buf[0] == '\0') ||
	    ((buf[0] == '3') && (buf[1] == 'D') && (buf[2] == 'I'))) {
		return len;
	}

	if (len < 10) {
		return len;
	}

	memcpy(frame_id, buf, 4);
	frame_id[4] = '\0';
	if (version == 0x03) {
		frame_size = pay_len = meta_id3v2_read_int(buf+4);
	} else if (version == 0x04) {
		frame_size = pay_len = meta_id3v2_read_synchsafe_int(buf+4);
		if (unsynch_all || (buf[9] & 0x02)) { /* unsynch-ed frame */
			pay_len = un_unsynch(buf+10, frame_size);
		}
	}
	/* XXX */printf("frame_id = %s, size = %d\n", frame_id, frame_size);

	if (frame_id[0] == 'T') {
		if ((frame_id[1] == 'X') &&
		    (frame_id[2] == 'X') &&
		    (frame_id[3] == 'X')) {
			meta_parse_id3v2_txxx(meta, buf, pay_len);
		} else {
			meta_parse_id3v2_t___(meta, buf, pay_len);
		}
	} else if (strcmp(frame_id, "COMM") == 0) {
		meta_parse_id3v2_comm(meta, buf, pay_len);
	} else if (strcmp(frame_id, "APIC") == 0) {
		meta_parse_id3v2_apic(meta, buf, pay_len);
	} else {
		/* TODO some mechanism to preserve data of unrecognized
		   frames between load/save */
	}

	return frame_size+10;
}


int
metadata_from_id3v2(metadata_t * meta, unsigned char * buf, int length) {

	int pos = 10;
	int payload_length;

	if ((buf[3] != 0x3) && (buf[3] != 0x4)) {
		/* ID3v2 version not 2.3 or 2.4, not supported */
		return 0;
	}

	/* In ID3v2.3 first unsynch the whole tag (after header),
	   then skip extended header. In ID3v2.4 frames are
	   individually unsynch-ed. */
	if (buf[3] == 0x03) {
		if (buf[5] & 0x80) {
			payload_length = un_unsynch(buf+pos, length-pos);
		} else {
			payload_length = length - pos;
		}
		if (buf[5] & 0x40) {
			int ext_len = meta_id3v2_read_int(buf+10) + 4;
			pos += ext_len;
			payload_length -= ext_len;
		}
	} else if (buf[3] == 0x04) {
		payload_length = length - pos;
		if (buf[5] & 0x40) {
			int ext_len = meta_id3v2_read_synchsafe_int(buf+10);
			pos += ext_len;
			payload_length -= ext_len;
		}
	}

	while (length > pos) {
		pos += meta_parse_id3v2_frame(meta, buf+pos, payload_length,
					      buf[3], buf[5] & 0x80);
	}

	return 1;
}


int
metadata_to_id3v2(metadata_t * meta, unsigned char * buf) {


	return 0;
}
