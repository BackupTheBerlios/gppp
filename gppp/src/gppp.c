#include "config.h"
#include <gnome.h>
#include "gppp_main.h"
#include "setup.h"
#include "storeconfig.h"

static gint session_die(GnomeClient* client, gpointer client_data);

static gint save_session(GnomeClient *client, gint phase, 
                         GnomeSaveStyle save_style,
                         gint is_shutdown, GnomeInteractStyle interact_style,
                         gint is_fast, gpointer client_data);

int main(int argc, char* argv[])
{
	GtkWidget* app;
  
	GnomeClient* client;

	bindtextdomain(PACKAGE, GNOMELOCALEDIR);  
	textdomain(PACKAGE);

	gnome_init(PACKAGE, VERSION, argc, argv);  

	/* Session Management */
  
	client = gnome_master_client ();
	gtk_signal_connect (GTK_OBJECT (client), "save_yourself",
                      GTK_SIGNAL_FUNC (save_session), argv[0]);
	gtk_signal_connect (GTK_OBJECT (client), "die",
                      GTK_SIGNAL_FUNC (session_die), NULL);

	
	/* Load configuration */
	accounts = gppp_load_config();
	
	/* Main app */
	app = gppp_main();
	gtk_widget_show_all(app);

	gtk_main();
	
	return 0;
}

static gint save_session (GnomeClient *client, gint phase, GnomeSaveStyle save_style,
              gint is_shutdown, GnomeInteractStyle interact_style,
              gint is_fast, gpointer client_data)
{
	gchar** argv;
	guint argc;

	argv = g_malloc0(sizeof(gchar*)*4);
	argc = 1;

	argv[0] = client_data;

	gnome_client_set_clone_command (client, argc, argv);
	gnome_client_set_restart_command (client, argc, argv);

	return TRUE;
}


static gint session_die(GnomeClient* client, gpointer client_data)
{
	gtk_main_quit ();
	return TRUE;
}


