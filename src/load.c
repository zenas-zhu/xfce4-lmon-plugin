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
		static char label[6];
		double proportion_used = 100.0 * data->cpus *
			(used - data->last_used) / (all - data->last_all);
		if (proportion_used < 100000.00) {
			snprintf(label, 6, "%03.0f%%", proportion_used);
			gtk_label_set_text(GTK_LABEL(data->cpuvalue), label);
		} else {
			gtk_label_set_text(GTK_LABEL(data->cpuvalue), "1K+");
		}
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
	static char label[7];
	snprintf(label, 7, "%04.1f%%", 100.0 * (total - avail) / total);
	gtk_label_set_text(GTK_LABEL(data->memvalue), label);
}

void measure_load(LMonData *data)
{
	double loadavg[3];
	if (getloadavg(loadavg, 3) < 0) {
		g_warning("can't read load averages");
		return;
	}
	static char labelmini[3][7];
	for (int i = 0; i < 3; i++) {
		if (loadavg[i] < 1000.0) {
			snprintf(labelmini[i], 7, "%.2f", loadavg[i]);
		} else {
			snprintf(labelmini[i], 7, "1K+");
		}
	}
	static char label[27];
	snprintf(label, 27, "%s · %s · %s",
			labelmini[0], labelmini[1], labelmini[2]);
	gtk_label_set_text(GTK_LABEL(data->loadvalue), label);
}
