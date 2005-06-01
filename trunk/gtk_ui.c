/*
  Copyright (c) 2003, 2004, 2005 Miguel Mendez <flynn@energyhq.es.eu.org>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer. 
  * Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution. 

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  $Id$

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <unistd.h>
#include <pwd.h>
#include <libgen.h>
#include <time.h>
extern char *tzname[2];

#include <glib.h>
#include <gtk/gtk.h>

#include "gtk-send-pr.h"
#include "gtk_ui.h"
#include "smtp.h"
#include "pr_defs.h"
#include "user_settings.h"
#include "file.h"
#include "uname_gather.h"

#include "ladybird16.xpm"
#include "ladybird32.xpm"
#include "ladybird48.xpm"
#include "ladybird64.xpm"

extern char global_smtp_error_msg[1024];

static gint delete_event( GtkWidget *, GdkEvent *, gpointer);
static void destroy( GtkWidget *, gpointer);
static void quit_pressed(GtkWidget *, gpointer);
static void about_pressed(GtkWidget *, gpointer);
static void help_pressed(GtkWidget *, gpointer);
static void send_pressed(GtkWidget *, gpointer);
static void open_pressed(GtkWidget *, gpointer);
static void clear_fix_pressed(GtkWidget *, gpointer);
static void auth_ok_pressed(GtkWidget *, gpointer);
static void fill_pr(PROBLEM_REPORT *);
static void update_profile(void);
static void fix_view_drag_data_received(GtkWidget *, GdkDragContext *, 
					gint, gint, GtkSelectionData *, 
					guint, guint);

static int dirty;
static GtkWidget *window;

static USER_PROFILE my_profile;
static GtkWidget *type_entry1;

static GtkWidget *email_entry1;
static GtkWidget *email_entry2;
static GtkWidget *email_entry4;
static GtkWidget *email_entry5;
static GtkWidget *email_entry6;
static GtkWidget *email_entry7;
static GtkWidget *email_entry8;
static GtkWidget *email_port_entry;
static GtkWidget *email_ssl_option;

#define SSL_NO "Don't use SSL"
#define SSL_EN "Use SSL if possible"
#define SSL_RE "Always Use SSL"

static GtkWidget *type_combo1;
static GtkWidget *type_combo2;
static GtkWidget *type_combo3;
static GtkWidget *type_combo4;
static GtkWidget *system_entry1;
static GtkWidget *system_view1;
static GtkWidget *details_view1;
static GtkWidget *details_view2;
static GtkWidget *fix_view;

static GtkTextBuffer *fix_buffer1;
static GtkWidget *send_button;

static char *env_buffer;
static char *desc_buffer;
static char *how_buffer;
static char *fix_buffer;

/* XXX drag and drop signals are received twice!?!? */
static char drag_dupe[1024];

static GtkWidget *auth_window;
static GtkWidget *auth_vbox;
static GtkWidget *auth_label;
static GtkWidget *auth_userframe;
static GtkWidget *auth_passframe;
static GtkWidget *auth_ok;

static GtkWidget *auth_userentry;
static GtkWidget *auth_passentry;
static int open_menu_up;

static GdkPixbuf *icon64_pixbuf;

int gsp_auth_done;
static GSP_AUTH *auth_info;

