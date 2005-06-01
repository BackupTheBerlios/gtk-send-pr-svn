/*
Copyright (c) 2003, Miguel Mendez. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer. 
	* Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation 
	and/or other materials provided with the distribution. 
	* Neither the name of Miguel Mendez nor the names of his contributors
	may be used to endorse or promote products derived from this software 
	without specific prior written permission.

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
#include <unistd.h>

#include <time.h>
extern char *tzname[2];

#include <gtk/gtk.h>

//#include "gtk-send-pr.h"
#include "gtk_ui.h"

gint delete_event( GtkWidget *, GdkEvent *, gpointer);
void destroy( GtkWidget *, gpointer);


		int dirty;
		GtkWidget *window;
		
int 
create_gtk_ui(void)
{
		
		GtkWidget *send_button;
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
		
		GtkTooltips *send_tip;
		GtkTooltips *quit_tip;
		GtkTooltips *about_tip;
		GtkTooltips *help_tip;


	dirty=1;
	/* Define main window */
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW (window), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable((GtkWindow *)window,TRUE);
	gtk_window_set_title(GTK_WINDOW(window),"GTK Send-PR");


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

	gtk_box_pack_start((GtkBox *)h_buttons, send_button, FALSE, FALSE, 0);
	gtk_box_pack_start((GtkBox *)h_buttons, about_button, FALSE, FALSE, 0);
	gtk_box_pack_start((GtkBox *)h_buttons, help_button, FALSE, FALSE, 0);
	gtk_box_pack_start((GtkBox *) h_buttons, quit_button, FALSE, FALSE, 0);


	g_signal_connect(GTK_OBJECT(window), "delete_event",\
		      GTK_SIGNAL_FUNC(delete_event), NULL);

	g_signal_connect(GTK_OBJECT(window), "destroy",\
		      GTK_SIGNAL_FUNC(destroy), NULL);


	vbox1 = gtk_vbox_new (FALSE, 2);
	hseparator1 = gtk_hseparator_new ();
	
	tab_email=gtk_label_new("E-Mail");
	tab_type=gtk_label_new("Type");
	tab_system=gtk_label_new("System");
	tab_descr=gtk_label_new("Details");

	mynotebook=gtk_notebook_new();
	
//	gtk_notebook_append_page((GtkNotebook *)mynotebook,scrolled_window1,tab_bools);

	gtk_box_pack_start(GTK_BOX (vbox1), mynotebook, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX (vbox1), hseparator1, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (vbox1), h_buttons, FALSE, FALSE, 0);
	gtk_box_set_homogeneous((GtkBox *)vbox1,FALSE);
	gtk_container_add(GTK_CONTAINER (window), vbox1);

	gtk_widget_show(mynotebook);
	gtk_widget_show(vbox1);
	
	
	gtk_widget_show(hseparator1);
	gtk_widget_show(h_buttons);
	gtk_widget_show(send_button);
	gtk_widget_show(quit_button);
	gtk_widget_show(about_button);
	gtk_widget_show(help_button);
//	gtk_widget_show (scrolled_window2);
//	gtk_widget_show (mytable2);
//	gtk_widget_show (scrolled_window1);
//	gtk_widget_show (mytable1);
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

	dialog = gtk_message_dialog_new ((GtkWindow *)window,
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_WARNING,
                                  GTK_BUTTONS_YES_NO,
                                  "There are unsaved changes, quit anyway?");
	result=gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);

	} else {
	
		return FALSE;
	
	}
	
	if(result==GTK_RESPONSE_YES) {
	
    	return FALSE;

	} else {
	
		return TRUE;
	}

}

void 
destroy( GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}
