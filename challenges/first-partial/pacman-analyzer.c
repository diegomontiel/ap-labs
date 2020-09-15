#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define REPORT_FILE "packages_report.txt"

void analizeLog(char *logFile, char *report);

struct Node {
	char* name;
	char* insDate;
	char* updDate;
	int nUpdates;
	char* rmDate;
};
struct Node nodes[600];
int installed, update, current;

//getline
char* readLine(int readFile){
	int size = 50;
	int total = 0; // move to nect place in buffer
	char* buffer;
	buffer = calloc(size, sizeof(char));
	int n;

	while((n = read(readFile, buffer+total, size)) > 0){
		for(int i=total; i<total+n; i++){
			if(buffer[i] == '\n'){
				lseek(readFile, i-(total+n)+1, SEEK_CUR);
				buffer[i] = '\0';
				return buffer;
			}
		}
		total+=n;
		buffer = realloc(buffer, total+size);
	}

	if(total>0){
		buffer[total] = '\0'; //check for end of line
		return buffer;
	}
	return NULL;
}

char* getDate(char *str){
	int size = 0;
	for(size; str[size] != ']'; size++);
	size++; // last bracket

	char *temp = calloc(size, sizeof(char));
	for(int i=0; i<size; i++){
		temp[i] = str[i];
	}

	temp[size] = '\0';
	return temp;
}

char* getName(char *str){
	int size = 0;
	for(size; size<strlen(str); size++){
		if(str[size] == ' ' || str[size] == '\n'){
		       break;
		}
 	}

	char *temp = calloc(size+1, sizeof(char));
	for(int i=0; i<size; i++){
		temp[i] = str[i];
	}
	
	temp[size] = '\0';
	return temp;
}

void addNode(char* line){
	nodes[installed].name = getName(line+36);
	nodes[installed].insDate = getDate(line+1);
	installed++;
	current++;
}

void upNode(char* line){
	char *name = getName(line+35);
	for(int i=0; i<installed; i++){
		if(strcmp(nodes[i].name, name) == 0){
			nodes[i].updDate = getDate(line+1);
			nodes[i].nUpdates++;
			update++;
			free(name);
			return;
		}
	}
}

void rmNode(char* line){
	char *name = getName(line+34);
	for(int i=0; i<installed; i++){
		if(strcmp(nodes[i].name, name) == 0){
			nodes[i].rmDate = getDate(line+1);
			current--;
			free(name);
			return;
		}
	}
}

int main(int argc, char **argv) {

    if (argc == 5) {
	printf("Usage:./pacman-analizer.o pacman.log\n");
	analizeLog(argv[2], argv[4]);
    }else{
	    printf("Parmeters do not match\n");
    }

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

	int readFile = open(logFile, O_RDONLY);
	if(readFile < 0){
		printf("Error in log\n");
		return;
	}
       // if(logFile == "pacman2.txt"){
	//	printf("usas pacman2");
	//}
	char *line;
	while((line = readLine(readFile)) != NULL){
		char *state = getName(line+26);
		if(strcmp("installed", state) == 0){
			addNode(line);
		} else if(strcmp("upgraded", state) == 0){
			upNode(line);
		} else if(strcmp("removed", state) == 0){
			rmNode(line);
		}
		free(state);
		free(line);
	}
	close(readFile);

	int writer = open(report, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if(writer < 0){
		printf("Error in report\n");
		return;
	}

	// PACKAGES REPORT
	char buffer[10];
	write(writer, "Pacman Packages Report\n", 23); //strlen()+1 for every write to keep format
	write(writer, "----------------------\n", 23);
	write(writer, "- Installed packages : ",23);
	sprintf(buffer,"%d\n", installed);
	write(writer, buffer, strlen(buffer));
	write(writer, "- Removed packages   : ",23);
	sprintf(buffer,"%d\n", (installed - current));
	write(writer, buffer, strlen(buffer));
	write(writer, "- Upgraded packages  : ",23);
	sprintf(buffer,"%d\n", update);
	write(writer, buffer, strlen(buffer));
	write(writer, "- Current installed  : ",23);
	sprintf(buffer,"%d\n", current);
	write(writer, buffer, strlen(buffer));

	// PACKAGE INFORMATION
	write(writer, "\nList of packages\n------------------\n", 38);
	for(int i=0; i<installed; i++){
		write(writer, "- Package Name        : ", 24);
		write(writer, nodes[i].name, strlen(nodes[i].name));
		write(writer, "\n  - Install date      : ",25);
		write(writer, nodes[i].insDate, strlen(nodes[i].insDate));
		write(writer, "\n  - Last update date  : ",25);
		//check update date
		if(nodes[i].updDate != NULL){
			write(writer, nodes[i].updDate, strlen(nodes[i].updDate));
		}else{
			write(writer, "-", 1);
		}
		write(writer, "\n  - How many updates  : ",25);
		//write(writer, nodes[i].nUpdates, strlen(nodes[i].nUpdates));
		sprintf(buffer,"%d\n", nodes[i].nUpdates);
		write(writer, buffer, strlen(buffer));
		write(writer, "  - Removal date      : ", 24);
		//check remove date
		if(nodes[i].rmDate != NULL){
			write(writer, nodes[i].rmDate, strlen(nodes[i].rmDate));
		}else{
			write(writer, "-", 1);
		}
		write(writer, "\n", 1);
	}
	close(writer);

    printf("Report is generated at: [%s]\n", report);
}
