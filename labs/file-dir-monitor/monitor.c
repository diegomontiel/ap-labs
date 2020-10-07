#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ftw.h>
#include <fcntl.h>
#include<sys/inotify.h>
#include "logger.h"

#define NAME_MAX 1024
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int fd, wd;
struct inotify_event *evt;
int dir = 0;
ssize_t moves;
char *point;
char buffer[BUF_LEN] __attribute__ ((aligned(8)));

static void monitor(struct inotify_event *i){
	if(i->mask & IN_CREATE){
		if(evt->mask & IN_ISDIR){
			infof("Directory created %s\n", i->name);
			dir=1;
		}else{
			infof("File created %s\n", i->name);
		}
	}
	if(i->mask & IN_DELETE){
		infof("Deleted %s\n", i->name);
	}
	if(i->mask & IN_MOVED_FROM){
		infof("Moved FROM %s\n", i->name);
	}
	if(i->mask & IN_MOVED_TO){
		infof("Moved TO %s\n", i->name);
	}
}

int addWatch (const char *pathname, const struct stat *sb,int flag) {
	wd = inotify_add_watch(fd, pathname, IN_ALL_EVENTS);
	if(wd == -1){
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[]){
	if(argc < 2){
		errorf("Missing parameters \n");
		return -1;
	}else if(argc >2){
		errorf("Exceeding parameters \n");
		return -1;
	}else{
		fd = inotify_init();

		if(fd == -1){
			errorf("inotify error");
			return 0;
		}
		ftw(argv[1], addWatch, 2048);

		for(;;){
			moves = read(fd, buffer, BUF_LEN);
			if(moves == 0){
				errorf("init() is 0");
			}
			if(moves == -1){
				errorf("fail to read");
			}
			for(point = buffer; point < buffer+moves; ){
				evt = (struct inotify_event *) point;
				monitor(evt);
				point += sizeof(struct inotify_event) + evt->len;
			}
			if(dir == 1){
				ftw(argv[1], addWatch, 2048);
				dir = 0;
			}
		}

		inotify_rm_watch(fd, wd);
		close(fd);
		return 0;
	}
}