int
create_gtk_ui(char *included_file, int maint_mode)
{

  GtkWidget *about_icon;
  GtkWidget *about_label;
  GtkWidget *about_hbox;
  GtkWidget *about_align;
  GtkWidget *send_icon;
  GtkWidget *send_label;
  GtkWidget *send_hbox;
  GtkWidget *send_align;
  GtkWidget *quit_button;
  GtkWidget *about_button;
  GtkWidget *help_button;
  GtkWidget *h_buttons;
  GtkWidget *vbox1;
  GtkWidget *hseparator1;

  GtkWidget *mynotebook;
  GtkWidget *tab_email;
  GtkWidget *tab_type;
  GtkWidget *tab_system;
  GtkWidget *tab_descr;
  GtkWidget *tab_fix;

  GtkTooltips *send_tip;
  GtkTooltips *quit_tip;
  GtkTooltips *about_tip;
  GtkTooltips *help_tip;

  GtkWidget     *email_vbox;
  GtkWidget	*email_frame1;
  GtkWidget     *email_hbox1;

  GtkWidget	*email_frame2;
  GtkWidget	*email_frame3;
  GtkWidget	*email_entry3;
  GtkWidget	*email_frame4;
  GtkWidget	*email_frame5;
  GtkWidget	*email_frame6;
  GtkWidget	*email_frame7;
  GtkWidget	*email_frame8;

  GtkWidget	*type_vbox;
  GtkWidget     *type_vbox1;
  GtkWidget     *type_vbox2;
  GtkWidget     *type_vbox3;
  GtkWidget     *type_vbox4;

  GtkWidget	*type_frame1;
  GtkWidget	*type_frame2;
  GtkWidget	*type_frame3;
  GtkWidget	*type_frame4;
  GtkWidget	*type_frame5;

  GtkWidget	*system_vbox;
  GtkWidget	*system_frame1;		
  GtkWidget	*system_frame2;

  GtkTextBuffer *system_buffer1;
  GtkWidget	*scrolled_window1;

  GtkWidget	*details_vbox;
  GtkWidget	*details_frame1;
  GtkWidget	*details_frame2;

  GtkWidget	*scrolled_window2;
  GtkWidget	*scrolled_window3;
  GtkWidget	*scrolled_window4;				

  GtkWidget *fix_vbox;
  GtkWidget *fix_hbuttons;
  GtkWidget *fix_frame;
  GtkWidget *fix_button1;
  GtkWidget *fix_button2;
  GtkTooltips *fix_tip1;
  GtkTooltips *fix_tip2;

  /* For the window icon */
  GdkPixbuf *icon16_pixbuf;
  GdkPixbuf *icon32_pixbuf;
  GdkPixbuf *icon48_pixbuf;

  GList *icon_list = NULL;

  char uname_srm[256];
  char uname_snrvm[1024]; /* better safe than sorry */
  int i;
  char *fix_buffer = NULL; /* Buffer for the -a option */
  GtkWidget *file_dialog;
  char file_warning[1024];
  int load_failed = FALSE;

  /* Drag and drop support */
  enum
  {
    TARGET_URI_LIST
  };

  static GtkTargetEntry target_table[] = {

    { "text/uri-list", 0, TARGET_URI_LIST },

  };

  static guint n_targets = sizeof(target_table) / sizeof(target_table[0]);

  open_menu_up = 0;
  gsp_auth_done = FALSE;

  /* Let's go */

  memset(drag_dupe, 0, 1024);

  load_settings(&my_profile);
  uname_gather(uname_srm, uname_snrvm);

  dirty = 1;
  /* Define main window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
  gtk_window_set_title(GTK_WINDOW(window), "gtk-send-pr " GSP_VERSION);
  gtk_window_set_default_size(GTK_WINDOW(window), my_profile.geom_x, my_profile.geom_y);

  /* Set the icon */
  icon16_pixbuf = gdk_pixbuf_new_from_xpm_data((const char **)ladybird16);
  icon32_pixbuf = gdk_pixbuf_new_from_xpm_data((const char **)ladybird32);
  icon48_pixbuf = gdk_pixbuf_new_from_xpm_data((const char **)ladybird48);
  icon64_pixbuf = gdk_pixbuf_new_from_xpm_data((const char **)ladybird64);

  g_list_append(icon_list, icon16_pixbuf);
  g_list_append(icon_list, icon32_pixbuf);
  g_list_append(icon_list, icon48_pixbuf);	
  g_list_append(icon_list, icon64_pixbuf);

  gtk_window_set_icon(GTK_WINDOW(window), icon64_pixbuf);
  gtk_window_set_default_icon_list(icon_list);

  g_list_free (icon_list);

  /* Basic buttons */
  send_button = gtk_button_new();
  send_icon = gtk_image_new_from_stock(GTK_STOCK_APPLY, GTK_ICON_SIZE_MENU);
  send_label = gtk_label_new_with_mnemonic("_Send");
  send_hbox = gtk_hbox_new(FALSE, 2);
  send_align = gtk_alignment_new(0.5, 0.5, 0.0, 0.0);
  gtk_box_pack_start(GTK_BOX(send_hbox), send_icon, FALSE, FALSE, 0);
  gtk_box_pack_end(GTK_BOX(send_hbox), send_label, FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(send_button), send_align);
  gtk_container_add(GTK_CONTAINER(send_align), send_hbox);

  quit_button = gtk_button_new_from_stock(GTK_STOCK_QUIT);

  about_button = gtk_button_new();

#if(GTK_MINOR_VERSION>=6)
  about_icon = gtk_image_new_from_stock(GTK_STOCK_ABOUT, GTK_ICON_SIZE_MENU);
#else
  about_icon = gtk_image_new_from_stock(GTK_STOCK_PROPERTIES, GTK_ICON_SIZE_MENU);
#endif


  about_label = gtk_label_new_with_mnemonic("_About");
  about_hbox = gtk_hbox_new(FALSE, 2);
  about_align = gtk_alignment_new(0.5, 0.5, 0.0, 0.0);
  gtk_box_pack_start(GTK_BOX(about_hbox), about_icon, FALSE, FALSE, 0);
  gtk_box_pack_end(GTK_BOX(about_hbox), about_label, FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(about_button), about_align);
  gtk_container_add(GTK_CONTAINER(about_align), about_hbox);

  help_button = gtk_button_new_from_stock(GTK_STOCK_HELP);

  /* Button tooltips */
  send_tip = gtk_tooltips_new();
  gtk_tooltips_set_tip(send_tip,send_button, "Send problem report", "");
  gtk_tooltips_enable(send_tip);

  help_tip = gtk_tooltips_new();
  gtk_tooltips_set_tip(help_tip,help_button, "Show online help", "");
  gtk_tooltips_enable(help_tip);

  about_tip = gtk_tooltips_new();
  gtk_tooltips_set_tip(about_tip,about_button, "About this program", "");
  gtk_tooltips_enable(about_tip);

  quit_tip = gtk_tooltips_new();
  gtk_tooltips_set_tip(quit_tip,quit_button, "Exit this program", "");
  gtk_tooltips_enable(quit_tip);

  h_buttons = gtk_hbutton_box_new();

  gtk_box_pack_start(GTK_BOX(h_buttons), send_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(h_buttons), about_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(h_buttons), help_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(h_buttons), quit_button, FALSE, FALSE, 0);

  g_signal_connect(GTK_OBJECT(window), "delete_event",
		   G_CALLBACK(delete_event), NULL);

  g_signal_connect(GTK_OBJECT(window), "destroy",
		   G_CALLBACK(destroy), NULL);

  g_signal_connect(GTK_OBJECT(quit_button), "clicked",
		   G_CALLBACK(quit_pressed), NULL);

  g_signal_connect(GTK_OBJECT(about_button), "clicked",
		   G_CALLBACK(about_pressed), NULL);

  g_signal_connect(GTK_OBJECT(send_button), "clicked",
		   G_CALLBACK(send_pressed), NULL);

  g_signal_connect(GTK_OBJECT(help_button), "clicked",
		   G_CALLBACK(help_pressed), NULL);				

  vbox1 = gtk_vbox_new(FALSE, 2);
  hseparator1 = gtk_hseparator_new ();

  tab_email = gtk_label_new("E-Mail");
  tab_type = gtk_label_new("Type");
  tab_system = gtk_label_new("System");
  tab_descr = gtk_label_new("Details");
  tab_fix = gtk_label_new("Fix");

  /* E-mail stuff */
  email_vbox = gtk_vbox_new(FALSE, 2);

  email_frame1 = gtk_frame_new(" To ");
  email_entry1 = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry1), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry1), default_to);

  email_frame2 = gtk_frame_new(" From ");
  email_entry2 = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry2), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry2), my_profile.email);

  email_frame3 = gtk_frame_new(" Reply to ");
  email_entry3 = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry3), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry3), my_profile.email);

  email_frame4 = gtk_frame_new(" CC ");
  email_entry4 = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry4), 255);	

  email_frame5 = gtk_frame_new(" Submitter-Id ");
  email_entry5 = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry5), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry5), default_sub_id);

  email_frame6 = gtk_frame_new(" Originator ");
  email_entry6 = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry6), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry6), my_profile.name);

  email_frame7 = gtk_frame_new(" Organization ");
  email_entry7 = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry7), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry7), my_profile.org);

  email_frame8 = gtk_frame_new(" SMTP Server and Port ");

  email_entry8 = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry8), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry8), my_profile.smtp);

  email_port_entry = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_port_entry), 16);
  gtk_entry_set_text(GTK_ENTRY(email_port_entry), my_profile.smtp_port);

  email_ssl_option = gtk_combo_box_new_text();
  gtk_combo_box_append_text(GTK_COMBO_BOX(email_ssl_option), (const gchar *) SSL_NO);
  gtk_combo_box_append_text(GTK_COMBO_BOX(email_ssl_option), (const gchar *) SSL_EN);
  gtk_combo_box_append_text(GTK_COMBO_BOX(email_ssl_option), (const gchar *) SSL_RE);
  gtk_combo_box_set_active(GTK_COMBO_BOX(email_ssl_option), my_profile.ssl_mode);
  
  email_hbox1 = gtk_hbox_new(FALSE, 4);
  gtk_box_pack_start(GTK_BOX(email_hbox1), email_entry8, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(email_hbox1), email_port_entry, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX(email_hbox1), email_ssl_option, FALSE, FALSE, 4);
  
  gtk_container_add(GTK_CONTAINER(email_frame1), email_entry1);
  gtk_container_add(GTK_CONTAINER(email_frame2), email_entry2);
  gtk_container_add(GTK_CONTAINER(email_frame3), email_entry3);
  gtk_container_add(GTK_CONTAINER(email_frame4), email_entry4);
  gtk_container_add(GTK_CONTAINER(email_frame5), email_entry5);
  gtk_container_add(GTK_CONTAINER(email_frame6), email_entry6);
  gtk_container_add(GTK_CONTAINER(email_frame7), email_entry7);

  gtk_container_add(GTK_CONTAINER(email_frame8), email_hbox1);

  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame1, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame2, TRUE, TRUE, 4);	
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame3, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame4, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame5, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame6, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame7, TRUE, TRUE, 4);			
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame8, FALSE, FALSE, 4);	

  /* PR type code */
  type_vbox = gtk_vbox_new(FALSE, 2);

  type_frame1 = gtk_frame_new(" Synopsis ");
  type_entry1 = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(type_entry1), 255);

  /* Maintainer mode */
  if (maint_mode == MAINT_YES) {

    gtk_entry_set_text(GTK_ENTRY(type_entry1), "[Maintainer Update] ");

  }

  /* Severity */
  type_frame2 = gtk_frame_new(" Severity ");
  type_vbox1 = gtk_vbox_new(TRUE, 2);
  type_combo1 = gtk_combo_box_entry_new_text();

  for (i = 0; i < (sizeof(pr_severities)/sizeof(char *)); i++) {

    gtk_combo_box_append_text(GTK_COMBO_BOX(type_combo1),
			      pr_severities[i]);

  }

  gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo1), 0);

  gtk_box_pack_start(GTK_BOX(type_vbox1), type_combo1, FALSE, FALSE, 0);

  gtk_container_add(GTK_CONTAINER(type_frame1), type_entry1);
  gtk_container_add(GTK_CONTAINER(type_frame2), type_vbox1);

  gtk_box_pack_start(GTK_BOX(type_vbox), type_frame1, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(type_vbox), type_frame2, TRUE, TRUE, 4);

  /* Priority */
  type_frame3 = gtk_frame_new(" Priority ");
  type_vbox2 = gtk_vbox_new(TRUE, 2);
  type_combo2 = gtk_combo_box_entry_new_text();

  for (i = 0; i < (sizeof(pr_priorities)/sizeof(char *)); i++) {

    gtk_combo_box_append_text(GTK_COMBO_BOX(type_combo2),
			      pr_priorities[i]);

  }

  gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo2), 0);
  gtk_box_pack_start(GTK_BOX(type_vbox2), type_combo2, FALSE, FALSE, 0);

  gtk_container_add(GTK_CONTAINER(type_frame3), type_vbox2);
  gtk_box_pack_start(GTK_BOX(type_vbox), type_frame3, TRUE, TRUE, 4);

  /* Category */
  type_frame4 = gtk_frame_new(" Category ");
  type_vbox3 = gtk_vbox_new(TRUE, 2);
  type_combo3 = gtk_combo_box_entry_new_text();

  for (i = 0; i < (sizeof(pr_categories)/sizeof(char *));i++) {

    gtk_combo_box_append_text(GTK_COMBO_BOX(type_combo3),
			      pr_categories[i]);

  }

  if (maint_mode == MAINT_YES) {

      for (i = 0; i < (sizeof(pr_categories)/sizeof(char *));i++) {

	if (strncmp(pr_categories[i], MAINT_CAT, 255) == 0 ) {

	  gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo3), i);
	  break;

	}

      }

  } else {

    gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo3), 0);

  }

  gtk_box_pack_start(GTK_BOX(type_vbox3), type_combo3, FALSE, FALSE, 0);

  gtk_container_add(GTK_CONTAINER(type_frame4), type_vbox3);
  gtk_box_pack_start(GTK_BOX(type_vbox), type_frame4, TRUE, TRUE, 4);

  /* Class */
  type_frame5 = gtk_frame_new(" Class ");
  type_vbox4 = gtk_vbox_new(TRUE, 2);
  type_combo4 = gtk_combo_box_entry_new_text();

  for ( i = 0; i < (sizeof(pr_classes)/sizeof(char *)); i++) {

    gtk_combo_box_append_text(GTK_COMBO_BOX(type_combo4),
			      pr_classes[i]);

  }

  if (maint_mode == MAINT_YES) {

    for ( i = 0; i < (sizeof(pr_classes)/sizeof(char *)); i++) {

	if(strncmp(pr_classes[i], MAINT_CLASS, 255) == 0 ) {

	  gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo4), i);
	  break;

	}

    }

  } else {

    gtk_combo_box_set_active(GTK_COMBO_BOX(type_combo4), 0);

  }

  gtk_box_pack_start(GTK_BOX(type_vbox4), type_combo4, FALSE, FALSE, 0);

  gtk_container_add(GTK_CONTAINER(type_frame5), type_vbox4);
  gtk_box_pack_start(GTK_BOX(type_vbox), type_frame5, TRUE, TRUE, 4);

  /* System info */
  system_vbox = gtk_vbox_new(FALSE, 2);	

  system_frame1 = gtk_frame_new(" Release ");	

  system_entry1 = gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(system_entry1), 255);
  gtk_entry_set_text(GTK_ENTRY(system_entry1), uname_srm);

  /* Environment */

  system_frame2 = gtk_frame_new(" Environment ");

  system_view1 = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(system_view1), GTK_WRAP_CHAR);
  system_buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(system_view1));
  gtk_text_buffer_set_text(system_buffer1, uname_snrvm, -1);

  scrolled_window1 = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window1), 10);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window1),
				 GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  gtk_container_add(GTK_CONTAINER(scrolled_window1)
		    , system_view1);

  gtk_container_add(GTK_CONTAINER(system_frame1), system_entry1);
  gtk_container_add(GTK_CONTAINER(system_frame2), scrolled_window1);

  gtk_box_pack_start(GTK_BOX(system_vbox), system_frame1, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX(system_vbox), system_frame2, TRUE, TRUE, 4);

  /* PR details */
  details_vbox = gtk_vbox_new (FALSE, 2);	
  details_frame1 = gtk_frame_new(" Description ");

  details_view1 = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(details_view1), GTK_WRAP_CHAR);

  scrolled_window2 = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window2), 10);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window2),
				 GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  gtk_container_add(GTK_CONTAINER(scrolled_window2)
		    , details_view1);

  details_frame2 = gtk_frame_new(" How-To-Repeat ");

  details_view2 = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(details_view2), GTK_WRAP_CHAR);

  scrolled_window3 = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window3), 10);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window3),
				 GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  gtk_container_add(GTK_CONTAINER(scrolled_window3)
		    , details_view2);

  gtk_container_add(GTK_CONTAINER(details_frame1), scrolled_window2);
  gtk_container_add(GTK_CONTAINER(details_frame2), scrolled_window3);

  gtk_box_pack_start(GTK_BOX(details_vbox), details_frame1, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(details_vbox), details_frame2, TRUE, TRUE, 4);

  /* Fix */
  fix_vbox = gtk_vbox_new(FALSE, 2);
  fix_frame = gtk_frame_new(" Fix (You can drag and drop text files here)");

  fix_view = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(fix_view), GTK_WRAP_CHAR);

  /* Drag and drop support */
  gtk_drag_dest_set(fix_view,
		    GTK_DEST_DEFAULT_ALL,
		    target_table, n_targets,
		    GDK_ACTION_COPY | GDK_ACTION_MOVE);
  /* XXX - Why do we need GDK_ACTION_MOVE for dnd to work with Konqueror? */

 g_signal_connect(fix_view, "drag_data_received",
		  G_CALLBACK(fix_view_drag_data_received), NULL);

  fix_buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(fix_view));

  if (included_file != NULL) {

    fix_buffer = load_file(included_file);

    if (fix_buffer != NULL) {

      gtk_text_buffer_set_text(fix_buffer1, fix_buffer, -1);
      free(fix_buffer);
      load_failed = FALSE;

    } else {

      load_failed = TRUE;

    }

  }

  scrolled_window4 = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window4), 10);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window4),
				 GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  gtk_container_add(GTK_CONTAINER(scrolled_window4)
		    , fix_view);

  gtk_container_add(GTK_CONTAINER(fix_frame), scrolled_window4);

  fix_hbuttons = gtk_hbutton_box_new();

  fix_button1 = gtk_button_new_from_stock(GTK_STOCK_OPEN);
  fix_tip1=gtk_tooltips_new();
  gtk_tooltips_set_tip(fix_tip1,fix_button1,"Insert a file at cursor location", "");
  gtk_tooltips_enable(fix_tip1);

  fix_button2 = gtk_button_new_from_stock(GTK_STOCK_CLEAR);
  fix_tip2=gtk_tooltips_new();
  gtk_tooltips_set_tip(fix_tip2,fix_button2,"Clear the Fix buffer", "");
  gtk_tooltips_enable(fix_tip2);

  g_signal_connect(GTK_OBJECT(fix_button1), "clicked",
		   G_CALLBACK(open_pressed), NULL);

  g_signal_connect(GTK_OBJECT(fix_button2), "clicked",
		   G_CALLBACK(clear_fix_pressed), NULL);

  gtk_box_pack_start(GTK_BOX(fix_hbuttons), fix_button1, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(fix_hbuttons), fix_button2, FALSE, FALSE, 0);

  gtk_box_pack_start(GTK_BOX(fix_vbox), fix_frame, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(fix_vbox), fix_hbuttons, FALSE, FALSE, 4);

  /* Notebook */
  mynotebook=gtk_notebook_new();

  gtk_notebook_append_page(GTK_NOTEBOOK(mynotebook), email_vbox,tab_email);
  gtk_notebook_append_page(GTK_NOTEBOOK(mynotebook), type_vbox,tab_type);
  gtk_notebook_append_page(GTK_NOTEBOOK(mynotebook), system_vbox,tab_system);
  gtk_notebook_append_page(GTK_NOTEBOOK(mynotebook), details_vbox,tab_descr);
  gtk_notebook_append_page(GTK_NOTEBOOK(mynotebook), fix_vbox,tab_fix);
		
  gtk_box_pack_start(GTK_BOX(vbox1), mynotebook, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox1), hseparator1, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox1), h_buttons, FALSE, FALSE, 0);
  gtk_box_set_homogeneous(GTK_BOX(vbox1), FALSE);
  gtk_container_add(GTK_CONTAINER(window), vbox1);

  gtk_widget_show_all(window);

  /* Show warning message if we couldn't load the file */
  if (load_failed == TRUE) {

    snprintf(file_warning,1024, "Unable to read: %s", included_file);
    file_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
					 GTK_DIALOG_DESTROY_WITH_PARENT,
					 GTK_MESSAGE_ERROR,
					 GTK_BUTTONS_CLOSE,
					 file_warning);
    gtk_dialog_run(GTK_DIALOG(file_dialog));
    gtk_widget_destroy(file_dialog);

  }

  /* Let the show begin */
  gtk_main();

  return 0;

}

