#include "config.h"
#include <gnome.h>
#include "setup.h"
#include "stuff.h"
#include "accounts.h"
#include "storeconfig.h"

static GtkWidget* combobox;

/* Callback Functions */
static gint delete_event_cb(GtkWidget* window, GdkEventAny* e, gpointer data);
static void gppp_quit_cb(GtkWidget* widget, gpointer data);
static void gppp_show_setup_dialog_cb(GtkWidget* widget, gpointer data);
static void gppp_setup_ok_clicked_cb(GtkWidget* widget, gpointer data);

static void update_combobox(void);

GtkWidget* gppp_main(void)
{
	GtkWidget* app;
	GtkWidget* connect_button, *setup_button, *quit_button;
	GtkWidget* label;
	GtkWidget* status;
	GtkWidget* h_box, *v_box;
	GtkWidget* separator;
	char buffer[255];
	
	
	sprintf(buffer,"%s %s - %s",PACKAGE, VERSION,_("Gnome PPP Dialer"));	
	
	app = gnome_app_new(PACKAGE, buffer);
	
	h_box = gtk_hbox_new(TRUE,8);
	v_box = gtk_vbox_new(FALSE,10);
    
	label = gtk_label_new(_("Connect to:"));

	combobox = gtk_combo_new();
  
	separator = gtk_hseparator_new();    
  
	connect_button = gppp_button(GNOME_STOCK_PIXMAP_JUMP_TO,_("Connect"));
	setup_button = gppp_button(GNOME_STOCK_PIXMAP_SAVE_AS,_("Setup"));
	quit_button = gppp_button(GNOME_STOCK_PIXMAP_EXIT,_("Quit"));
  
	status = gnome_appbar_new(FALSE, TRUE, GNOME_PREFERENCES_NEVER);

	gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(combobox)->entry), FALSE);
	update_combobox();
	
	gtk_window_set_policy(GTK_WINDOW(app), FALSE, FALSE, FALSE);
	gtk_window_set_wmclass(GTK_WINDOW(app), "gppp", "Gnome PPP Dialer");

	gtk_container_set_border_width(GTK_CONTAINER(v_box),10);
  
	gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.0f);

	gtk_box_pack_start(GTK_BOX(v_box),label,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(v_box),combobox,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(v_box),separator,TRUE,TRUE,6);
	gtk_box_pack_start(GTK_BOX(v_box),h_box,TRUE,TRUE,0);

	gtk_box_pack_start(GTK_BOX(h_box),connect_button,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(h_box),setup_button,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(h_box),quit_button,TRUE,TRUE,0);
  
  
	gnome_app_set_contents(GNOME_APP(app), v_box);

	gnome_app_set_statusbar(GNOME_APP(app), status);

	gtk_signal_connect(GTK_OBJECT(app),
                     "delete_event",
                     GTK_SIGNAL_FUNC(delete_event_cb),
                     NULL);
	gtk_signal_connect(GTK_OBJECT(quit_button),
  					"clicked",
  					GTK_SIGNAL_FUNC(gppp_quit_cb),
  					NULL);
	gtk_signal_connect(GTK_OBJECT(setup_button),
  					"clicked",
  					GTK_SIGNAL_FUNC(gppp_show_setup_dialog_cb),
  					NULL);

	return app;
}

static gint delete_event_cb(GtkWidget* window, GdkEventAny* e, gpointer data)
{
	gppp_quit_cb(NULL,NULL);
	return TRUE;
}

static void gppp_quit_cb(GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

static void gppp_show_setup_dialog_cb(GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog;
	gint answer;
	
	dialog = gppp_setup_new();
	gppp_setup_set_data();
	gnome_dialog_button_connect (GNOME_DIALOG(dialog),0,
								GTK_SIGNAL_FUNC(gppp_setup_ok_clicked_cb),NULL);
	answer = gnome_dialog_run_and_close(GNOME_DIALOG(dialog));
}

static void gppp_setup_ok_clicked_cb(GtkWidget* widget, gpointer data)
{
	gppp_store_config(accounts);
	update_combobox();	
}

static void update_combobox(void)
{
	gchar* text;
	GList *list;
	gint i, max;
	struct Account* account;
	text = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(combobox)->entry));
	
	list = NULL;
	
	max = g_list_length(accounts);
	
	for (i=0;i<max;i++)
	{
		account = g_list_nth_data(accounts, i);
		list = g_list_append(list, account->name);
	}
	
	gtk_combo_set_popdown_strings(GTK_COMBO(combobox), list);
}