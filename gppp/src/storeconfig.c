#include "config.h"
#include <gnome.h>
#include "accounts.h"

void gppp_store_config(GList* accounts)
{
	
	void store_account(gpointer data, gpointer user_data)
	{
		struct Account* account = (struct Account*)data;
		static gint nr;
		gchar* path;
		gchar* key;
		
		path = g_strdup_printf("gppp/account%i/",nr);
		
		key = g_strconcat(path,"name",NULL);
		gnome_config_set_string(key,account->name);
		g_free(key);
		key = g_strconcat(path,"phonenumber",NULL);
		gnome_config_set_string(key,account->phonenumber);
		g_free(key);
		key = g_strconcat(path,"user",NULL);
		gnome_config_set_string(key,account->user);
		g_free(key);
		key = g_strconcat(path,"password",NULL);
		gnome_config_set_string(key,account->password);
		g_free(key);
		key = g_strconcat(path,"save_pwd",NULL);
		gnome_config_set_bool(key,account->save_pwd);
		g_free(key);
		key = g_strconcat(path,"use_script",NULL);
		gnome_config_set_bool(key,account->use_script);
		g_free(key);
		key = g_strconcat(path,"static_ip",NULL);
		gnome_config_set_bool(key,account->static_ip);
		g_free(key);
		key = g_strconcat(path,"ip",NULL);
		gnome_config_set_string(key,account->ip);
		g_free(key);
		key = g_strconcat(path,"subnet_mask",NULL);
		gnome_config_set_string(key,account->subnet_mask);
		g_free(key);
		key = g_strconcat(path,"domain",NULL);
		gnome_config_set_string(key,account->domain);
		g_free(key);
		key = g_strconcat(path,"manual_dns",NULL);
		gnome_config_set_bool(key,account->manual_dns);
		g_free(key);
		key = g_strconcat(path,"prim_dns",NULL);
		gnome_config_set_string(key,account->prim_dns);
		g_free(key);
		key = g_strconcat(path,"sec_dns",NULL);
		gnome_config_set_string(key,account->sec_dns);
		g_free(key);
		key = g_strconcat(path,"disable_ex_dns",NULL);
		gnome_config_set_bool(key,account->disable_ex_dns);
		g_free(key);
		/* FIXME: here I have to save the script */
		key = g_strconcat(path,"execute1",NULL);
		gnome_config_set_string(key,account->execute1);
		g_free(key);
		key = g_strconcat(path,"execute2",NULL);
		gnome_config_set_string(key,account->execute2);
		g_free(key);
		key = g_strconcat(path,"use_costs",NULL);
		gnome_config_set_bool(key,account->use_costs);
		g_free(key);
		key = g_strconcat(path,"costs",NULL);
		gnome_config_set_float(key,account->costs);		
		g_free(key);
		key = g_strconcat(path,"time",NULL);
		gnome_config_set_int(key,account->time);		
		g_free(key);
		g_free(path);
		nr++;
	}
	gint count = g_list_length(accounts);
	
	
	gnome_config_set_int("gppp/global/accounts", count);
	
	g_list_foreach(accounts, (GFunc)store_account, NULL);
	
	gnome_config_sync();
}

GList* gppp_load_config(void)
{
	struct Account* load_account(gint nr)
	{
		struct Account* account;
		gchar* path;
		gchar* key;
		
		account = g_malloc(sizeof(struct Account));
		
		path = g_strdup_printf("gppp/account%i/",nr);
		
		key = g_strconcat(path,"name",NULL);
		account->name = gnome_config_get_string(key);
		g_free(key);
		key = g_strconcat(path,"phonenumber",NULL);
		account->phonenumber = gnome_config_get_string(key);
		g_free(key);
		key = g_strconcat(path,"user",NULL);
		account->user = gnome_config_get_string(key);
		g_free(key);
		key = g_strconcat(path,"password",NULL);
		account->password = gnome_config_get_string(key);
		g_free(key);
		key = g_strconcat(path,"save_pwd",NULL);
		account->save_pwd = gnome_config_get_bool(key);
		g_free(key);
		key = g_strconcat(path,"use_script",NULL);
		account->use_script = gnome_config_get_bool(key);
		g_free(key);
		key = g_strconcat(path,"static_ip",NULL);
		account->static_ip = gnome_config_get_bool(key);
		g_free(key);
		key = g_strconcat(path,"ip",NULL);
		account->ip = gnome_config_get_string(key);
		g_free(key);
		key = g_strconcat(path,"subnet_mask",NULL);
		account->subnet_mask = gnome_config_get_string(key);
		g_free(key);
		key = g_strconcat(path,"domain",NULL);
		account->domain = gnome_config_get_string(key);
		g_free(key);
		key = g_strconcat(path,"manual_dns",NULL);
		account->manual_dns = gnome_config_get_bool(key);
		g_free(key);
		key = g_strconcat(path,"prim_dns",NULL);
		account->prim_dns = gnome_config_get_string(key);
		g_free(key);
		key = g_strconcat(path,"sec_dns",NULL);
		account->sec_dns = gnome_config_get_string(key);
		g_free(key);
		key = g_strconcat(path,"disable_ex_dns",NULL);
		account->disable_ex_dns = gnome_config_get_bool(key);
		g_free(key);
		/* FIXME: here I have to save the script */
		account->script = NULL;
		key = g_strconcat(path,"execute1",NULL);
		account->execute1 = gnome_config_get_string(key);
		g_free(key);
		key = g_strconcat(path,"execute2",NULL);
		account->execute2 = gnome_config_get_string(key);
		g_free(key);
		key = g_strconcat(path,"use_costs",NULL);
		account->use_costs = gnome_config_get_bool(key);
		g_free(key);
		key = g_strconcat(path,"costs",NULL);
		account->costs = gnome_config_get_float(key);		
		g_free(key);
		key = g_strconcat(path,"time",NULL);
		account->time = gnome_config_get_int(key);		
		g_free(key);
		g_free(path);
	
		return account;
	}
	
	gint i, max;
	GList* accounts; 
	struct Account* account;
	
	accounts = NULL;
	
	max = gnome_config_get_int_with_default("gppp/global/accounts", 0);
	for (i=0;i<max;i++)
	{
		account = load_account(i);
		accounts = g_list_append(accounts, account);
		//g_print("%s\n", account->name);
	}
	
	return accounts;
}		