#include "config.h"
#include <gnome.h>
#include "stuff.h"
#include "accounts.h"

#define COMMANDS_NR 16 /* NR of Commands in the commands-array */

/* Some vars */

static gchar* commands[] = {"Expect", "Send", "Pause (sec.)", "Hangup", "Answer", "Timeout (sec.)", 
	"Password", "ID", "Prompt", "PWPrompt", "LoopStart", "LoopEnd", "Scan", "Save", "Mode", "SendNoEcho"};

static gchar* times[] = {N_("hour"),N_("minute"),N_("second")};

static gint selected_row;

/* For every tab in the account dialog there is a function, which returns the frame that will be 
put in the tab*/
static GtkWidget* gppp_accounts_general_frame(void);
static GtkWidget* gppp_accounts_ip_frame(void);
static GtkWidget* gppp_accounts_dns_frame(void);
static GtkWidget* gppp_accounts_script_frame(void);
static GtkWidget* gppp_accounts_misc_frame(void);
static GtkWidget* gppp_accounts_costs_frame(void);

/* These are globals so that I can the data input */

/* The widgets for the "General"-Tab */
static GtkWidget* savepwd_check, *usescript_check;
static GtkWidget* name_entry, *phone_entry, *user_entry, *pwd_entry;

/* The widgets for the "IP"-Tab */
static GtkWidget* ip_entry, *mask_entry;
static GtkWidget* dyn_radbutton, *sta_radbutton;

/* The widgets for the "DNS"-Tab */
static GtkWidget* primdns_entry, *secdns_entry; 
static GtkWidget* domain_entry; 
static GtkWidget* auto_radbutton, *man_radbutton;
static GtkWidget* disableolddns_cb;

/* The widgets for the "Script"-Tab */
static GtkWidget* key_combo;
static GtkWidget* value_entry;
static GtkWidget* clist;

/* The widgets for the "Misc"-Tab */
static GtkWidget* exe1_entry, *exe2_entry;

/* The widgets for the "Costs"-Tab */
static GtkWidget* cost_entry, *time_combo;
static GtkWidget* usecosts_cb;

/* End of widget-declaration */


/* Some labels that are enabled/disabled by callback functions and therefor must be global*/
static GtkWidget* ip_label, *mask_label, *primdns_label, *secdns_label, 
				*cost_label, *time_label, *pwd_label;


/* The Callback-functions */
static void dyn_radbutton_toggled_cb(GtkWidget* widget, gpointer data);
static void auto_radbutton_toggled_cb(GtkWidget* widget, gpointer data);
static void usecosts_cb_toggled_cb(GtkWidget* widget, gpointer data);
static void savepwd_cb_toggled_cb(GtkWidget* widget, gpointer data);
static void add_button_clicked_cb(GtkWidget* widget, gpointer data);
static void del_button_clicked_cb(GtkWidget* widget, gpointer data);
static void clist_select_row_cb(GtkWidget* widget, gint row, gint col, 
				GdkEventButton* event, gpointer data);
static void clist_unselect_row_cb(GtkWidget* widget, gint row, gint col, 
				GdkEventButton* event, gpointer data);
/* End of Callback-function Declaration */



/* The account Dialog */
GtkWidget* gppp_account_new(void)
{

	GtkWidget* dialog;
	GtkWidget* notebook;
	GtkWidget* general_frame, *ip_frame, *dns_frame, *script_frame, *misc_frame, *costs_frame;
	GtkWidget* general_label, *ip_label, *dns_label, *script_label, *misc_label, *costs_label;
  
	dialog = GTK_WIDGET(gnome_dialog_new(_("Gnome PPP Dialer - Acount Setup"),
  							GNOME_STOCK_BUTTON_OK,
  							GNOME_STOCK_BUTTON_CANCEL,
  							NULL));
	
	notebook = gtk_notebook_new();
	
	general_label = gtk_label_new(_("General"));
	ip_label = gtk_label_new(_(" IP "));
	dns_label = gtk_label_new(_("DNS"));
	script_label = gtk_label_new(_("Script"));
	misc_label = gtk_label_new(_("Misc"));
	costs_label = gtk_label_new(_("Costs"));
	
	general_frame = gppp_accounts_general_frame();
	ip_frame = gppp_accounts_ip_frame();
	dns_frame = gppp_accounts_dns_frame();
	script_frame = gppp_accounts_script_frame();
	misc_frame = gppp_accounts_misc_frame();
	costs_frame = gppp_accounts_costs_frame();

	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), general_frame, general_label);
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), ip_frame, ip_label);
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), dns_frame, dns_label);
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), script_frame, script_label);
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), misc_frame, misc_label);
	gtk_notebook_append_page (GTK_NOTEBOOK(notebook), costs_frame, costs_label);

	gtk_widget_show_all(notebook);
	gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(dialog)->vbox),notebook,TRUE,TRUE,0);	
	
	return dialog;
}

