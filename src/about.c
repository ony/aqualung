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


#include <config.h>

#include <gtk/gtk.h>

#include "common.h"
#include "version.h"
#include "i18n.h"
#include "about.h"

#define MARK "+"


GtkWidget * about_window;
extern GtkWidget * main_window;
extern void set_sliders_width(void);

static gint
ok(GtkWidget * widget, gpointer data) {

	gtk_widget_destroy(about_window);
        set_sliders_width();
	return TRUE;
}


void
create_about_window() {

	GtkWidget * vbox0;
	GtkWidget * vbox;

	GtkWidget * xpm;
	GdkPixbuf * pixbuf;

	GtkWidget * frame;
	GtkWidget * scrolled_win;
	GtkWidget * view;
	GtkTextBuffer * buffer;
	GtkTextIter iter;

	GtkWidget * hbuttonbox;
	GtkWidget * ok_btn;

	GdkColor white = { 0, 49152, 51118, 52429 };
	GdkColor blue1 = { 0, 41288, 47841, 55050 };
	GdkColor blue2 = { 0, 45288, 51841, 60050 };
	GdkColor blue3 = { 0, 55552, 56832,  57600};

	GtkTextTag * tag;
	GtkTextTag * tag2;

	char path[MAXLEN];

	about_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_transient_for(GTK_WINDOW(about_window), GTK_WINDOW(main_window));
	gtk_window_set_modal(GTK_WINDOW(about_window), TRUE);
	gtk_widget_set_name(about_window, "");
        gtk_window_set_title(GTK_WINDOW(about_window), _("About"));
        gtk_widget_set_size_request(about_window, 483, 430);
	gtk_window_set_position(GTK_WINDOW(about_window), GTK_WIN_POS_CENTER);
	gtk_widget_modify_bg(about_window, GTK_STATE_NORMAL, &white);

        vbox0 = gtk_vbox_new(FALSE, 0);
        gtk_container_add(GTK_CONTAINER(about_window), vbox0);

        vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
	gtk_box_pack_end(GTK_BOX(vbox0), vbox, TRUE, TRUE, 0);

	hbuttonbox = gtk_hbutton_box_new();
	gtk_widget_set_name(hbuttonbox, "");
	gtk_box_pack_end(GTK_BOX(vbox), hbuttonbox, FALSE, TRUE, 0);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(hbuttonbox), GTK_BUTTONBOX_END);

        ok_btn = gtk_button_new_from_stock (GTK_STOCK_CLOSE); 
	gtk_widget_set_name(ok_btn, "");
        g_signal_connect(ok_btn, "clicked", G_CALLBACK(ok), NULL);
  	gtk_container_add(GTK_CONTAINER(hbuttonbox), ok_btn);   
	gtk_widget_modify_bg(ok_btn, GTK_STATE_NORMAL, &blue1);
	gtk_widget_modify_bg(ok_btn, GTK_STATE_PRELIGHT, &blue2);
	gtk_widget_modify_bg(ok_btn, GTK_STATE_ACTIVE, &blue2);

	frame = gtk_frame_new(NULL);
	gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);

	scrolled_win = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_name(scrolled_win, "");
	gtk_widget_set_name(GTK_SCROLLED_WINDOW(scrolled_win)->vscrollbar, "");
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win),
				       GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

	gtk_widget_modify_bg(GTK_SCROLLED_WINDOW(scrolled_win)->vscrollbar, GTK_STATE_NORMAL, &blue1);
	gtk_widget_modify_bg(GTK_SCROLLED_WINDOW(scrolled_win)->vscrollbar, GTK_STATE_PRELIGHT, &blue2);
	gtk_widget_modify_bg(GTK_SCROLLED_WINDOW(scrolled_win)->vscrollbar, GTK_STATE_ACTIVE, &blue3);


        view = gtk_text_view_new();
	gtk_widget_set_name(view, "");

	gtk_widget_modify_base(view, GTK_STATE_NORMAL, &blue3);
	gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view), 3);
	gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view), 3);
        gtk_text_view_set_editable(GTK_TEXT_VIEW(view), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), FALSE);
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
        gtk_text_view_set_buffer(GTK_TEXT_VIEW(view), buffer);

	tag = gtk_text_buffer_create_tag(buffer, NULL, "foreground", "#0000C0", NULL);
	tag2 = gtk_text_buffer_create_tag(buffer, NULL, "foreground", "#d9dee1", NULL);


	/* insert text */

	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, _("Build version: "), -1, tag, NULL);
	gtk_text_buffer_insert_at_cursor(buffer, aqualung_version, -1);

	gtk_text_buffer_insert_at_cursor(buffer, "\n\n", -1);
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, _("Homepage:"), -1, tag, NULL);
	gtk_text_buffer_insert_at_cursor(buffer, " http://aqualung.sf.net\n", -1);

	gtk_text_buffer_insert_at_cursor(buffer, "\nCopyright (C) 2004-2006 Tom Szilagyi\n\n\n", -1);

	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, _("Authors:"), -1, tag, NULL);
	gtk_text_buffer_insert_at_cursor(buffer, "\n\n", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Core design, engineering & programming:\n"), -1);
	gtk_text_buffer_insert_at_cursor(buffer, "\tTom Szilagyi <tszilagyi@users.sourceforge.net>\n\n", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Skin support, look & feel, GUI hacks:\n"), -1);
	gtk_text_buffer_insert_at_cursor(buffer, "\tPeter Szilagyi <peterszilagyi@users.sourceforge.net>\n\n", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Programming, GUI engineering:\n"), -1);
	gtk_text_buffer_insert_at_cursor(buffer, "\tTomasz Maka <pasp@users.sourceforge.net>\n\n\n", -1);

	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, _("Translators:"), -1, tag, NULL);
	gtk_text_buffer_insert_at_cursor(buffer, "\n\n", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("German, Hungarian:\n"), -1);
	gtk_text_buffer_insert_at_cursor(buffer, "\tPeter Szilagyi <peterszilagyi@users.sourceforge.net>\n\n", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Ukrainian:\n"), -1);
	gtk_text_buffer_insert_at_cursor(buffer, "\tSergiy Niskorodov <sgh_punk@users.sourceforge.net>\n\n\n", -1);

	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, _("Graphics:"), -1, tag, NULL);
	gtk_text_buffer_insert_at_cursor(buffer, "\n\n", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Logo:\n"), -1);
	gtk_text_buffer_insert_at_cursor(buffer, "\tMaja Kocon <ironya@ll.pl>\n\n\n", -1);


	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter,
					 _("This Aqualung binary is compiled with:"), -1, tag, NULL);

	gtk_text_buffer_insert_at_cursor(buffer, "\n\n\t", -1);

	gtk_text_buffer_insert_at_cursor(buffer, _("File format support:"), -1);
	gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);


	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_SNDFILE
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_SNDFILE */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("sndfile (WAV, AIFF, etc.)\n"), -1);

	
	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_FLAC
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_FLAC */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Free Lossless Audio Codec (FLAC)\n"), -1);
	

	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_OGG_VORBIS
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_OGG_VORBIX */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Ogg Vorbis\n"), -1);

	
	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_SPEEX
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_SPEEX */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Ogg Speex\n"), -1);
	

	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_MPEG
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_MPEG */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("MPEG Audio (MPEG 1-2.5 Layer I-III)\n"), -1);
	

	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_MOD
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_MOD */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
        gtk_text_buffer_insert_at_cursor(buffer, _("MOD Audio (MOD, S3M, XM, IT, etc.)\n"), -1);


	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_MPC
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_MPC */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
        gtk_text_buffer_insert_at_cursor(buffer, _("Musepack\n"), -1);


	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_MAC
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_MAC */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
        gtk_text_buffer_insert_at_cursor(buffer, _("Monkey's Audio Codec\n"), -1);

	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_ID3
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_ID3 */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
        gtk_text_buffer_insert_at_cursor(buffer, _("ID3 tags\n"), -1);


	gtk_text_buffer_insert_at_cursor(buffer, "\n\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Output driver support:"), -1);
	gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);
	

	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_OSS
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_OSS */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("OSS Audio\n"), -1);


	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_ALSA
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_ALSA */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("ALSA Audio\n"), -1);

	
	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[" MARK "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("JACK Audio Server\n"), -1);


	gtk_text_buffer_insert_at_cursor(buffer, "\n\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Miscellaneous:"), -1);
	gtk_text_buffer_insert_at_cursor(buffer, "\n", -1);


	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_SRC
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_SRC */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Internal Sample Rate Converter support\n"), -1);


	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_CDDB
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_CDDB */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("CDDB support\n"), -1);


	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_IFP
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_IFP */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("iRiver iFP driver support\n"), -1);
	

	gtk_text_buffer_insert_at_cursor(buffer, "\t\t[", -1);
