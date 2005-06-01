/*
  Copyright (c) 2003-2004, Miguel Mendez. All rights reserved.

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
#include <sys/uio.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pwd.h>
#include <libgen.h>
#include <time.h>
extern char *tzname[2];

#include <gtk/gtk.h>

#include "gtk-send-pr.h"
#include "gtk_ui.h"
#include "smtp.h"
#include "pr_defs.h"
#include "user_settings.h"
#include "file.h"

extern char global_smtp_error_msg[1024];

gint delete_event( GtkWidget *, GdkEvent *, gpointer);
void destroy( GtkWidget *, gpointer);
void quit_pressed(GtkWidget *, gpointer);
void about_pressed(GtkWidget *, gpointer);
void help_pressed(GtkWidget *, gpointer);
void send_pressed(GtkWidget *, gpointer);
void open_pressed(GtkWidget *, gpointer);
void clear_fix_pressed(GtkWidget *, gpointer);
void select_ok(GtkWidget *, gpointer);
void select_cancelled(GtkWidget *, gpointer);
void fill_pr(PROBLEM_REPORT *);
void update_profile(void);

extern char *included_file;

int dirty;
GtkWidget *window;
struct passwd *my_user;
struct utsname my_uname;
USER_PROFILE my_profile;
GtkWidget *type_entry1;

GtkWidget *email_entry1;
GtkWidget *email_entry2;
GtkWidget *email_entry4;
GtkWidget *email_entry5;
GtkWidget *email_entry6;
GtkWidget *email_entry7;
GtkWidget *email_entry8;
GtkWidget *type_combo1;
GtkWidget *type_combo2;
GtkWidget *type_combo3;
GtkWidget *type_combo4;
GtkWidget *system_entry1;
GtkWidget *system_view1;
GtkWidget *details_view1;
GtkWidget *details_view2;
GtkWidget *fix_view;
GtkWidget *fix_filsel;
GtkTextBuffer *fix_buffer1;
GtkWidget *send_button;

char *env_buffer;
char *desc_buffer;
char *how_buffer;
char *fix_buffer;

int open_menu_up;

int 
create_gtk_ui(char *included_file)
{

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

  GtkWidget	*email_frame2;
  GtkWidget	*email_frame3;
  GtkWidget	*email_entry3;
  GtkWidget	*email_frame4;
  GtkWidget	*email_frame5;
  GtkWidget	*email_frame6;
  GtkWidget	*email_frame7;
  GtkWidget	*email_frame8;

  GtkWidget	*type_vbox;
  GtkWidget	*type_frame1;
  GtkWidget	*type_frame2;
  GList		*type_items1 = NULL;
  GtkWidget	*type_frame3;
  GList		*type_items2 = NULL;
  GtkWidget	*type_frame4;
  GList		*type_items3 = NULL;
  GtkWidget	*type_frame5;
  GList		*type_items4 = NULL;
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


  char uname_srm[256];
  char uname_snrvm[1024]; /* better safe than sorry */
  int i;
  char *fix_buffer=NULL; /* Buffer for the -a option */
  GtkWidget *file_dialog;
  char file_warning[1024];

  open_menu_up=0;

  /* Let's go */

  i=uname(&my_uname);
  if(i==-1) {
    perror("uname()");
    exit(EXIT_FAILURE);
  }

  load_settings(&my_profile);

  snprintf(uname_srm,255,"%s %s %s",my_uname.sysname,
	   my_uname.release,
	   my_uname.machine);

  snprintf(uname_snrvm,1023,"System: %s %s %s %s %s",
	   my_uname.sysname,
	   my_uname.nodename,
	   my_uname.release,
	   my_uname.version,
	   my_uname.machine);

  dirty=1;
  /* Define main window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable(GTK_WINDOW(window),TRUE);
  gtk_window_set_title(GTK_WINDOW(window), "GTK Send-PR");
  gtk_window_set_default_size(GTK_WINDOW(window),my_profile.geom_x,my_profile.geom_y);

  /* Basic buttons */
  send_button = gtk_button_new_from_stock("Send");
  quit_button = gtk_button_new_from_stock(GTK_STOCK_QUIT);
  about_button = gtk_button_new_with_label("About");
  help_button = gtk_button_new_from_stock(GTK_STOCK_HELP);

  /* Button tooltips */
  send_tip=gtk_tooltips_new();
  gtk_tooltips_set_tip(send_tip,send_button,"Send problem report","");
  gtk_tooltips_enable(send_tip);

  help_tip=gtk_tooltips_new();
  gtk_tooltips_set_tip(help_tip,help_button,"Show online help","");
  gtk_tooltips_enable(help_tip);

  about_tip=gtk_tooltips_new();
  gtk_tooltips_set_tip(about_tip,about_button,"About this program","");
  gtk_tooltips_enable(about_tip);

  quit_tip=gtk_tooltips_new();
  gtk_tooltips_set_tip(quit_tip,quit_button,"Exit this program","");
  gtk_tooltips_enable(quit_tip);

  h_buttons=gtk_hbutton_box_new();

  gtk_box_pack_start(GTK_BOX(h_buttons), send_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(h_buttons), about_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(h_buttons), help_button, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(h_buttons), quit_button, FALSE, FALSE, 0);

  g_signal_connect(GTK_OBJECT(window), "delete_event",
		   GTK_SIGNAL_FUNC(delete_event), NULL);

  g_signal_connect(GTK_OBJECT(window), "destroy",
		   GTK_SIGNAL_FUNC(destroy), NULL);

  g_signal_connect(GTK_OBJECT(quit_button), "clicked",
		   GTK_SIGNAL_FUNC(quit_pressed), NULL);

  g_signal_connect(GTK_OBJECT(about_button), "clicked",
		   GTK_SIGNAL_FUNC(about_pressed), NULL);

  g_signal_connect(GTK_OBJECT(send_button), "clicked",
		   GTK_SIGNAL_FUNC(send_pressed), NULL);

  g_signal_connect(GTK_OBJECT(help_button), "clicked",
		   GTK_SIGNAL_FUNC(help_pressed), NULL);				

  vbox1 = gtk_vbox_new(FALSE, 2);
  hseparator1 = gtk_hseparator_new ();

  tab_email=gtk_label_new("E-Mail");
  tab_type=gtk_label_new("Type");
  tab_system=gtk_label_new("System");
  tab_descr=gtk_label_new("Details");
  tab_fix=gtk_label_new("Fix");

  /* E-mail stuff */
  email_vbox = gtk_vbox_new(FALSE, 2);

  email_frame1=gtk_frame_new(" To ");
  email_entry1=gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry1), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry1), default_to);

  email_frame2=gtk_frame_new(" From ");
  email_entry2=gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry2), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry2), my_profile.email);

  email_frame3=gtk_frame_new(" Reply to ");
  email_entry3=gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry3), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry3), my_profile.email);

  email_frame4=gtk_frame_new(" CC ");
  email_entry4=gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry4), 255);	

  email_frame5=gtk_frame_new(" Submitter-Id ");
  email_entry5=gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry5), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry5), default_sub_id);

  email_frame6=gtk_frame_new(" Originator ");
  email_entry6=gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry6), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry6), my_profile.name);

  email_frame7=gtk_frame_new(" Organization ");
  email_entry7=gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry7), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry7), my_profile.org);

  email_frame8=gtk_frame_new(" SMTP Server ");
  email_entry8=gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(email_entry8), 255);
  gtk_entry_set_text(GTK_ENTRY(email_entry8), my_profile.smtp);

  gtk_container_add(GTK_CONTAINER(email_frame1), email_entry1);
  gtk_container_add(GTK_CONTAINER(email_frame2), email_entry2);
  gtk_container_add(GTK_CONTAINER(email_frame3), email_entry3);
  gtk_container_add(GTK_CONTAINER(email_frame4), email_entry4);
  gtk_container_add(GTK_CONTAINER(email_frame5), email_entry5);
  gtk_container_add(GTK_CONTAINER(email_frame6), email_entry6);
  gtk_container_add(GTK_CONTAINER(email_frame7), email_entry7);
  gtk_container_add(GTK_CONTAINER(email_frame8), email_entry8);

  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame1, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame2, TRUE, TRUE, 4);	
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame3, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame4, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame5, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame6, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame7, TRUE, TRUE, 4);			
  gtk_box_pack_start(GTK_BOX(email_vbox), email_frame8, TRUE, TRUE, 4);	

  /* PR type code */
  type_vbox = gtk_vbox_new(FALSE, 2);

  type_frame1=gtk_frame_new(" Synopsis ");
  type_entry1=gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(type_entry1), 255);

  /* Severity */
  type_frame2=gtk_frame_new(" Severity ");

  for(i=0;i<(sizeof(pr_severities)/sizeof(char *));i++) {
    type_items1 = g_list_append (type_items1, pr_severities[i]);
  }

  type_combo1 = gtk_combo_new();
  gtk_combo_set_popdown_strings (GTK_COMBO(type_combo1), type_items1);

  gtk_container_add(GTK_CONTAINER(type_frame1), type_entry1);
  gtk_container_add(GTK_CONTAINER(type_frame2), type_combo1);

  gtk_box_pack_start(GTK_BOX(type_vbox), type_frame1, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(type_vbox), type_frame2, TRUE, TRUE, 4);

  /* Priority */
  type_frame3=gtk_frame_new(" Priority ");
  for(i=0;i<(sizeof(pr_priorities)/sizeof(char *));i++) {
    type_items2 = g_list_append (type_items2, pr_priorities[i]);
  }

  type_combo2 = gtk_combo_new();
  gtk_combo_set_popdown_strings(GTK_COMBO (type_combo2), type_items2);

  gtk_container_add(GTK_CONTAINER(type_frame3), type_combo2);
  gtk_box_pack_start(GTK_BOX(type_vbox), type_frame3, TRUE, TRUE, 4);

  /* Category */
  type_frame4=gtk_frame_new(" Category ");
  for(i=0;i<(sizeof(pr_categories)/sizeof(char *));i++) {
    type_items3 = g_list_append (type_items3, pr_categories[i]);
  }
  type_combo3 = gtk_combo_new();
  gtk_combo_set_popdown_strings(GTK_COMBO (type_combo3), type_items3);

  gtk_container_add(GTK_CONTAINER(type_frame4), type_combo3);
  gtk_box_pack_start(GTK_BOX(type_vbox), type_frame4, TRUE, TRUE, 4);

  /* Class */
  type_frame5=gtk_frame_new(" Class ");
  for(i=0;i<(sizeof(pr_classes)/sizeof(char *));i++) {
    type_items4 = g_list_append (type_items4, pr_classes[i]);
  }	

  type_combo4 = gtk_combo_new();
  gtk_combo_set_popdown_strings(GTK_COMBO (type_combo4), type_items4);

  gtk_container_add(GTK_CONTAINER(type_frame5), type_combo4);
  gtk_box_pack_start(GTK_BOX(type_vbox), type_frame5, TRUE, TRUE, 4);

  /* System info */
  system_vbox = gtk_vbox_new(FALSE, 2);	

  system_frame1=gtk_frame_new(" Release ");	

  system_entry1=gtk_entry_new();
  gtk_entry_set_max_length(GTK_ENTRY(system_entry1), 255);
  gtk_entry_set_text(GTK_ENTRY(system_entry1), uname_srm);

  /* Environment */

  system_frame2=gtk_frame_new(" Environment ");

  system_view1 = gtk_text_view_new();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(system_view1),GTK_WRAP_CHAR);
  system_buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(system_view1));
  gtk_text_buffer_set_text(system_buffer1, uname_snrvm, -1);

  scrolled_window1=gtk_scrolled_window_new(NULL, NULL);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window1),10);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window1),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window1)
					,system_view1);

  gtk_container_add(GTK_CONTAINER (system_frame1), system_entry1);
  gtk_container_add(GTK_CONTAINER (system_frame2), scrolled_window1);

  gtk_box_pack_start(GTK_BOX (system_vbox), system_frame1, FALSE, FALSE, 4);
  gtk_box_pack_start(GTK_BOX (system_vbox), system_frame2, TRUE, TRUE, 4);

  /* PR details */
  details_vbox = gtk_vbox_new (FALSE, 2);	
  details_frame1=gtk_frame_new(" Description ");

  details_view1 = gtk_text_view_new ();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(details_view1), GTK_WRAP_CHAR);

  scrolled_window2=gtk_scrolled_window_new(NULL, NULL);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window2),10);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window2),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window2)
					,details_view1);

  details_frame2=gtk_frame_new(" How-To-Repeat ");

  details_view2 = gtk_text_view_new ();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(details_view2), GTK_WRAP_CHAR);

  scrolled_window3=gtk_scrolled_window_new(NULL, NULL);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window3),10);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window3),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window3)
					,details_view2);

  gtk_container_add(GTK_CONTAINER(details_frame1), scrolled_window2);
  gtk_container_add(GTK_CONTAINER(details_frame2), scrolled_window3);

  gtk_box_pack_start(GTK_BOX(details_vbox), details_frame1, TRUE, TRUE, 4);
  gtk_box_pack_start(GTK_BOX(details_vbox), details_frame2, TRUE, TRUE, 4);

  /* Fix */
  fix_vbox = gtk_vbox_new (FALSE, 2);
  fix_frame=gtk_frame_new(" Fix ");

  fix_view = gtk_text_view_new ();
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(fix_view), GTK_WRAP_CHAR);

  fix_buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(fix_view));

  if(included_file!=NULL) {

    fix_buffer=load_file(included_file);
    if(fix_buffer!=NULL) {

      gtk_text_buffer_set_text(fix_buffer1, fix_buffer, -1);
      free(fix_buffer);

    } else {

      snprintf(file_warning,1024,"Unable to read: %s",included_file);
      file_dialog = gtk_message_dialog_new (GTK_WINDOW(window),
					    GTK_DIALOG_DESTROY_WITH_PARENT,
					    GTK_MESSAGE_ERROR,
					    GTK_BUTTONS_OK,
					    file_warning);
      gtk_dialog_run (GTK_DIALOG (file_dialog));
      gtk_widget_destroy (file_dialog);

    }

  }

  scrolled_window4=gtk_scrolled_window_new(NULL, NULL);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window4),10);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window4),
				  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window4)
					,fix_view);

  gtk_container_add(GTK_CONTAINER(fix_frame), scrolled_window4);

  fix_hbuttons=gtk_hbutton_box_new();

  fix_button1 = gtk_button_new_from_stock(GTK_STOCK_OPEN);
  fix_tip1=gtk_tooltips_new();
  gtk_tooltips_set_tip(fix_tip1,fix_button1,"Insert a file at cursor location","");
  gtk_tooltips_enable(fix_tip1);

  fix_button2 = gtk_button_new_from_stock(GTK_STOCK_CLEAR);
  fix_tip2=gtk_tooltips_new();
  gtk_tooltips_set_tip(fix_tip2,fix_button2,"Clear the Fix buffer","");
  gtk_tooltips_enable(fix_tip2);

  g_signal_connect(GTK_OBJECT(fix_button1), "clicked",
		   GTK_SIGNAL_FUNC(open_pressed), NULL);

  g_signal_connect(GTK_OBJECT(fix_button2), "clicked",
		   GTK_SIGNAL_FUNC(clear_fix_pressed), NULL);

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

  gtk_widget_show(mynotebook);
  gtk_widget_show(vbox1);

  gtk_widget_show(email_frame1);
  gtk_widget_show(email_entry1);

  gtk_widget_show(email_frame2);
  gtk_widget_show(email_entry2);

  gtk_widget_show(email_frame3);
  gtk_widget_show(email_entry3);

  gtk_widget_show(email_frame4);
  gtk_widget_show(email_entry4);				

  gtk_widget_show(email_frame5);
  gtk_widget_show(email_entry5);

  gtk_widget_show(email_frame6);
  gtk_widget_show(email_entry6);	

  gtk_widget_show(email_frame7);
  gtk_widget_show(email_entry7);

  gtk_widget_show(email_frame8);
  gtk_widget_show(email_entry8);

  gtk_widget_show(email_vbox);

  gtk_widget_show(type_frame1);
  gtk_widget_show(type_entry1);	

  gtk_widget_show(type_frame2);
  gtk_widget_show(type_combo1);

  gtk_widget_show(type_frame3);
  gtk_widget_show(type_combo2);

  gtk_widget_show(type_frame4);
  gtk_widget_show(type_combo3);	

  gtk_widget_show(type_frame5);
  gtk_widget_show(type_combo4);
	
  gtk_widget_show(type_vbox);

  gtk_widget_show(system_frame1);
  gtk_widget_show(system_entry1);

  gtk_widget_show(system_frame2);
  gtk_widget_show(system_view1);
  gtk_widget_show(scrolled_window1);
	
  gtk_widget_show(system_vbox);

  gtk_widget_show(details_frame1);
  gtk_widget_show(details_view1);
  gtk_widget_show(scrolled_window2);

  gtk_widget_show(details_frame2);
  gtk_widget_show(details_view2);
  gtk_widget_show(scrolled_window3);

  gtk_widget_show(details_vbox);

  gtk_widget_show(fix_frame);
  gtk_widget_show(fix_view);
  gtk_widget_show(scrolled_window4);
  gtk_widget_show(fix_hbuttons);
  gtk_widget_show(fix_button1);
  gtk_widget_show(fix_button2);
  gtk_widget_show(fix_vbox);

  gtk_widget_show(hseparator1);
  gtk_widget_show(h_buttons);
  gtk_widget_show(send_button);
  gtk_widget_show(quit_button);
  gtk_widget_show(about_button);
  gtk_widget_show(help_button);
  gtk_widget_show(window);
	
  /* Let the show begin */
  gtk_main ();

  return 0;

}

