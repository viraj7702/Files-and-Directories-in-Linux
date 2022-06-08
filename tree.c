#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>

void sort(char list[999][999], int count) {

        for(int i = 0; i < count-1; i++) {
                for(int j = 0; j < count-i-1; j++) {
                        if(strcasecmp(list[j], list[j+1]) > 0) {
                                char *temp = malloc(999*sizeof(char));
                                strcpy(temp, list[j]);
                                strcpy(list[j], list[j+1]);
                                strcpy(list[j+1], temp);
                                free(temp);
                        }
                }
        }

}


void recurse(char *filename, int count) {

	chdir(filename);
	struct stat file;
	struct dirent *f;
	DIR *d;
	d = opendir(".");
	char arr[999][999];
	int x = 0;

	while((f = readdir(d)) != NULL) {
		if((f->d_name)[0] != '.') {
			strcpy(arr[x], f->d_name);
			x++;
		}
	}
	closedir(d);
	sort(arr, x);
	
	count+=2;

	for(int i = 0; i < x; i++) {
		stat(arr[i], &file);
		
		if(S_ISDIR(file.st_mode)) {
			for(int i = 0; i < count; i++) {
				printf(" ");
			}
			printf("-%s\n", arr[i]);
			recurse(arr[i], count);
		}
		else {
			for(int i = 0; i < count; i++) {
				printf(" ");
			}
			printf("-%s\n", arr[i]);
		}
	}
			
	chdir("..");

}

int main(int argc, char **argv) {

	DIR *d;
        struct dirent *file;
	struct stat f;
	
        d = opendir(".");
	printf(".\n");
	char arr[999][999];
	int x = 0;

	while((file = readdir(d)) != NULL) {
		if((file->d_name)[0] != '.' && (file->d_name)[strlen(file->d_name)-1] != '~') {
			strcpy(arr[x], file->d_name);
			x++;
		}
	}
	closedir(d);
	sort(arr, x);

	for(int i = 0; i < x; i++) {
		stat(arr[i], &f);
		if(S_ISDIR(f.st_mode)) {
			printf("-%s\n", arr[i]);
			recurse(arr[i], 0);
		}
		else
			printf("-%s\n", arr[i]);
	}

}
