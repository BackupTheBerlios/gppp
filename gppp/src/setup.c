#include "config.h"
#include <gnome.h>
#include "stuff.h"
#include "accounts.h"
#include "setup.h"

#define DEV_NR 7 	/* Nr of Devices in the devs[] array */
#define SPEED_NR 8	/* Nr of "Speeds" in the speeds[] array */

/* Some vars */
static char* devs[] = {"/dev/modem", "/dev/ttyS0", "/dev/ttyS1", "/dev/ttyS2", "/dev/ttyS3",
							"/dev/ttyl0", "/dev/ttyl1", "/dev/ttyl2", "/dev/ttyl3"};
static char* speeds[] = {"2400", "9600", "19200", "38400", "57600", "115200", "230400","460800"};
static char* flows[] = {"CRTSCTS","XON/XOF","None"};
static char* terms[] = {"CR","LF","CR/LF"}; 

static gint selected_row;


/* global Widgets form the "Accounts"-Frame */
static GtkWidget* clist;
static GtkWidget* new_button, *edit_button, *del_button, *cop_button;


/* For every tab in the account dialog there is a function, which returns the frame that will be 
put in the tab*/
static GtkWidget* gppp_setup_accounts_frame(void);
static GtkWidget* gppp_setup_device_frame(void);
static GtkWidget* gppp_setup_misc_frame(void);

/* Some callback functions */
static void new_button_clicked_cb(GtkWidget *widget, gpointer data);
static void edit_button_clicked_cb(GtkWidget *widget, gpointer data);
static void del_button_clicked_cb(GtkWidget *widget, gpointer data);
static void copy_button_clicked_cb(GtkWidget *widget, gpointer data);
static void gppp_account_new_ok_clicked_cb(GtkWidget *widget, gpointer data);
static void gppp_account_edit_ok_clicked_cb(GtkWidget *widget, gpointer data);
static void clist_select_row_cb(GtkWidget* widget, gint row, gint col, 
				GdkEventButton* event, gpointer data);
static void clist_unselect_row_cb(GtkWidget* widget, gint row, gint col, 
				GdkEventButton* event, gpointer data);

/* The setup dialog */
GtkWidget* gppp_setup_new(void)
{

	GtkWidget* dialog;
	GtkWidget* notebook;
	GtkWidget* accounts_frame, *device_frame, *misc_frame;
	GtkWidget* accounts_label, *device_label, *misc_label;
  
	dialog = GTK_WIDGET(gnome_dialog_new(_("Gnome PPP Dialer - Setup"),
  							GNOME_STOCK_BUTTON_OK,
  							GNOME_STOCK_BUTTON_CANCEL,
  							NULL));
  							
	notebook = gtk_notebook_new();
	
	accounts_label = gtk_label_new(_("Accounts"));
	device_label = gtk_label_new(_("Device"));
	misc_label = gtk_label_new(_("Misc"));
	
	accounts_frame = gppp_setup_accounts_frame();
	device_frame = gppp_setup_device_frame();
	misc_frame = gppp_setup_misc_frame();

	
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), accounts_frame, accounts_label);
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), device_frame, device_label);
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), misc_frame, misc_label);


	gtk_widget_show_all(notebook);
	gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(dialog)->vbox),notebook,TRUE,TRUE,0);	
	
	return dialog;
}
/* end of the setup dialog */