/* CALLBACKS */

gint 
delete_event( GtkWidget *widget, GdkEvent *event, gpointer data)
{

  GtkWidget *dialog;
  gint result;

  if(dirty>0) {

    dialog = gtk_message_dialog_new (GTK_WINDOW(window),
				     GTK_DIALOG_DESTROY_WITH_PARENT,
				     GTK_MESSAGE_WARNING,
				     GTK_BUTTONS_YES_NO,
				     "Report not sent yet, quit anyway?");
    result=gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);

  } else {

    return FALSE;

  }

  if(result==GTK_RESPONSE_YES) {

    update_profile();
    save_settings(&my_profile);
    return FALSE;

  } else {

    return TRUE;

  }

}

void 
destroy( GtkWidget *widget, gpointer data)
{
  update_profile();
  save_settings(&my_profile);
  gtk_main_quit();
}

void 
quit_pressed( GtkWidget *widget, gpointer data)
{
  GtkWidget *dialog;
  gint result;

  if(dirty>0) {

    dialog = gtk_message_dialog_new (GTK_WINDOW(window),
				     GTK_DIALOG_DESTROY_WITH_PARENT,
				     GTK_MESSAGE_WARNING,
				     GTK_BUTTONS_YES_NO,
				     "Report not sent yet, quit anyway?");
    result=gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);    

    if(result==GTK_RESPONSE_YES) {

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


void 
about_pressed( GtkWidget *widget, gpointer data)
{

  GtkWidget *dialog;

  dialog = gtk_message_dialog_new (GTK_WINDOW(window),
				   GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_INFO,
				   GTK_BUTTONS_OK,
				   "gtk-send-pr "
				   GSP_VERSION " "
				   GSP_CODENAME
				   "\nCopyright (c) 2003-2004, "
				   "Miguel Mendez\nE-Mail: <flynn@energyhq.es.eu.org>\n"
				   "http://www.energyhq.es.eu.org/gtk-send-pr.html\n");
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);

}

void 
help_pressed( GtkWidget *widget, gpointer data)
{

  GtkWidget *dialog;

  dialog = gtk_message_dialog_new (GTK_WINDOW(window),
				   GTK_DIALOG_DESTROY_WITH_PARENT,
				   GTK_MESSAGE_INFO,
				   GTK_BUTTONS_OK,
				   "Please, read the gtk-send-pr man page \n"
				   "for info on using this program.\n");
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);

}