#ifdef HAVE_SYSTRAY
	gtk_text_buffer_insert_at_cursor(buffer, MARK, -1);
#else
	gtk_text_buffer_get_end_iter(buffer, &iter);
	gtk_text_buffer_insert_with_tags(buffer, &iter, MARK, -1, tag2, NULL);
#endif /* HAVE_SYSTRAY */
	gtk_text_buffer_insert_at_cursor(buffer, "]\t", -1);
	gtk_text_buffer_insert_at_cursor(buffer, _("Systray support\n"), -1);
	

	gtk_text_buffer_insert_at_cursor(buffer, "\n\n", -1);
	gtk_text_buffer_insert_at_cursor(buffer,
		       _("This program is free software; you can redistribute it and/or modify\n\
it under the terms of the GNU General Public License as published by\n\
the Free Software Foundation; either version 2 of the License, or\n\
(at your option) any later version.\n\n\
This program is distributed in the hope that it will be useful,\n\
but WITHOUT ANY WARRANTY; without even the implied warranty of\n\
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n\
GNU General Public License for more details.\n\n\
You should have received a copy of the GNU General Public License\n\
along with this program; if not, write to the Free Software\n\
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA."), -1);


	gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
	gtk_text_buffer_place_cursor(buffer, &iter);

	gtk_box_pack_end(GTK_BOX(vbox), frame, TRUE, TRUE, 6);
	gtk_container_add(GTK_CONTAINER(frame), scrolled_win);
	gtk_container_add(GTK_CONTAINER(scrolled_win), view);

	sprintf(path, "%s/logo.png", DATADIR);
        if ((pixbuf = gdk_pixbuf_new_from_file(path, NULL)) != NULL) {
		xpm = gtk_image_new_from_pixbuf (pixbuf);
		gtk_box_pack_start(GTK_BOX(vbox0), xpm, FALSE, FALSE, 0);
		gtk_widget_show(xpm);
	}

	gtk_widget_show_all(about_window);
        set_sliders_width();    /* MAGIC */

        gtk_widget_grab_focus(ok_btn);
}

// vim: shiftwidth=8:tabstop=8:softtabstop=8 :  