/* CALLBACKS */

static gint
delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{

  GtkWidget *dialog;
  gint result;

  if (dirty > 0) {

    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_WARNING,
				    GTK_BUTTONS_YES_NO,
				    "Report not sent yet, quit anyway?");
    result = gtk_dialog_run(GTK_DIALOG (dialog));
    gtk_widget_destroy(dialog);

  } else {

    return FALSE;

  }

  if (result == GTK_RESPONSE_YES) {

    update_profile();
    save_settings(&my_profile);
    return FALSE;

  } else {

    return TRUE;

  }

}

static void
destroy( GtkWidget *widget, gpointer data)
{
  update_profile();
  save_settings(&my_profile);
  gtk_main_quit();
}

static void
quit_pressed( GtkWidget *widget, gpointer data)
{
  GtkWidget *dialog;
  gint result;

  if (dirty > 0) {

    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_WARNING,
				    GTK_BUTTONS_YES_NO,
				    "Report not sent yet, quit anyway?");
    result = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);    

    if (result == GTK_RESPONSE_YES) {

      update_profile();
      save_settings(&my_profile);
      gtk_main_quit();

    }

  } else {

    update_profile();
    save_settings(&my_profile);
    gtk_main_quit();

  }
        
}

static void
about_pressed( GtkWidget *widget, gpointer data)
{
#if(GTK_MINOR_VERSION>=6)

  const gchar *authors[] = {
    "Miguel Mendez <flynn@energyhq.es.eu.org>",
    NULL
  };

  const gchar *artists[] = {
    "Alan Smith <alan@toonart.co.uk>",
    NULL
  };

  const gchar *comments = 
    "A user friendly GNATS client";

  const gchar *license =
    "Copyright (c) 2003, 2004, 2005 Miguel Mendez <flynn@energyhq.es.eu.org>\n"
    "\n"
    "Redistribution and use in source and binary forms, with or without\n"
    "modification, are permitted provided that the following conditions are met:\n"
    "\n"
    "* Redistributions of source code must retain the above copyright notice,\n"
    "this list of conditions and the following disclaimer.\n"
    "* Redistributions in binary form must reproduce the above copyright notice,\n"
    "this list of conditions and the following disclaimer in the documentation\n"
    "and/or other materials provided with the distribution.\n"
    "\n"
    "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n"
    "AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n"
    "IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n"
    "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE\n"
    "FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n"
    "DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n"
    "SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER\n"
    "CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,\n"
    "OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n"
    "OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n";

  gtk_show_about_dialog(GTK_WINDOW(window),
			"authors", authors,
			"artists", artists,
			"copyright", "(C) 2003, 2004, 2005 Miguel Mendez",
			"comments", comments,
			"license", license,
			"logo", icon64_pixbuf,
			"name", "gtk-send-pr",
			"version", GSP_VERSION,
			"website", "http://www.energyhq.es.eu.org/gtk-send-pr.html",
			NULL);

#else

  GtkWidget *dialog;

  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				  GTK_DIALOG_DESTROY_WITH_PARENT,
				  GTK_MESSAGE_INFO,
				  GTK_BUTTONS_OK,
				  "gtk-send-pr "
				  GSP_VERSION " "
				  GSP_CODENAME
				  "\nCopyright (c) 2003-2005, "
				  "Miguel Mendez\nE-Mail: <flynn@energyhq.es.eu.org>\n"
				  "http://www.energyhq.es.eu.org/gtk-send-pr.html\n");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