/* End of accounts Dialog */


/* The "General"-Tab frame */

static GtkWidget* gppp_accounts_general_frame(void)
{
	GtkWidget* frame;
	GtkWidget* name_label, *phone_label, *user_label;
	GtkWidget* table;
	
	frame = gtk_frame_new(_("General Settings:"));

	table = gtk_table_new(2,6,FALSE);
	
	name_label = gtk_label_new(_("Name of the account:"));
	phone_label = gtk_label_new(_("Phonenumber:"));
	user_label = gtk_label_new(_("Username:"));
	pwd_label = gtk_label_new(_("Password:"));
	
	name_entry = gtk_entry_new();	
	phone_entry = gtk_entry_new();	
	user_entry = gtk_entry_new();	
	pwd_entry = gtk_entry_new();	
		
	savepwd_check = gtk_check_button_new_with_label(_("Save Password"));
	usescript_check = gtk_check_button_new_with_label(_("Use Login Script"));
	
	if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(savepwd_check)))
		{
			gtk_widget_set_sensitive(pwd_label,FALSE);
			gtk_widget_set_sensitive(pwd_entry,FALSE);
		}
	
	gtk_misc_set_alignment(GTK_MISC(name_label), 0.0f, 0.5f);
	gtk_misc_set_alignment(GTK_MISC(phone_label), 0.0f, 0.5f);
	gtk_misc_set_alignment(GTK_MISC(user_label), 0.0f, 0.5f);
	gtk_misc_set_alignment(GTK_MISC(pwd_label), 0.0f, 0.5f);

	gtk_entry_set_visibility(GTK_ENTRY(pwd_entry),FALSE);
	
	gtk_container_add(GTK_CONTAINER(frame),table);
	gtk_container_set_border_width(GTK_CONTAINER(frame),3);
	gtk_container_set_border_width(GTK_CONTAINER(table),8);

	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	gtk_table_set_col_spacing(GTK_TABLE(table),0,10);
	
	gtk_table_attach_defaults(GTK_TABLE(table),name_label,0,1,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),name_entry,1,2,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),phone_label,0,1,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),phone_entry,1,2,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),user_label,0,1,2,3);
	gtk_table_attach_defaults(GTK_TABLE(table),user_entry,1,2,2,3);
	gtk_table_attach_defaults(GTK_TABLE(table),pwd_label,0,1,3,4);
	gtk_table_attach_defaults(GTK_TABLE(table),pwd_entry,1,2,3,4);
	gtk_table_attach_defaults(GTK_TABLE(table),savepwd_check,0,2,4,5);
	gtk_table_attach_defaults(GTK_TABLE(table),usescript_check,0,2,5,6);

	gtk_signal_connect(GTK_OBJECT(savepwd_check),
						"toggled",
						GTK_SIGNAL_FUNC(savepwd_cb_toggled_cb),
						NULL);
	
	return frame;
}

/* End of the "General"-Tab frame */


/* The "IP"-Tab frame */

