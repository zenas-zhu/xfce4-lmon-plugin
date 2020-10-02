#ifndef LMON_RESOURCE_H_
#define LMON_RESOURCE_H_

extern char _binary_data_lmon_ui_start;
extern char _binary_data_lmon_ui_end;
//extern char _binary_data_lmon_ui_size;

#define LMON_UI_START (&_binary_data_lmon_ui_start)
#define LMON_UI_END (&_binary_data_lmon_ui_end)
#define LMON_UI_SIZE ((size_t)(LMON_UI_END-LMON_UI_START))

extern char _binary_data_lmon_css_start;
extern char _binary_data_lmon_css_end;
//extern char _binary_data_lmon_css_size;

#define LMON_CSS_START (&_binary_data_lmon_css_start)
#define LMON_CSS_END (&_binary_data_lmon_css_end)
#define LMON_CSS_SIZE ((size_t)(LMON_CSS_END-LMON_CSS_START))

#endif