void 
send_pressed( GtkWidget *widget, gpointer data)
{
  int retcode;
  GtkWidget *dialog;

  PROBLEM_REPORT mypr;


  fill_pr(&mypr);
  retcode=send_pr(&mypr,&my_uname);

  if(retcode==0) {

    dirty=0;
    gtk_widget_set_sensitive(send_button,FALSE);
    dialog = gtk_message_dialog_new (GTK_WINDOW(window),
				     GTK_DIALOG_DESTROY_WITH_PARENT,
				     GTK_MESSAGE_INFO,
				     GTK_BUTTONS_OK,
				     "Problem report sent, thanks.\n");
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);

  } else {

    dialog = gtk_message_dialog_new (GTK_WINDOW(window),
				     GTK_DIALOG_DESTROY_WITH_PARENT,
				     GTK_MESSAGE_ERROR,
				     GTK_BUTTONS_OK,
				     global_smtp_error_msg);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);	

  }

}

void 
open_pressed( GtkWidget *widget, gpointer data)
{
  if(open_menu_up==0) {

    fix_filsel=gtk_file_selection_new("Select file to be included the Fix field");
    g_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (fix_filsel)->ok_button),
		      "clicked",
		      G_CALLBACK (select_ok),
		      (gpointer) fix_filsel);

    g_signal_connect(GTK_OBJECT (GTK_FILE_SELECTION (fix_filsel)),
		     "delete_event",
		     GTK_SIGNAL_FUNC(select_cancelled), 
		     NULL);

    g_signal_connect(GTK_OBJECT (GTK_FILE_SELECTION (fix_filsel)),
		     "destroy",
		     GTK_SIGNAL_FUNC(select_cancelled), 
		     NULL);

    g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (fix_filsel)->ok_button),
			      "clicked",
			      G_CALLBACK (select_cancelled), 
			      (gpointer) fix_filsel); 

    g_signal_connect_swapped (GTK_OBJECT (GTK_FILE_SELECTION (fix_filsel)->cancel_button),
			      "clicked",
			      G_CALLBACK (select_cancelled),
			      (gpointer) fix_filsel); 

    gtk_widget_show(fix_filsel);

    open_menu_up=1;
  }
}

