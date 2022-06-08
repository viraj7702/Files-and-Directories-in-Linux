#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<dirent.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>

struct features {
	int x;
	int y;
	char permissions[100];
	char UID[999];
	long UIDNUM;
	char GID[999];
	long GIDNUM;
	long int size;
	char dt[100];
	char name[999];
};

void structsort(struct features layout[], int count) {
	

	for(int i = 0; i < count-1; i++) {
                for(int j = 0; j < count-i-1; j++) {
                        if(strcasecmp(layout[j].name, layout[j+1].name) > 0) {
				
				struct features temp;
                             
				temp.x = layout[j].x;
				temp.y = layout[j].y;
				strcpy(temp.permissions, layout[j].permissions);
				strcpy(temp.UID, layout[j].UID);
				temp.UIDNUM = layout[j].UIDNUM;
				strcpy(temp.GID, layout[j].GID);
				temp.GIDNUM = layout[j].GIDNUM;
				temp.size = layout[j].size;
				strcpy(temp.dt, layout[j].dt);
				strcpy(temp.name, layout[j].name);

				layout[j].x = layout[j+1].x;
                                layout[j].y = layout[j+1].y;
                                strcpy(layout[j].permissions, layout[j+1].permissions);
                                strcpy(layout[j].UID, layout[j+1].UID);
                                layout[j].UIDNUM = layout[j+1].UIDNUM;
                                strcpy(layout[j].GID, layout[j+1].GID);
                                layout[j].GIDNUM = layout[j+1].GIDNUM;
                                layout[j].size = layout[j+1].size;
                                strcpy(layout[j].dt, layout[j+1].dt);
                                strcpy(layout[j].name, layout[j+1].name);

				layout[j+1].x = temp.x;
                                layout[j+1].y = temp.y;
                                strcpy(layout[j+1].permissions, temp.permissions);
                                strcpy(layout[j+1].UID, temp.UID);
                                layout[j+1].UIDNUM = temp.UIDNUM;
                                strcpy(layout[j+1].GID, temp.GID);
                                layout[j+1].GIDNUM = temp.GIDNUM;
                                layout[j+1].size = temp.size;
                                strcpy(layout[j+1].dt, temp.dt);
                                strcpy(layout[j+1].name, temp.name);
                              
                        }
                }
	}
		
	for(int i = 0; i < count; i++) {
		if(layout[i].x) 
			printf("%s %ld ", layout[i].permissions, layout[i].UIDNUM); 
		else
			printf("%s %s ", layout[i].permissions, layout[i].UID);

		if(layout[i].y)
			printf("%ld %ld %s %s\n", layout[i].GIDNUM, layout[i].size, layout[i].dt, layout[i].name);
		else
			printf("%s %ld %s %s\n", layout[i].GID, layout[i].size, layout[i].dt, layout[i].name);
		
	}
}	


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

	for(int i = 0; i < count; i++) {
		printf("%s\n", list[i]);
	}
}


void list(char *filename, struct features layout[], int x) {

	struct stat file;
	stat(filename, &file);

	strcat(layout[x].permissions,((S_ISDIR(file.st_mode)) ? "d" : "-"));
	strcat(layout[x].permissions,(((file.st_mode & S_IRUSR) ? "r" : "-")));
	strcat(layout[x].permissions,(((file.st_mode & S_IWUSR) ? "w" : "-")));
	strcat(layout[x].permissions,(((file.st_mode & S_IXUSR) ? "x" : "-")));
	strcat(layout[x].permissions,(((file.st_mode & S_IRGRP) ? "r" : "-")));
	strcat(layout[x].permissions,(((file.st_mode & S_IWGRP) ? "w" : "-")));
	strcat(layout[x].permissions,(((file.st_mode & S_IXGRP) ? "x" : "-")));
	strcat(layout[x].permissions,(((file.st_mode & S_IROTH) ? "r" : "-")));
	strcat(layout[x].permissions,(((file.st_mode & S_IWOTH) ? "w" : "-")));
	strcat(layout[x].permissions,(((file.st_mode & S_IXOTH) ? "x " : "- ")));
	
	struct passwd *p = getpwuid(file.st_uid);
	if(p == NULL) { 
		layout[x].x = 1;
		layout[x].UIDNUM = ((long)file.st_uid);
	}
	else { 
		layout[x].x = 0;
		strcpy(layout[x].UID, (p->pw_name));
	}

	struct group *g = getgrgid(file.st_gid);
	if(g == NULL) { 
		layout[x].y = 1;
		layout[x].GIDNUM = (long)file.st_gid;
	}
	else {
		layout[x].y = 0;
		strcpy(layout[x].GID, g->gr_name);
	}

	layout[x].size = (long)(file.st_size);

	char modif[100];
	strftime(modif, 100, "%b %d %H:%M", localtime(&(file.st_mtime)));
	
	strcpy(layout[x].dt, modif);

}

int main(int argc, char **argv) {

	DIR *d;
	struct dirent *file;
	char elements[999][999];
	int i = 0;
	struct features layout[999];
        int x = 0;
	
	d = opendir(".");

	if(argc == 1) {
		while((file = readdir(d)) != NULL) {
			if((file->d_name)[0] != '.' && (file->d_name)[strlen(file->d_name)-1] != '~') {
				strcpy(elements[i], file->d_name);
				i++;
			}
			//	printf("%s\n", file->d_name);
		}
		sort(elements, i);
	}

	else if(argc == 2) {
		while((file = readdir(d)) != NULL) {
			if((file->d_name)[0] != '.' && (file->d_name)[strlen(file->d_name)-1] != '~') {
				strcpy(layout[x].name, file->d_name);
				list(file->d_name, layout, x);
				x++;
			}
		}
		structsort(layout, x);
	}

	closedir(d);

	return EXIT_SUCCESS;

}
