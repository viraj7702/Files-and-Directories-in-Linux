#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

void traverse(char *filename, char *target, char *path);

void traverse(char *filename, char *target, char *path) {
	struct stat file;
	stat(filename, &file);
	int x = 1;
	
	if(S_ISDIR(file.st_mode)) {
		DIR *d;
		struct dirent *f;
		chdir(filename);
		d = opendir(".");
		while((f = readdir(d)) != NULL) {
			if((f->d_name)[0] != '.') {
				if(x) {
					strcat(path, "/");
					strcat(path, filename);
					x = 0;
					if(strstr(filename, target) != NULL) {
			                	printf("%s\n", path);
              				}

				}
				traverse(f->d_name, target, path);
			}
		}
		char temp[9999];
		for(int i = 0; i < strlen(path)-strlen(filename)-1; i++) {
                	temp[i] = path[i];
                }
                        strcpy(path, temp);

		chdir("..");
		closedir(d);	
	}

	else {
		if(strstr(filename, target) != NULL) {
			printf("%s/%s\n", path, filename);
		}
	}

}	

int main(int argc, char **argv) {
	
	DIR *d;
	struct dirent *file;
	char path[9999];
	strcat(path, ".");

	d = opendir(".");

	if(argc > 1) {
		while((file = readdir(d)) != NULL) {
			if((file->d_name)[0] != '.') {
				traverse(file->d_name, argv[1], path);
			}
		}
	}

	closedir(d);

	return EXIT_SUCCESS;
}