/* the "accounts"-frame */
static GtkWidget* gppp_setup_accounts_frame(void)
{
	GtkWidget* frame;
	GtkWidget* h_box, *v_box1, *v_box2;
	GtkWidget* scrolled_window;
	GtkWidget* label;
	
	frame = gtk_frame_new(_("Accounts Settings:"));
	
	h_box = gtk_hbox_new(FALSE,4);
	v_box1 = gtk_vbox_new(FALSE,0);	
	v_box2 = gtk_vbox_new(TRUE,8);
	
	label = gtk_label_new("Accounts:");
	
	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	clist = gtk_clist_new(1);
	
	new_button = gppp_button(GNOME_STOCK_PIXMAP_NEW,(_("New")));
	edit_button = gppp_button(GNOME_STOCK_PIXMAP_PROPERTIES,(_("Edit")));
	del_button = gppp_button(GNOME_STOCK_PIXMAP_CLEAR,(_("Delete")));
	cop_button = gppp_button(GNOME_STOCK_PIXMAP_COPY,(_("Copy")));
	
	gtk_widget_set_sensitive(GTK_WIDGET(edit_button), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(del_button), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(cop_button), FALSE);

	gtk_container_set_border_width(GTK_CONTAINER(frame),3);
	gtk_container_set_border_width(GTK_CONTAINER(h_box),8);
	gtk_container_add(GTK_CONTAINER(frame),h_box);

	/* Des brauch ich glau net */
	//gtk_clist_set_reorderable(GTK_CLIST(clist),TRUE);
	
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                             GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gtk_container_add(GTK_CONTAINER(scrolled_window), clist);

	gtk_clist_set_column_width (GTK_CLIST(clist), 0, 100);
	
	gtk_misc_set_alignment(GTK_MISC(label), 0.0f, 0.0f);
	
	gtk_box_pack_start(GTK_BOX(h_box),v_box1,TRUE,TRUE,4);
	gtk_box_pack_start(GTK_BOX(h_box),v_box2,FALSE,FALSE,4);
	
	gtk_box_pack_start(GTK_BOX(v_box1),label,FALSE,FALSE,4);
	gtk_box_pack_start(GTK_BOX(v_box1),scrolled_window,TRUE,TRUE,4);
	
	gtk_box_pack_start(GTK_BOX(v_box2),new_button,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(v_box2),edit_button,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(v_box2),del_button,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(v_box2),cop_button,TRUE,TRUE,0);
	

	gtk_signal_connect(GTK_OBJECT(new_button), 
						"clicked",
						GTK_SIGNAL_FUNC(new_button_clicked_cb),
						NULL);
	gtk_signal_connect(GTK_OBJECT(edit_button), 
						"clicked",
						GTK_SIGNAL_FUNC(edit_button_clicked_cb),
						NULL);
	gtk_signal_connect(GTK_OBJECT(del_button), 
						"clicked",
						GTK_SIGNAL_FUNC(del_button_clicked_cb),
						NULL);
	gtk_signal_connect(GTK_OBJECT(cop_button), 
						"clicked",
						GTK_SIGNAL_FUNC(copy_button_clicked_cb),
						NULL);
	gtk_signal_connect(GTK_OBJECT(clist),
						"select-row",
						GTK_SIGNAL_FUNC(clist_select_row_cb),
						NULL);
	gtk_signal_connect(GTK_OBJECT(clist),
						"unselect-row",
						GTK_SIGNAL_FUNC(clist_unselect_row_cb),
						NULL);


	gtk_widget_show_all(frame);
	
	return frame;			

}
/* end of the "accounts"-frame */


/* the "device"-frame */
static GtkWidget* gppp_setup_device_frame(void)
{
	GtkWidget* frame;
	GtkWidget* table;
	GtkWidget* dev_label, *flow_label, *term_label, *speed_label, *vol_label;
	GtkWidget* dev_combo, *flow_combo, *term_combo, *speed_combo;
	GtkWidget* vol_slide;
	GtkObject* vol_adj;
	GList* dev_list, *flow_list, *term_list, *speed_list;	
	int i;
	
	frame = gtk_frame_new(_("Device Settings:"));
	
	table = gtk_table_new(2, 5, FALSE);
	
	dev_label = gtk_label_new(_("Device:"));
	flow_label = gtk_label_new(_("Flow Control:"));
	term_label = gtk_label_new(_("Line Termination:"));
	speed_label = gtk_label_new(_("Device Speed:"));
	vol_label = gtk_label_new(_("Volume:"));
	
	dev_combo = gtk_combo_new();
	flow_combo = gtk_combo_new();
	term_combo = gtk_combo_new();
	speed_combo = gtk_combo_new();
	
	vol_adj = gtk_adjustment_new(2.0, 0.0, 3.0, 1.0, 1.0, 0.0);
	vol_slide = gtk_hscale_new(GTK_ADJUSTMENT(vol_adj));

	dev_list = NULL;
	for (i=0;i<DEV_NR;i++) 
		dev_list = g_list_append(dev_list, devs[i]);
	flow_list = NULL;
	for (i=0;i<3;i++) 
		flow_list = g_list_append(flow_list, flows[i]);
	term_list = NULL;
	for (i=0;i<3;i++) 
		term_list = g_list_append(term_list, terms[i]);
	speed_list = NULL;
	for (i=0;i<SPEED_NR;i++) 
		speed_list = g_list_append(speed_list, speeds[i]);

	gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(dev_combo)->entry),FALSE);
	gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(flow_combo)->entry),FALSE);
	gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(term_combo)->entry),FALSE);
	gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(speed_combo)->entry),FALSE);

	gtk_combo_set_popdown_strings (GTK_COMBO (dev_combo), dev_list);
	gtk_combo_set_popdown_strings (GTK_COMBO (flow_combo), flow_list);
	gtk_combo_set_popdown_strings (GTK_COMBO (term_combo), term_list);
	gtk_combo_set_popdown_strings (GTK_COMBO (speed_combo), speed_list);

	gtk_scale_set_digits (GTK_SCALE(vol_slide),0);
	
	gtk_misc_set_alignment(GTK_MISC(dev_label), 0.0f, 0.0f);
	gtk_misc_set_alignment(GTK_MISC(flow_label), 0.0f, 0.0f);
	gtk_misc_set_alignment(GTK_MISC(term_label), 0.0f, 0.0f);
	gtk_misc_set_alignment(GTK_MISC(speed_label), 0.0f, 0.0f);
	gtk_misc_set_alignment(GTK_MISC(vol_label), 0.0f, 0.0f);

	gtk_container_add(GTK_CONTAINER(frame),table);
	gtk_container_set_border_width(GTK_CONTAINER(frame),3);
	gtk_container_set_border_width(GTK_CONTAINER(table),8);

	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	gtk_table_set_col_spacing(GTK_TABLE(table),0,10);

	gtk_table_attach_defaults(GTK_TABLE(table),dev_label,0,1,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),dev_combo,1,2,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),flow_label,0,1,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),flow_combo,1,2,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),term_label,0,1,2,3);
	gtk_table_attach_defaults(GTK_TABLE(table),term_combo,1,2,2,3);
	gtk_table_attach_defaults(GTK_TABLE(table),speed_label,0,1,3,4);
	gtk_table_attach_defaults(GTK_TABLE(table),speed_combo,1,2,3,4);
	gtk_table_attach_defaults(GTK_TABLE(table),vol_label,0,1,4,5);
	gtk_table_attach_defaults(GTK_TABLE(table),vol_slide,1,2,4,5);