#endif
}

static void
help_pressed( GtkWidget *widget, gpointer data)
{

  GtkWidget *dialog;

  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				  GTK_DIALOG_DESTROY_WITH_PARENT,
				  GTK_MESSAGE_INFO,
				  GTK_BUTTONS_OK,
				  "Please, read the gtk-send-pr man page \n"
				  "for info on using this program.\n");
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);

}

static void
send_pressed( GtkWidget *widget, gpointer data)
{
  int retcode;
  GtkWidget *dialog;

  PROBLEM_REPORT mypr;


  fill_pr(&mypr);
  retcode = send_pr(&mypr);

  if (retcode == 0) {

    dirty=0;
    gtk_widget_set_sensitive(send_button, FALSE);
    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_INFO,
				    GTK_BUTTONS_OK,
				    "Problem report sent, thanks.\n");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

  } else {

    dialog = gtk_message_dialog_new(GTK_WINDOW(window),
				    GTK_DIALOG_DESTROY_WITH_PARENT,
				    GTK_MESSAGE_ERROR,
				    GTK_BUTTONS_OK,
				    global_smtp_error_msg);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);	

  }

}

static void
open_pressed( GtkWidget *widget, gpointer data)
{
  GtkWidget *file_dialog;
  char *selected_filename;
  GtkWidget *error_dialog;
  char file_warning[1024];

  file_dialog = gtk_file_chooser_dialog_new("Select file to be included in the Fix field",
					    GTK_WINDOW(window),
					    GTK_FILE_CHOOSER_ACTION_OPEN,
					    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
					    GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
					    NULL);

  if (gtk_dialog_run(GTK_DIALOG(file_dialog)) == GTK_RESPONSE_ACCEPT) {

    selected_filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file_dialog));

    fix_buffer = load_file(selected_filename);

    g_free(selected_filename);

    if (fix_buffer != NULL) {

      gtk_text_buffer_insert_at_cursor(fix_buffer1, fix_buffer, -1);
      free(fix_buffer);

    } else {

      snprintf(file_warning, 1024, "Unable to read: %s", selected_filename);
      error_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
					    GTK_DIALOG_DESTROY_WITH_PARENT,
					    GTK_MESSAGE_ERROR,
					    GTK_BUTTONS_OK,
					    file_warning);
      gtk_dialog_run(GTK_DIALOG(error_dialog));
      gtk_widget_destroy(error_dialog);

    }

  }

  gtk_widget_destroy(file_dialog);

}

