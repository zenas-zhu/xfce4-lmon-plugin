#include <stdio.h>
#include "lmon.h"

void measure_cpu(LMonData *data)
{
	FILE *fd = fopen("/proc/stat", "r");
	if (fd == NULL) {
		g_warning("can't read cpu values");
		return;
	}
	unsigned long long user, nice, sys, idle, io, irq, softirq;
	int scanned = fscanf(fd,
		"cpu %llu %llu %llu %llu %llu %llu %llu",
		&user, &nice, &sys, &idle, &io, &irq, &softirq);
	if (scanned < 7) {
		g_warning("didn't read cpu values");
		return;
	}
	unsigned long used = user + nice + sys + irq + softirq;
	unsigned long all = used + idle + io;
	if (data->last_all) {
		static char label[10];
		double proportion_used = 100.0 * data->cpus *
			(used - data->last_used) / (all - data->last_all);
		snprintf(label, 7, "%03.0f%%", proportion_used);
		gtk_label_set_text(GTK_LABEL(data->cpuvalue), label);
	} else {
		data->cpus = 0;
		fscanf(fd, "%*[^\n]\n");
		int i;
		while (fscanf(fd, "cpu%d %*[^\n]\n", &i) == 1) {
			data->cpus += 1;
		}
		gtk_label_set_text(GTK_LABEL(data->cpuvalue), "...");
	}
	fclose(fd);
	data->last_used = used;
	data->last_all = all;
}

void measure_mem(LMonData *data)
{
	FILE *fd = fopen("/proc/meminfo", "r");
	if (fd == NULL) {
		g_warning("can't read mem values");
		return;
	}
	unsigned long long total, avail;
	int scanned = fscanf(fd,
		"MemTotal: %llu kB %*[^\n] MemAvailable: %llu kB",
		&total, &avail);
	fclose(fd);
	if (scanned < 2) {
		g_warning("didn't read mem values");
		return;
	}
	static char label[10];
	snprintf(label, 7, "%04.1f%%", 100.0 * (total - avail) / total);
	gtk_label_set_text(GTK_LABEL(data->memvalue), label);
}