/*	gtk_signal_connect(GTK_OBJECT(vol_adj),"value-changed",
			GTK_SIGNAL_FUNC(combochanged),NULL);
*/	
	//gtk_widget_show_all(frame);

	return frame;
	
}
/* end of the "device"-frame */


/* the "misc"-frame */
static GtkWidget* gppp_setup_misc_frame(void)
{
	GtkWidget* frame;

	frame = gtk_frame_new(_("Misc. Settings:"));

	gtk_container_set_border_width(GTK_CONTAINER(frame),3);

	return frame;
}
/* end of the "misc"-frame */

/* the callback functions */

static void new_button_clicked_cb(GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog;
	gint answer;
	
	dialog = gppp_account_new();
	gnome_dialog_button_connect (GNOME_DIALOG(dialog),0,
								GTK_SIGNAL_FUNC(gppp_account_new_ok_clicked_cb),NULL);
	answer = gnome_dialog_run_and_close(GNOME_DIALOG(dialog));
	
}

static void edit_button_clicked_cb(GtkWidget *widget, gpointer data)
{
	GtkWidget *dialog;
	gint answer;
	struct Account* account;
	
	dialog = gppp_account_new();
	account = g_list_nth_data(accounts, selected_row);
	
	g_assert(account != NULL);
		
	gppp_account_set_data(account);
	gnome_dialog_button_connect (GNOME_DIALOG(dialog),0,
								GTK_SIGNAL_FUNC(gppp_account_edit_ok_clicked_cb),NULL);
	answer = gnome_dialog_run_and_close(GNOME_DIALOG(dialog));
}

static void del_button_clicked_cb(GtkWidget *widget, gpointer data)
{
	g_print("FIXME\n");
}

static void copy_button_clicked_cb(GtkWidget *widget, gpointer data)
{
	g_print("FIXME\n");
}

static void gppp_account_new_ok_clicked_cb(GtkWidget *widget, gpointer data)
{
	gchar* text[1];
	struct Account* account;
	
	account = gppp_account_get_data();
	
	account->new = TRUE;
	account->old_data = NULL;

	g_assert(account != NULL);
	
	accounts = g_list_append(accounts, account);
	text[0] = account->name;
	gtk_clist_append(GTK_CLIST(clist), text);
}			

static void gppp_account_edit_ok_clicked_cb(GtkWidget *widget, gpointer data)
{
	struct Account* account, *tmp;
	GList* element;

	account = gppp_account_get_data();
	tmp = g_list_nth_data(accounts, selected_row);

	g_assert(account != NULL);
	g_assert(tmp != NULL);
	
	element = g_list_find(accounts, tmp);
	
	if (tmp->old_data != NULL)
	{
		account->old_data = tmp->old_data;
		gppp_account_free(tmp);
		g_free(tmp);
	}
	else account->old_data = tmp;
	if (tmp->new) account->new = TRUE; 
	element->data = account;
	gtk_clist_set_text(GTK_CLIST(clist), selected_row, 0, account->name);
			
}			

static void clist_select_row_cb(GtkWidget* widget, gint row, gint col, 
			GdkEventButton* event, gpointer data)
{
	selected_row = row;
	gtk_widget_set_sensitive(GTK_WIDGET(edit_button), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(del_button), TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(cop_button), TRUE);
}

static void clist_unselect_row_cb(GtkWidget* widget, gint row, gint col, 
			GdkEventButton* event, gpointer data)
{
	selected_row = -1;
	gtk_widget_set_sensitive(GTK_WIDGET(edit_button), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(del_button), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(cop_button), FALSE);
}

/* end of the callback functions */

void gppp_setup_set_data(void)
{
	gint i, max;
	gchar *text[1];
	struct Account *account;
	
	max = g_list_length(accounts);
	
	for(i=0;i<max;i++)
	{
		account = g_list_nth_data(accounts, i);
		g_assert(account != NULL);
		text[0] = account->name;
		gtk_clist_append(GTK_CLIST(clist), text);
	}
}