void 
clear_fix_pressed( GtkWidget *widget, gpointer data)
{

  gtk_text_buffer_set_text(fix_buffer1, "", 0);

}

void
select_ok( GtkWidget *widget, gpointer data)
{
  char *selected_filename;
  GtkWidget *file_dialog;
  char file_warning[1024];

  open_menu_up=0;

  selected_filename = (char *) gtk_file_selection_get_filename(GTK_FILE_SELECTION(fix_filsel));

  gtk_widget_hide(fix_filsel);
  gtk_widget_destroy(fix_filsel);

  fix_buffer=load_file(selected_filename);
  if(fix_buffer!=NULL) {

    gtk_text_buffer_insert_at_cursor(fix_buffer1, fix_buffer, -1);
    free(fix_buffer);

  } else {

    snprintf(file_warning,1024,"Unable to read: %s",selected_filename);
    file_dialog = gtk_message_dialog_new (GTK_WINDOW(window),
					  GTK_DIALOG_DESTROY_WITH_PARENT,
					  GTK_MESSAGE_ERROR,
					  GTK_BUTTONS_OK,
					  file_warning);
    gtk_dialog_run (GTK_DIALOG (file_dialog));
    gtk_widget_destroy (file_dialog);

  }
}

void
select_cancelled( GtkWidget *widget, gpointer data)
{
  gtk_widget_hide(fix_filsel);
  gtk_widget_destroy(fix_filsel);
  open_menu_up=0;
}