static GtkWidget* gppp_accounts_ip_frame(void)
{
	GtkWidget* frame;
	GtkWidget* table;
	GtkWidget* box;
	
	GSList* group;
	
	frame = gtk_frame_new(_("IP-Adress Settings:"));
	
	box = gtk_vbox_new(FALSE,0);

	table = gtk_table_new(2, 4, FALSE);
	
	dyn_radbutton = gtk_radio_button_new_with_label(NULL,(_("Dynamic IP Address")));
	group = gtk_radio_button_group(GTK_RADIO_BUTTON(dyn_radbutton));
	sta_radbutton = gtk_radio_button_new_with_label(group,(_("Static IP Address")));
	group = gtk_radio_button_group(GTK_RADIO_BUTTON(sta_radbutton));
	
	ip_label = gtk_label_new(_("IP-Adresse:"));
	mask_label = gtk_label_new(_("Subnet Mask:"));
	
	ip_entry = gtk_entry_new();
	mask_entry = gtk_entry_new();

	gtk_misc_set_alignment(GTK_MISC(ip_label), 0.0f, 0.5f);
	gtk_misc_set_alignment(GTK_MISC(mask_label), 0.0f, 0.5f);
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(dyn_radbutton)))
	{
		gtk_widget_set_sensitive(ip_label, FALSE);
		gtk_widget_set_sensitive(mask_label, FALSE);
		gtk_widget_set_sensitive(ip_entry, FALSE);
		gtk_widget_set_sensitive(mask_entry, FALSE);
	}

	gtk_box_pack_start(GTK_BOX(box),table,FALSE,FALSE,0);

	gtk_container_add(GTK_CONTAINER(frame),box);
	gtk_container_set_border_width(GTK_CONTAINER(frame),3);
	gtk_container_set_border_width(GTK_CONTAINER(table),8);

	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	gtk_table_set_col_spacing(GTK_TABLE(table),0,10);
	
	gtk_table_attach_defaults(GTK_TABLE(table),dyn_radbutton,0,2,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),sta_radbutton,0,2,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),ip_label,0,1,2,3);
	gtk_table_attach_defaults(GTK_TABLE(table),ip_entry,1,2,2,3);
	gtk_table_attach_defaults(GTK_TABLE(table),mask_label,0,1,3,4);
	gtk_table_attach_defaults(GTK_TABLE(table),mask_entry,1,2,3,4);

	gtk_signal_connect(GTK_OBJECT(dyn_radbutton),
						"toggled",
						GTK_SIGNAL_FUNC(dyn_radbutton_toggled_cb),
						NULL);
						

	return frame;
}

/* End of the "IP"-Tab frame */


/* The "DNS"-Tab frame */

static GtkWidget* gppp_accounts_dns_frame(void)
{
	GtkWidget* frame;
	GtkWidget* table;
	GtkWidget* domain_label, *conf_label; 
	GSList* group;

	frame = gtk_frame_new(_("DNS Settings:"));
	
	table = gtk_table_new(2,6,FALSE);

	domain_label = gtk_label_new(_("Domain Name:"));
	primdns_label = gtk_label_new(_("Primary DNS Server:"));
	secdns_label = gtk_label_new(_("Secondary DNS Server:"));
	conf_label = gtk_label_new(_("Configuration:"));
	
	domain_entry = gtk_entry_new();
	primdns_entry = gtk_entry_new();
	secdns_entry = gtk_entry_new();

	auto_radbutton = gtk_radio_button_new_with_label(NULL,(_("automatic")));
	group = gtk_radio_button_group(GTK_RADIO_BUTTON(auto_radbutton));
	man_radbutton = gtk_radio_button_new_with_label(group,(_("manual")));
	group = gtk_radio_button_group(GTK_RADIO_BUTTON(man_radbutton));
	
	disableolddns_cb = gtk_check_button_new_with_label(_("Disable existing DNS Servers during Connection"));
	
	gtk_misc_set_alignment(GTK_MISC(domain_label), 0.0f, 0.5f);
	gtk_misc_set_alignment(GTK_MISC(primdns_label), 0.0f, 0.5f);
	gtk_misc_set_alignment(GTK_MISC(secdns_label), 0.0f, 0.5f);
	gtk_misc_set_alignment(GTK_MISC(conf_label), 0.0f, 0.5f);
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(auto_radbutton)))
	{
		gtk_widget_set_sensitive(primdns_label, FALSE);
		gtk_widget_set_sensitive(secdns_label, FALSE);
		gtk_widget_set_sensitive(primdns_entry, FALSE);
		gtk_widget_set_sensitive(secdns_entry, FALSE);
	}

	gtk_container_add(GTK_CONTAINER(frame),table);
	gtk_container_set_border_width(GTK_CONTAINER(frame),3);
	gtk_container_set_border_width(GTK_CONTAINER(table),8);

	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	gtk_table_set_col_spacing(GTK_TABLE(table),0,10);
	
	gtk_table_attach_defaults(GTK_TABLE(table),domain_label,0,1,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),domain_entry,1,2,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),conf_label,0,1,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),auto_radbutton,1,2,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),man_radbutton,1,2,2,3);
	gtk_table_attach_defaults(GTK_TABLE(table),primdns_label,0,1,3,4);
	gtk_table_attach_defaults(GTK_TABLE(table),primdns_entry,1,2,3,4);
	gtk_table_attach_defaults(GTK_TABLE(table),secdns_label,0,1,4,5);
	gtk_table_attach_defaults(GTK_TABLE(table),secdns_entry,1,2,4,5);
	gtk_table_attach_defaults(GTK_TABLE(table),disableolddns_cb,0,2,5,6);

	gtk_signal_connect(GTK_OBJECT(auto_radbutton),
						"toggled",
						GTK_SIGNAL_FUNC(auto_radbutton_toggled_cb),
						NULL);
	
	return frame;
}

