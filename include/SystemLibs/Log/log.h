#ifndef __IMOU_LOG_H__
#define __IMOU_LOG_H__

#include <syslog.h>

#ifndef ILOG_TAG
#define ILOG_TAG "    "
#endif
#ifdef __cplusplus
extern "C" {
#endif
int __imou_log_print(int prio, const char *tag, const char *fmt, ...);

#define imou_printLog(prio, tag, fmt...) \
	__imou_log_print(prio, tag, fmt)

#ifndef ILOG_PRI
	#define ILOG_PRI(priority, tag, ...) \
		imou_printLog(priority, tag, __VA_ARGS__)
#endif

#ifndef ILOGD
	#define ILOGD(...) ((void)ILOG(LOG_DEBUG, ILOG_TAG, __VA_ARGS__))
#endif

#ifndef ILOGW
	#define ILOGW(...) ((void)ILOG(LOG_WARNING, ILOG_TAG, __VA_ARGS__))
#endif

#ifndef ILOGE
	#define ILOGE(...) ((void)ILOG(LOG_ERR, ILOG_TAG, __VA_ARGS__))
#endif

#ifndef ILOGI
	#define ILOGI(...) ((void)ILOG(LOG_INFO, ILOG_TAG, __VA_ARGS__))
#endif

#ifndef ILOGN
	#define ILOGN(...) ((void)ILOG(LOG_NOTICE, ILOG_TAG, __VA_ARGS__))
#endif

#ifndef ILOGC
	#define ILOGC(...) ((void)ILOG(LOG_CRIT, ILOG_TAG, __VA_ARGS__))
#endif

#ifndef ILOGA
	#define ILOGA(...) ((void)ILOG(LOG_ALERT, ILOG_TAG, __VA_ARGS__))
#endif

#ifndef ILOGM
	#define ILOGM(...) ((void)ILOG(LOG_EMERG, ILOG_TAG, __VA_ARGS__))
#endif

#ifndef ILOG
	#define ILOG(priority, tag, ...) \
		ILOG_PRI(priority, tag, __VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif

#endif
