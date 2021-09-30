#ifndef LMON_H_
#define LMON_H_

#include <stdbool.h>
#include <gtk/gtk.h>

typedef struct {
	GtkWidget *root;
	GtkWidget *boxa;
	GtkWidget *boxb;
	GtkLabel *cpuvalue;
	GtkLabel *memvalue;
	GtkLabel *loadvalue;
	GtkWindow *about;
	GtkBuilder *b;
	GtkCssProvider *s;
	bool side;
	unsigned long long last_used;
	unsigned long long last_all;
	int cpus;
	guint timer;
	bool aboutvisible;
} LMonData;

void measure_cpu(LMonData *);
void measure_mem(LMonData *);
void measure_load(LMonData *);

#endif