void fill_pr(PROBLEM_REPORT *mypr)
{

  char *ctmp;
  char *from;
  char c;
  int i,j;
  GtkTextBuffer *buffer;
  GtkTextIter beg_iter;
  GtkTextIter end_iter;
  char *temp_cc,*cc_field;

  /* Process CC field */
  mypr->smtp_cc_num=0;

  mypr->smtp_cc_text=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry4));
  cc_field=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry4));

  do {
    temp_cc=strsep(&cc_field,",");
    if(temp_cc!=NULL) {
	
      mypr->smtp_cc[mypr->smtp_cc_num]=temp_cc;
      mypr->smtp_cc_num++;
    }

  } while(temp_cc!=NULL);

  mypr->smtp_server=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry8));
  mypr->smtp_from=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry2));
  mypr->smtp_to=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry1));
  mypr->smtp_rcpt=default_rcpt;
  mypr->smtp_subject=(char *)gtk_entry_get_text(GTK_ENTRY(type_entry1));
  mypr->submitter_id=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry5));
  mypr->originator=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry6));
  mypr->organization=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry7));
  mypr->synopsis=(char *)gtk_entry_get_text(GTK_ENTRY(type_entry1));
  mypr->severity=(char *)gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(type_combo1)->entry));
  mypr->priority=(char *)gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(type_combo2)->entry));
  mypr->category=(char *)gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(type_combo3)->entry));
  mypr->class=(char *)gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(type_combo4)->entry));
  mypr->release=(char *)gtk_entry_get_text(GTK_ENTRY(system_entry1));

  buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(system_view1));
  i=gtk_text_buffer_get_char_count(buffer);
  gtk_text_buffer_get_iter_at_offset(buffer,(GtkTextIter *)&beg_iter,0);
  gtk_text_buffer_get_end_iter(buffer, (GtkTextIter *)&end_iter);
  from=gtk_text_iter_get_visible_text(&beg_iter,&end_iter);

  env_buffer=malloc(2*i+16);
  memset(env_buffer,0,2*i+16);
  ctmp=env_buffer;

  for(j=0;j<i;j++) {

    c=(char )*from++;
    if(c!='\n') {
      *ctmp++=c;
    } else if(c>126|| (c<31 && c!='\n')) {
      break;
    } else {
      *ctmp++='\r';
      *ctmp++=c;
    }

  }

  mypr->environment=env_buffer;

  buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(details_view1));
  i=gtk_text_buffer_get_char_count(buffer);
  gtk_text_buffer_get_iter_at_offset(buffer,(GtkTextIter *)&beg_iter,0);
  gtk_text_buffer_get_end_iter(buffer, (GtkTextIter *)&end_iter);
  from=gtk_text_iter_get_visible_text(&beg_iter,&end_iter);

  desc_buffer=malloc(2*i+16);
  memset(desc_buffer,0,2*i+16);
  ctmp=desc_buffer;

  for(j=0;j<i;j++) {
    c=(char )*from++;

    if(c!='\n') {
      *ctmp++=c;
    } else if(c>126||(c<31 && c!='\n')) {
      break;			
    } else {
      *ctmp++='\r';
      *ctmp++=c;
    }

  }

  mypr->description=desc_buffer;

  buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(details_view2));
  i=gtk_text_buffer_get_char_count(buffer);
  gtk_text_buffer_get_iter_at_offset(buffer,(GtkTextIter *)&beg_iter,0);
  gtk_text_buffer_get_end_iter(buffer, (GtkTextIter *)&end_iter);
  from=gtk_text_iter_get_visible_text(&beg_iter,&end_iter);
        					 
  how_buffer=malloc(2*i+16); /* Let's be convervative here */
  memset(how_buffer,0,2*i+16);
  ctmp=how_buffer;

  for(j=0;j<i;j++) {
    c=(char )*from++;
    if(c!='\n') {

      *ctmp++=c;
    } else if(c>126||(c<31 && c!='\n')) {
      break;
    } else {
      *ctmp++='\r';
      *ctmp++=c;
    }

  }

  mypr->how_to_repeat=how_buffer;

  buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(fix_view));
  i=gtk_text_buffer_get_char_count(buffer);
  gtk_text_buffer_get_iter_at_offset(buffer,(GtkTextIter *)&beg_iter,0);
  gtk_text_buffer_get_end_iter(buffer, (GtkTextIter *)&end_iter);
  from=gtk_text_iter_get_visible_text(&beg_iter,&end_iter);
        	 
  fix_buffer=malloc(2*i+16); /* Let's be convervative here */
  memset(fix_buffer,0,2*i+16);
  ctmp=fix_buffer;
  for(j=0;j<i;j++) {
    c=(char )*from++;
    if(c!='\n') {

      *ctmp++=c;
    } else if(c>126 || (c<31 && c!='\n')) {
      break;
    } else {
      *ctmp++='\r';
      *ctmp++=c;
    }

  }

  mypr->fix=fix_buffer;

}

void 
update_profile(void)
{
  int newsize[2];
  char *tmp_entry;


  gtk_window_get_size(GTK_WINDOW(window),&newsize[0],&newsize[1]);

  my_profile.geom_x=newsize[0];
  my_profile.geom_y=newsize[1];

  tmp_entry=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry2));
  strncpy(my_profile.email,tmp_entry,255);

  tmp_entry=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry6));
  strncpy(my_profile.name,tmp_entry,255);	

  tmp_entry=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry7));
  strncpy(my_profile.org,tmp_entry,255);	

  tmp_entry=(char *)gtk_entry_get_text(GTK_ENTRY(email_entry8));
  strncpy(my_profile.smtp,tmp_entry,255);

}

int
gsp_smtp_auth_dialog(GSP_AUTH *my_auth)
{

  return 0;

}