/* End of the "DNS"-Tab frame */


/* The "Script"-Tab frame */

static GtkWidget* gppp_accounts_script_frame(void)
{
	GtkWidget* frame;
	GtkWidget* h_box, *v_box;
	GtkWidget* button_box;
	GtkWidget* add_button, *del_button;
	GtkWidget* scrolled_window;
	GList* list;
	int i;
	
	selected_row = -1;
	
	frame = gtk_frame_new(_("Script Settings:"));

	v_box = gtk_vbox_new(FALSE,5);
	
	h_box = gtk_hbox_new(FALSE,5);
	
	key_combo = gtk_combo_new();		

	value_entry = gtk_entry_new();
	
	button_box = gtk_hbutton_box_new();

	add_button = gppp_button(GNOME_STOCK_PIXMAP_ADD, _("Add"));
	del_button = gppp_button(GNOME_STOCK_PIXMAP_CLEAR, _("Delete"));
		
	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	
	clist = gtk_clist_new(2);
	
	gtk_clist_set_reorderable(GTK_CLIST(clist),TRUE);
	
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                             GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

	gtk_container_add(GTK_CONTAINER(scrolled_window), clist);

	gtk_clist_set_column_width(GTK_CLIST(clist),0,100);
	gtk_clist_set_column_width(GTK_CLIST(clist),1,100);
	
	gtk_box_pack_start(GTK_BOX(v_box),h_box, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(v_box),button_box, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(v_box),scrolled_window, TRUE, TRUE, 0);

	gtk_box_pack_start(GTK_BOX(h_box),key_combo, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(h_box),value_entry, FALSE, FALSE, 0);

	gtk_box_pack_start(GTK_BOX(button_box), add_button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(button_box), del_button, FALSE, FALSE, 0);

	list = NULL;
	for (i=0;i<COMMANDS_NR;i++)
		list = g_list_append(list,commands[i]);
	
	gtk_combo_set_popdown_strings(GTK_COMBO(key_combo),list);
	
	gtk_widget_set_sensitive(del_button, FALSE);
	
	gtk_container_add(GTK_CONTAINER(frame),v_box);
	gtk_container_set_border_width(GTK_CONTAINER(v_box),5);
	gtk_container_set_border_width(GTK_CONTAINER(frame),3);

	gtk_signal_connect(GTK_OBJECT(add_button),
						"clicked",
						GTK_SIGNAL_FUNC(add_button_clicked_cb),
						NULL);

	gtk_signal_connect(GTK_OBJECT(del_button),
						"clicked",
						GTK_SIGNAL_FUNC(del_button_clicked_cb),
						NULL);
						
	gtk_signal_connect(GTK_OBJECT(clist),
						"select-row",
						GTK_SIGNAL_FUNC(clist_select_row_cb),
						(gpointer) del_button);

	gtk_signal_connect(GTK_OBJECT(clist),
						"unselect-row",
						GTK_SIGNAL_FUNC(clist_unselect_row_cb),
						(gpointer) del_button);
						
	return frame;
}

/* End of the "Script"-Tab frame */


/* The "Misc"-Tab frame */