static void
clear_fix_pressed(GtkWidget *widget, gpointer data)
{

  gtk_text_buffer_set_text(fix_buffer1, "", 0);
  /*
   * Let the user drag and drop the same file again
   * if she wants to.
   */
  memset(drag_dupe, 0, 1024);
}

static void
fill_pr(PROBLEM_REPORT *mypr)
{

  char *ctmp;
  char *from;
  char c;
  int i,j;
  GtkTextBuffer *buffer;
  GtkTextIter beg_iter;
  GtkTextIter end_iter;
  char *temp_cc,*cc_field;
  char *temp_ssl;

  /* Process CC field */
  mypr->smtp_cc_num = 0;

  mypr->smtp_cc_text = (char *)gtk_entry_get_text(GTK_ENTRY(email_entry4));
  cc_field=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry4));

  if (strlen(mypr->smtp_cc_text)>0) {

    do {

      temp_cc = strsep(&cc_field, ",");

      if (temp_cc != NULL) {
	
	mypr->smtp_cc[mypr->smtp_cc_num] = temp_cc;
	mypr->smtp_cc_num++;
      }

    } while (temp_cc != NULL);

  }

  mypr->smtp_server = (char *)gtk_entry_get_text(GTK_ENTRY(email_entry8));
  mypr->smtp_port =  (char *)gtk_entry_get_text(GTK_ENTRY(email_port_entry));
  mypr->smtp_from = (char *)gtk_entry_get_text(GTK_ENTRY(email_entry2));
  mypr->smtp_to = (char *)gtk_entry_get_text(GTK_ENTRY(email_entry1));

  mypr->smtp_rcpt = default_rcpt;

  temp_ssl = gtk_combo_box_get_active_text(GTK_COMBO_BOX(email_ssl_option));

  if (strcmp(temp_ssl, SSL_NO) == 0) {

    mypr->ssl_mode = GSP_SSL_NO;

  } else if (strcmp(temp_ssl, SSL_EN) == 0) {

    mypr->ssl_mode = GSP_SSL_EN;

  } else if (strcmp(temp_ssl, SSL_RE) == 0) {

    mypr->ssl_mode = GSP_SSL_RE;

  } else {

    fprintf(stderr, "Warning, SSL mode was in undefined state.\n");
    mypr->ssl_mode = GSP_SSL_NO;

  }

  g_free(temp_ssl);

  mypr->smtp_subject = (char *)gtk_entry_get_text(GTK_ENTRY(type_entry1));
  mypr->submitter_id = (char *)gtk_entry_get_text(GTK_ENTRY(email_entry5));
  mypr->originator = (char *)gtk_entry_get_text(GTK_ENTRY(email_entry6));
  mypr->organization = (char *)gtk_entry_get_text(GTK_ENTRY(email_entry7));
  mypr->synopsis = (char *)gtk_entry_get_text(GTK_ENTRY(type_entry1));

  mypr->severity = (char *)gtk_entry_get_text(GTK_ENTRY(GTK_BIN(type_combo1)->child));
  mypr->priority = (char *)gtk_entry_get_text(GTK_ENTRY(GTK_BIN(type_combo2)->child));
  mypr->category = (char *)gtk_entry_get_text(GTK_ENTRY(GTK_BIN(type_combo3)->child));
  mypr->class = (char *)gtk_entry_get_text(GTK_ENTRY(GTK_BIN(type_combo4)->child));

  mypr->release = (char *)gtk_entry_get_text(GTK_ENTRY(system_entry1));

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(system_view1));
  i = gtk_text_buffer_get_char_count(buffer);
  gtk_text_buffer_get_iter_at_offset(buffer,(GtkTextIter *)&beg_iter,0);
  gtk_text_buffer_get_end_iter(buffer, (GtkTextIter *)&end_iter);
  from = gtk_text_iter_get_visible_text(&beg_iter,&end_iter);

  env_buffer = malloc(2*i+16);
  memset(env_buffer,0,2*i+16);
  ctmp = env_buffer;

  for (j = 0; j < i; j++) {

    c = (char )*from++;
    if (c != '\n') {

      *ctmp++ = c;

    } else if (c > 126 || (c < 31 && c != '\n')) {

      break;

    } else {

      *ctmp++ = '\r';
      *ctmp++ = c;
    }

  }

  mypr->environment = env_buffer;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(details_view1));
  i = gtk_text_buffer_get_char_count(buffer);
  gtk_text_buffer_get_iter_at_offset(buffer,(GtkTextIter *)&beg_iter,0);
  gtk_text_buffer_get_end_iter(buffer, (GtkTextIter *)&end_iter);
  from = gtk_text_iter_get_visible_text(&beg_iter,&end_iter);

  desc_buffer = malloc(2*i+16);
  memset(desc_buffer,0,2*i+16);
  ctmp = desc_buffer;

  for (j = 0; j < i; j++) {

    c = (char )*from++;

    if (c != '\n') {

      *ctmp++ = c;

    } else if (c > 126 || (c < 31 && c != '\n')) {

      break;

    } else {

      *ctmp++ = '\r';
      *ctmp++ = c;
    }

  }

  mypr->description = desc_buffer;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(details_view2));
  i = gtk_text_buffer_get_char_count(buffer);
  gtk_text_buffer_get_iter_at_offset(buffer,(GtkTextIter *)&beg_iter,0);
  gtk_text_buffer_get_end_iter(buffer, (GtkTextIter *)&end_iter);
  from = gtk_text_iter_get_visible_text(&beg_iter,&end_iter);
        					 
  how_buffer = malloc(2*i+16); /* Let's be convervative here */
  memset(how_buffer,0,2*i+16);
  ctmp = how_buffer;

  for ( j = 0; j < i; j++) {

    c = (char )*from++;

    if (c != '\n') {

      *ctmp++ = c;

    } else if (c > 126 || (c < 31 && c != '\n')) {

      break;

    } else {

      *ctmp++ = '\r';
      *ctmp++ = c;

    }

  }

  mypr->how_to_repeat = how_buffer;

  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(fix_view));
  i = gtk_text_buffer_get_char_count(buffer);
  gtk_text_buffer_get_iter_at_offset(buffer,(GtkTextIter *)&beg_iter,0);
  gtk_text_buffer_get_end_iter(buffer, (GtkTextIter *)&end_iter);
  from = gtk_text_iter_get_visible_text(&beg_iter,&end_iter);

  fix_buffer = malloc(2*i+16); /* Let's be convervative here */
  memset(fix_buffer,0,2*i+16);
  ctmp = fix_buffer;

  for(j = 0; j < i; j++) {

    c = (char )*from++;

    if (c != '\n') {

      *ctmp++ = c;

    } else if (c > 126 || (c < 31 && c != '\n')) {

      break;

    } else {

      *ctmp++ = '\r';
      *ctmp++ = c;

    }

  }

  mypr->fix = fix_buffer;

}

