#include "lmon.h"
#include "lmon_resource.h"
#include <gtk/gtk.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/libxfce4panel.h>

static void lmon_new(XfcePanelPlugin *);
static void lmon_delete(XfcePanelPlugin *, LMonData *);
static gboolean lmon_tick(LMonData *);

XFCE_PANEL_PLUGIN_REGISTER(lmon_new);

static void lmon_new(XfcePanelPlugin *plugin)
{
	LMonData *data = g_slice_new(LMonData);
	g_signal_connect(plugin, "free-data", G_CALLBACK(lmon_delete), data);

	data->b = gtk_builder_new_from_string(LMON_UI_START, LMON_UI_SIZE);
	
	data->s = gtk_css_provider_new();
	gtk_css_provider_load_from_data(data->s, LMON_CSS_START, LMON_CSS_SIZE, NULL);
	gtk_style_context_add_provider_for_screen(
		gdk_screen_get_default(), GTK_STYLE_PROVIDER(data->s),
		GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	data->box = (GtkWidget *)gtk_builder_get_object(data->b, "box");
	data->cpuvalue = (GtkLabel *)gtk_builder_get_object(data->b, "cpuvalue");
	data->memvalue = (GtkLabel *)gtk_builder_get_object(data->b, "memvalue");

	data->last_all = 0;

	gtk_container_add(GTK_CONTAINER(plugin), data->box);
	gtk_widget_show_all(GTK_WIDGET(plugin));
	lmon_tick(data);
	data->timer = g_timeout_add(1000, G_SOURCE_FUNC(lmon_tick), data);
}

static void lmon_delete(XfcePanelPlugin *plugin, LMonData *data)
{
	(void)plugin;
	g_source_remove(data->timer);
	gtk_widget_destroy(data->box);
	g_object_unref(data->b);
	g_object_unref(data->s);
	g_slice_free(LMonData, data);
}

static gboolean lmon_tick(LMonData *data)
{
	measure_cpu(data);
	measure_mem(data);
	return TRUE;
}