static GtkWidget* gppp_accounts_misc_frame(void)
{
	GtkWidget* frame;
	GtkWidget* box;
	GtkWidget* table;
	GtkWidget* exe1_label, *exe2_label;
	
	frame = gtk_frame_new(_("Misc. Settings:"));

	box = gtk_vbox_new(FALSE,0);

	table = gtk_table_new(2,2,FALSE);
	
	exe1_label = gtk_label_new(_("1) Execute after Connect:"));
	exe2_label = gtk_label_new(_("2) Execute after Connect:"));
	
	exe1_entry = gtk_entry_new();	
	exe2_entry = gtk_entry_new();	
	
	gtk_misc_set_alignment(GTK_MISC(exe1_label), 0.0f, 0.5f);
	gtk_misc_set_alignment(GTK_MISC(exe2_label), 0.0f, 0.5f);
	
	gtk_box_pack_start(GTK_BOX(box),table,FALSE,FALSE,0);

	gtk_container_add(GTK_CONTAINER(frame),box);
	gtk_container_set_border_width(GTK_CONTAINER(frame),3);
	gtk_container_set_border_width(GTK_CONTAINER(table),8);

	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	gtk_table_set_col_spacing(GTK_TABLE(table),0,10);

	gtk_table_attach_defaults(GTK_TABLE(table),exe1_label,0,1,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),exe1_entry,1,2,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),exe2_label,0,1,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),exe2_entry,1,2,1,2);

	return frame;
}

/* End of the "Misc"-Tab frame */


/* The "Costs"-Tab frame */

static GtkWidget* gppp_accounts_costs_frame(void)
{
	GtkWidget* frame;
	GtkWidget* table;
	GtkWidget* box;
	GList *list;
	
	frame = gtk_frame_new(_("Cost Settings:"));

	box = gtk_vbox_new(FALSE,0);
	
	table = gtk_table_new(4,2,FALSE);
	
	usecosts_cb = gtk_check_button_new_with_label(_("Use Cost Calculation"));
	
	cost_label = gtk_label_new(_("Dollar $:"));
	time_label = gtk_label_new(_("per"));
	
	cost_entry = gtk_entry_new();
	time_combo = gtk_combo_new();
	
	list = NULL;
	list = g_list_append(list, times[0]);
	list = g_list_append(list, times[1]);
	list = g_list_append(list, times[2]);
	
	gtk_misc_set_alignment(GTK_MISC(cost_label), 0.0f, 0.5f);
	gtk_misc_set_alignment(GTK_MISC(time_label), 0.0f, 0.5f);

	gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(time_combo)->entry),FALSE);
	gtk_combo_set_popdown_strings(GTK_COMBO(time_combo),list);
	
	gtk_widget_set_usize(cost_entry,50,20);
	gtk_widget_set_usize(time_combo,50,20);

	if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(usecosts_cb)))
		{
			gtk_widget_set_sensitive(cost_label,FALSE);
			gtk_widget_set_sensitive(cost_entry,FALSE);
			gtk_widget_set_sensitive(time_label,FALSE);
			gtk_widget_set_sensitive(time_combo,FALSE);
		}
	
	gtk_box_pack_start(GTK_BOX(box),table,FALSE,FALSE,0);
	
	gtk_container_add(GTK_CONTAINER(frame),box);
	gtk_container_set_border_width(GTK_CONTAINER(frame),3);
	gtk_container_set_border_width(GTK_CONTAINER(table),8);

	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	gtk_table_set_col_spacing(GTK_TABLE(table),0,10);
	gtk_table_set_col_spacing(GTK_TABLE(table),1,10);

	gtk_table_attach_defaults(GTK_TABLE(table),usecosts_cb,0,4,0,1);
	gtk_table_attach_defaults(GTK_TABLE(table),cost_label,0,1,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),cost_entry,1,2,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),time_label,2,3,1,2);
	gtk_table_attach_defaults(GTK_TABLE(table),time_combo,3,4,1,2);
	
	gtk_signal_connect(GTK_OBJECT(usecosts_cb),
						"toggled",
						GTK_SIGNAL_FUNC(usecosts_cb_toggled_cb),
						NULL);


	return frame;
}

/* End of the "Costs"-Tab frame */


/* The Callback-Functions */

static void dyn_radbutton_toggled_cb(GtkWidget* widget, gpointer data)
{
	gboolean test = !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	gtk_widget_set_sensitive(ip_label, test);
	gtk_widget_set_sensitive(mask_label, test);
	gtk_widget_set_sensitive(ip_entry, test);
	gtk_widget_set_sensitive(mask_entry, test);
}	