static void
update_profile(void)
{
  int newsize[2];
  char *tmp_entry;

  gtk_window_get_size(GTK_WINDOW(window), &newsize[0], &newsize[1]);

  my_profile.geom_x = newsize[0];
  my_profile.geom_y = newsize[1];

  tmp_entry = (char *)gtk_entry_get_text(GTK_ENTRY(email_entry2));
  strncpy(my_profile.email, tmp_entry, sizeof(my_profile.email));
  g_free(tmp_entry);

  tmp_entry = (char *)gtk_entry_get_text(GTK_ENTRY(email_entry6));
  strncpy(my_profile.name, tmp_entry, sizeof(my_profile.name));	
  g_free(tmp_entry);

  tmp_entry = (char *)gtk_entry_get_text(GTK_ENTRY(email_entry7));
  strncpy(my_profile.org, tmp_entry, sizeof(my_profile.org));	
  g_free(tmp_entry);

  tmp_entry = (char *)gtk_entry_get_text(GTK_ENTRY(email_entry8));
  strncpy(my_profile.smtp, tmp_entry, sizeof(my_profile.smtp));
  g_free(tmp_entry);

  tmp_entry = (char *)gtk_entry_get_text(GTK_ENTRY(email_port_entry));
  strncpy(my_profile.smtp_port, tmp_entry, sizeof(my_profile.smtp_port));
  g_free(tmp_entry);

  tmp_entry = gtk_combo_box_get_active_text(GTK_COMBO_BOX(email_ssl_option));

  if (strcmp(tmp_entry, SSL_NO) == 0) {

    my_profile.ssl_mode = GSP_SSL_NO;

  } else if (strcmp(tmp_entry, SSL_EN) == 0) {

    my_profile.ssl_mode = GSP_SSL_EN;

  } else if (strcmp(tmp_entry, SSL_RE) == 0) {

    my_profile.ssl_mode = GSP_SSL_RE;

  } else {

    fprintf(stderr, "Warning, SSL mode was in undefined state.\n");
    my_profile.ssl_mode = GSP_SSL_NO;

  }

  g_free(tmp_entry);

}

