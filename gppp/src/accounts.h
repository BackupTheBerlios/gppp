#ifndef GPPP_ACCOUNTS_H
#define GPPP_ACCOUNTS_H

struct Account 
{
	gboolean new;
	struct Account* old_data;
	gchar* name;
	gchar* phonenumber;
	gchar* user;
	gchar* password;
	gboolean save_pwd;
	gboolean use_script;
	gboolean static_ip;
	gchar *ip;
	gchar *subnet_mask;
	gchar *domain;
	gboolean manual_dns;
	gchar *prim_dns;
	gchar *sec_dns;
	gboolean disable_ex_dns;
	GList *script;
	gchar *execute1;
	gchar *execute2;
	gboolean use_costs;
	gdouble costs;
	gint time;
};
		
GtkWidget* gppp_account_new(void);

void gppp_account_set_data(struct Account* account);
struct Account* gppp_account_get_data(void); 
void gppp_account_free(struct Account* account);
#endif