static void auto_radbutton_toggled_cb(GtkWidget* widget, gpointer data)
{
	gboolean test = !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	gtk_widget_set_sensitive(primdns_label, test);
	gtk_widget_set_sensitive(secdns_label, test);
	gtk_widget_set_sensitive(primdns_entry, test);
	gtk_widget_set_sensitive(secdns_entry, test);
}	

static void usecosts_cb_toggled_cb(GtkWidget* widget, gpointer data)
{
	gboolean test = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	gtk_widget_set_sensitive(cost_label, test);
	gtk_widget_set_sensitive(cost_entry, test);
	gtk_widget_set_sensitive(time_label, test);
	gtk_widget_set_sensitive(time_combo, test);
}

static void savepwd_cb_toggled_cb(GtkWidget* widget, gpointer data)
{
	gboolean test = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
	gtk_widget_set_sensitive(pwd_label, test);
	gtk_widget_set_sensitive(pwd_entry, test);
	if (!test) 
		gtk_entry_set_text(GTK_ENTRY(pwd_entry),"");
}

static void add_button_clicked_cb(GtkWidget* widget, gpointer data)
{
	gchar* text[2];
	
	text[0] = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(key_combo)->entry));
	text[1] = gtk_entry_get_text(GTK_ENTRY(value_entry));
	
	if (selected_row!=-1)
		gtk_clist_insert(GTK_CLIST(clist),selected_row,text);
	else gtk_clist_append(GTK_CLIST(clist),text);

	gtk_entry_set_text(GTK_ENTRY(value_entry),"");
}	
	
static void del_button_clicked_cb(GtkWidget* widget, gpointer data)
{
	if (selected_row!=-1)
	{
		gtk_clist_remove(GTK_CLIST(clist), selected_row);
		gtk_entry_set_text(GTK_ENTRY(value_entry),"");
	}
} 

static void clist_select_row_cb(GtkWidget* widget, gint row, gint col, 
			GdkEventButton* event, gpointer data)
{
	gchar* text;
	gtk_clist_get_text(GTK_CLIST(clist),row, 1, &text);
	selected_row = row;
	gtk_entry_set_text(GTK_ENTRY(value_entry),text);
	gtk_widget_set_sensitive(GTK_WIDGET(data), TRUE);
}

static void clist_unselect_row_cb(GtkWidget* widget, gint row, gint col, 
			GdkEventButton* event, gpointer data)
{
	selected_row = -1;
	gtk_entry_set_text(GTK_ENTRY(value_entry),"");
	gtk_widget_set_sensitive(GTK_WIDGET(data), FALSE);
}

/* End of Callback Functions */


/* Set all the input-widgets with the data of the account */

void gppp_account_set_data(struct Account* account)
{
	void append(gpointer data, gpointer data2)
	{
		static gint i;
		static gchar* text[2];
		
		text[i] = (gchar*)data;
		
		if (i)
		{
			gtk_clist_append(GTK_CLIST(clist), text);
			i--;
		}
		else i++;
	}		
	
	gchar buffer[10];

	/* Set the widgets of the "General"-Tab */
	
	g_assert(account != NULL);

	gtk_entry_set_text(GTK_ENTRY(name_entry), account->name);
	gtk_entry_set_text(GTK_ENTRY(phone_entry), account->phonenumber);
	gtk_entry_set_text(GTK_ENTRY(user_entry), account->user);
	gtk_entry_set_text(GTK_ENTRY(pwd_entry), account->password);
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(savepwd_check),account->save_pwd);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(usescript_check),account->use_script);

	/* Set the widgets of the "IP"-Tab */
	gtk_entry_set_text(GTK_ENTRY(ip_entry), account->ip);
	gtk_entry_set_text(GTK_ENTRY(mask_entry), account->subnet_mask);
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sta_radbutton), account->static_ip);

	/* Set the widgets of the "DNS"-Tab */
	gtk_entry_set_text(GTK_ENTRY(domain_entry), account->domain);
	gtk_entry_set_text(GTK_ENTRY(primdns_entry), account->prim_dns);
	gtk_entry_set_text(GTK_ENTRY(secdns_entry), account->sec_dns);

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(man_radbutton), account->manual_dns);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(disableolddns_cb), account->disable_ex_dns);

	/* Set the widgets of the "Script"-Tab */
	g_list_foreach(account->script, (GFunc)append, NULL);

	/* Set the widgets of the "Misc"-Tab */
	gtk_entry_set_text(GTK_ENTRY(exe1_entry), account->execute1);
	gtk_entry_set_text(GTK_ENTRY(exe2_entry), account->execute2);
	

	/* Set the widgets of the "Costs"-Tab */
	g_snprintf(buffer,10,"%.2f",account->costs);	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(usecosts_cb), account->use_costs);
	
	gtk_entry_set_text(GTK_ENTRY(cost_entry), buffer);
	
	switch (account->time)
	{
		case 1: sprintf(buffer,"%s",_("hour"));
				break;
		case 2: sprintf(buffer,"%s",_("minute"));
				break;
		case 3: sprintf(buffer,"%s",_("second"));
				break;
		default: sprintf(buffer,"%s","???");
	}
	
	gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(time_combo)->entry),buffer);
}

