#ifndef LMON_H_
#define LMON_H_

#include <gtk/gtk.h>

typedef struct {
	GtkWidget *box;
	GtkLabel *cpuvalue;
	GtkLabel *memvalue;
	GtkBuilder *b;
	GtkCssProvider *s;
	unsigned long long last_used;
	unsigned long long last_all;
	int cpus;
	guint timer;
} LMonData;

void measure_cpu(LMonData *);
void measure_mem(LMonData *);

#endif
