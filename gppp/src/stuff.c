#include <gnome.h>

/* This one returns a button with a gnome_stock pixmap and custom text on it */
GtkWidget* gppp_button(const char *stock, const char *text)
{
	GtkWidget* label;
	GtkWidget* icon;
	GtkWidget* h_box;
	GtkWidget* button;
	
	button = gtk_button_new();

	label = gtk_label_new(text);
	
	icon = gnome_stock_new_with_icon(stock);

	h_box = gtk_hbox_new(FALSE,0);
	
	gtk_box_pack_start(GTK_BOX(h_box),icon,FALSE,FALSE,2);
	gtk_box_pack_start(GTK_BOX(h_box),label,FALSE,FALSE,2);

	gtk_container_add(GTK_CONTAINER(button),h_box);
	gtk_container_set_border_width(GTK_CONTAINER(h_box),1);
	
	gtk_widget_show_all(button);
	
	return button;
	
}

/* This function gets text from a entry and stores it in a gchar which has been g_malloced first */
gchar* gppp_entry_get_text(GtkWidget* entry)
{
	gchar* result, *text;

	text = gtk_entry_get_text(GTK_ENTRY(entry));
	result = g_malloc0(strlen(text)+1);
	strcpy(result,text);
	return result;
}