static void
fix_view_drag_data_received(GtkWidget          *widget,
			    GdkDragContext     *context,
			    gint                x,
			    gint                y,
			    GtkSelectionData   *data,
			    guint               info,
			    guint               time)
{

  char file_buffer[1024];
  char *file_temp;
  GtkWidget *file_dialog;
  char file_warning[1024];
  int i;

  memset(file_buffer, 0, 1024);

  if ((data->length >= 0) && (data->format == 8))
    {

#ifdef DEBUG_DND
      printf("Received \"%s\" in fix_view\n", data->data);
#endif

      file_temp = (char *) data->data;

      if (strncmp(drag_dupe, file_temp, 1024) == 0) {

#ifdef DEBUG_DND
	printf("Got duplicated drop [%s]\n", drag_dupe);
#endif

	gtk_drag_finish(context, TRUE, FALSE, time);
	return;

      } else {

	strncpy(drag_dupe, file_temp, 1024);

      }

      if (strncmp(file_temp, "file:", strlen("file:")) == 0 ) {

	strncpy(file_buffer, file_temp + strlen("file:"), 1024);

	for (i = 0; i < 1024; i++) {

	  if (file_buffer[i] == '\n'|| file_buffer[i] == '\r') file_buffer[i] = '\0';

	}

	fix_buffer = load_file(file_buffer);

	if (fix_buffer != NULL) {

	  gtk_text_buffer_insert_at_cursor(fix_buffer1, fix_buffer, -1);
	  free(fix_buffer);

	} else {

	  snprintf(file_warning, 1024, "Unable to read: %s", file_buffer);
	  file_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
					       GTK_DIALOG_DESTROY_WITH_PARENT,
					       GTK_MESSAGE_ERROR,
					       GTK_BUTTONS_OK,
					       file_warning);

	  gtk_dialog_run(GTK_DIALOG (file_dialog));
	  gtk_widget_destroy(file_dialog);

	}

      }


      gtk_drag_finish(context, TRUE, FALSE, time);
      return;

    }

  gtk_drag_finish(context, FALSE, FALSE, time);

}

