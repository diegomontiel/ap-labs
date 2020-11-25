#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include "logger.h"

int loggerType; 

int initLogger(char *logType) {
	if(strcmp("syslog", logType) == 0){
		loggerType = 1;
	} else if (strcmp("stdout", logType) == 0 || strcmp("",logType) == 0) {
		loggerType = 0;
	}	
	printf("Initializing Logger on: %s\n", logType);
	return 0;
}

int infof(const char *format, ...) {
	//reset
	printf("\033[0m");
	va_list arg;
	va_start(arg, format);

	if(loggerType == 1){
		//printf("reaches logger")
		vsyslog(LOG_INFO, format, arg);
		closelog();
	} else {
		vprintf(format, arg);
	}

	printf("\033[0m");
	va_end(arg);
	printf("\n");
	return 0;
}

int warnf(const char *format, ...) {
	//yellow
	printf("\033[1;33m");
	va_list arg;
	va_start(arg, format);

	if(loggerType == 1){
		vsyslog(LOG_WARNING, format, arg);
		closelog();
	} else {
		vprintf(format, arg);
	}

	printf("\033[0m");
	va_end(arg);
	printf("\n");
	return 0;
}

int errorf(const char *format, ...) {
	//red
	printf("\033[1;31m");
	va_list arg;
	va_start(arg, format);

	if(loggerType == 1){
		vsyslog(LOG_ERR, format, arg);
		closelog();
	} else {
		vprintf(format, arg);
	}

	printf("\033[0m");
	va_end(arg);
	printf("\n");
	return 0;
}
int panicf(const char *format, ...) {
	//pink
	printf("\033[1;35m");
	va_list arg;
	va_start(arg, format);

	if(loggerType == 1){
		vsyslog(LOG_CRIT, format, arg);
		closelog();
	} else {
		vprintf(format, arg);
	}

	printf("\033[0m");
	va_end(arg);
	printf("\n");
	return 0;
}