struct Account* gppp_account_get_data(void)
{
	gchar* text, *tmp1, *tmp2;
	gint result, i, j;
	
	GtkWidget* dialog;
	
	struct Account* account;
	
	account = g_malloc(sizeof(struct Account));
	
	/* Get the widgets of the "General"-Tab */
	account->name = gppp_entry_get_text(name_entry);
	account->phonenumber = gppp_entry_get_text(phone_entry);
	account->user = gppp_entry_get_text(user_entry);
	account->password = gppp_entry_get_text(pwd_entry);
	
	account->save_pwd = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(savepwd_check));
	account->use_script = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(usescript_check));
	
	/* Get the widgets of the "IP"-Tab */
	account->ip = gppp_entry_get_text(ip_entry);
	account->subnet_mask = gppp_entry_get_text(mask_entry);
	
	account->static_ip = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sta_radbutton));

	/* Get the widgets of the "DNS"-Tab */
	account->domain = gppp_entry_get_text(domain_entry);
	account->prim_dns = gppp_entry_get_text(primdns_entry);
	account->sec_dns = gppp_entry_get_text(secdns_entry);

	account->manual_dns = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(man_radbutton));
	account->disable_ex_dns = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(disableolddns_cb));

	/* Get the widgets of the "Script"-Tab */
	account->script = NULL;
	i=j=0;
	while ((gtk_clist_get_text(GTK_CLIST(clist),i,j++,&tmp1)) && 
		(gtk_clist_get_text(GTK_CLIST(clist),i++,j--,&tmp2)))
	{
		g_assert(tmp1 != NULL);
		g_assert(tmp2 != NULL);
		text = g_malloc(strlen(tmp1)+1);
		strcpy(text,tmp1);
		account->script = g_list_append(account->script,text);
		text = g_malloc(strlen(tmp2)+1);
		strcpy(text,tmp2);
		account->script = g_list_append(account->script,text);
	}
	
	/* Get the widgets of the "Misc"-Tab */
	account->execute1 = gppp_entry_get_text(exe1_entry);
	account->execute2 = gppp_entry_get_text(exe2_entry);
	

	/* Get the widgets of the "Costs"-Tab */
	account->use_costs = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(usecosts_cb));
	
	text = gtk_entry_get_text(GTK_ENTRY(cost_entry));
	
	account->costs = g_strtod(text,NULL); 

	i=1;
	text = gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(time_combo)->entry));

	while (strcmp(text, times[i-1])) 
		i++;
	
	account->time = i;
	return account;	
}

void gppp_account_free(struct Account* account)
{
	g_assert(account != NULL);
	
	g_assert(account->name != NULL);
	g_assert(account->phonenumber != NULL);
	g_assert(account->user != NULL);
	g_assert(account->password != NULL);
	g_assert(account->ip != NULL);
	g_assert(account->subnet_mask != NULL);
	g_assert(account->domain != NULL);
	g_assert(account->prim_dns != NULL);
	g_assert(account->sec_dns != NULL);
	//g_assert(account->script != NULL);
	g_assert(account->execute1 != NULL);
	g_assert(account->execute2 != NULL);

	g_free(account->name);
	g_free(account->phonenumber);
	g_free(account->user);
	g_free(account->password);
	g_free(account->ip);
	g_free(account->subnet_mask);
	g_free(account->domain);
	g_free(account->prim_dns);
	g_free(account->sec_dns);
	if (account->script != NULL) g_list_free(account->script);
	g_free(account->execute1);
	g_free(account->execute2);
	
	//g_free(account);
}