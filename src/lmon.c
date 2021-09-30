#include "lmon.h"
#include "lmon_resource.h"
#include <gtk/gtk.h>
#include <libxfce4util/libxfce4util.h>
#include <libxfce4panel/libxfce4panel.h>

static void lmon_new(XfcePanelPlugin *);
static void lmon_delete(XfcePanelPlugin *, LMonData *);
static gboolean lmon_tick(LMonData *);
static void lmon_clicked(GtkButton *, LMonData *);
static void lmon_about(XfcePanelPlugin *, LMonData *);
static gboolean lmon_dont_delete(GtkWidget *, GdkEvent *, LMonData *);

XFCE_PANEL_PLUGIN_REGISTER(lmon_new);

static void lmon_new(XfcePanelPlugin *plugin)
{
	LMonData *data = g_slice_new(LMonData);
	g_signal_connect(plugin, "free-data", G_CALLBACK(lmon_delete), data);
	g_signal_connect(plugin, "about", G_CALLBACK(lmon_about), data);

	data->b = gtk_builder_new_from_string(LMON_UI_START, LMON_UI_SIZE);

	data->s = gtk_css_provider_new();
	gtk_css_provider_load_from_data(data->s, LMON_CSS_START, LMON_CSS_SIZE, NULL);
	gtk_style_context_add_provider_for_screen(
		gdk_screen_get_default(), GTK_STYLE_PROVIDER(data->s),
		GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	data->root = (GtkWidget *)gtk_builder_get_object(data->b, "root");
	data->boxa = (GtkWidget *)gtk_builder_get_object(data->b, "percents_box");
	data->boxb = (GtkWidget *)gtk_builder_get_object(data->b, "loads_box");
	data->cpuvalue = (GtkLabel *)gtk_builder_get_object(data->b, "cpuvalue");
	data->memvalue = (GtkLabel *)gtk_builder_get_object(data->b, "memvalue");
	data->loadvalue = (GtkLabel *)gtk_builder_get_object(data->b, "loadvalue");
	data->about = (GtkWindow *)gtk_builder_get_object(data->b, "aboutwin");

	g_signal_connect(data->root, "clicked", G_CALLBACK(lmon_clicked), data);
	g_signal_connect(data->about, "delete-event", G_CALLBACK(lmon_dont_delete), data);

	gtk_container_add(GTK_CONTAINER(data->root), data->boxa);
	gtk_widget_show_all(GTK_WIDGET(data->root));
	gtk_widget_show_all(GTK_WIDGET(data->boxb));
	gtk_container_add(GTK_CONTAINER(plugin), data->root);

	data->side = false;
	data->aboutvisible = false;
	data->last_all = 0;

	lmon_tick(data);
	data->timer = g_timeout_add(1000, G_SOURCE_FUNC(lmon_tick), data);

	xfce_panel_plugin_menu_show_about(plugin);
	xfce_panel_plugin_menu_show_configure(plugin);
}

static void lmon_delete(XfcePanelPlugin *, LMonData *data)
{
	g_source_remove(data->timer);
	g_object_unref(data->b);
	g_object_unref(data->s);
	gtk_widget_destroy(data->about);
	g_slice_free(LMonData, data);
}

static gboolean lmon_tick(LMonData *data)
{
	measure_cpu(data);
	measure_mem(data);
	measure_load(data);
	return TRUE;
}

static void lmon_clicked(GtkButton *, LMonData *data)
{
	if (data->side) {
		gtk_container_remove(GTK_CONTAINER(data->root), data->boxb);
		gtk_container_add(GTK_CONTAINER(data->root), data->boxa);
	} else {
		gtk_container_remove(GTK_CONTAINER(data->root), data->boxa);
		gtk_container_add(GTK_CONTAINER(data->root), data->boxb);
	}
	data->side = !data->side;
}

static void lmon_about(XfcePanelPlugin *, LMonData *data)
{
	if (data->aboutvisible) {
		gtk_window_set_urgency_hint(data->about, TRUE);
	} else {
		data->aboutvisible = true;
		gtk_widget_show_all(GTK_WIDGET(data->about));
	}
}

static gboolean lmon_dont_delete(GtkWidget *, GdkEvent *, LMonData *data)
{
	data->aboutvisible = false;
	gtk_widget_hide(GTK_WIDGET(data->about));
	return TRUE;
}