int
gsp_smtp_auth_dialog(GSP_AUTH *my_auth)
{

  auth_info = my_auth;
  gsp_auth_done = FALSE;

  auth_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_modal(GTK_WINDOW(auth_window), TRUE);
  gtk_window_set_title(GTK_WINDOW(auth_window), "SMTP Auth");
  gtk_window_set_resizable(GTK_WINDOW(auth_window), TRUE);
  gtk_window_set_transient_for(GTK_WINDOW(auth_window), GTK_WINDOW(window));
  gtk_window_set_destroy_with_parent(GTK_WINDOW(auth_window), TRUE);

  g_signal_connect(GTK_OBJECT(auth_window), "delete_event",
		   G_CALLBACK(delete_event), NULL);

  g_signal_connect(GTK_OBJECT(auth_window), "destroy",
		   G_CALLBACK(destroy), NULL);

  auth_vbox = gtk_vbox_new(FALSE, 4);

  auth_label = gtk_label_new("\nSMTP server requires authentication\n");

  auth_userframe = gtk_frame_new("User name");
  auth_userentry = gtk_entry_new();
  gtk_container_add(GTK_CONTAINER(auth_userframe), auth_userentry);

  auth_passframe = gtk_frame_new("Password");
  auth_passentry = gtk_entry_new();
  gtk_entry_set_visibility(GTK_ENTRY(auth_passentry), FALSE);
  gtk_container_add(GTK_CONTAINER(auth_passframe), auth_passentry);

  auth_ok = gtk_button_new_from_stock(GTK_STOCK_OK);

  g_signal_connect(GTK_OBJECT(auth_ok), "clicked",
		   G_CALLBACK(auth_ok_pressed), NULL);

  gtk_box_pack_start(GTK_BOX(auth_vbox),auth_label, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX(auth_vbox),auth_userframe, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX(auth_vbox),auth_passframe, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX(auth_vbox),auth_ok, FALSE, FALSE, 4);

  gtk_box_set_homogeneous(GTK_BOX(auth_vbox), FALSE);
  gtk_container_add(GTK_CONTAINER(auth_window), auth_vbox);

  gtk_widget_show_all(auth_window);

  while(gsp_auth_done != TRUE) {

    gtk_main_iteration();

  }

  return 0;

}

static void
auth_ok_pressed( GtkWidget *widget, gpointer data)
{

  /* Keep a copy before destroying the widgets */
  strncpy(auth_info->username,(char *)gtk_entry_get_text(GTK_ENTRY(auth_userentry)), 1023);
  strncpy(auth_info->password,(char *)gtk_entry_get_text(GTK_ENTRY(auth_passentry)), 1023);

  gtk_widget_destroy(auth_window);

  gsp_auth_done = TRUE;

}

/* Show available categories */
void 
show_categories(void)
{
  int i;
  int len;
  int written;
  char my_fmt[16];

  printf("Known categories:\n");

  len = 0;

  for (i = 0; i < (sizeof(pr_categories)/sizeof(char *)); i++) {

    if (strlen(pr_categories[i]) > len ) len = strlen(pr_categories[i]);

  }

  snprintf(my_fmt, sizeof(my_fmt), "%%-%is ", len);

  written = 0;

  for (i = 0; i < (sizeof(pr_categories)/sizeof(char *)); i++) {

    written += printf(my_fmt, pr_categories[i]);

    if (written >= 70 ) {

      written = 0;
      printf("\n");

    }

  }

  printf("\n");

  exit(EXIT_SUCCESS);

}
