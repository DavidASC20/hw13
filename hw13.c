#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "hw13.h"


//reads csv file
int read_csv(){
    printf("Reading CSV file\n");
    int fd = open("./nyc_pop.csv", O_RDONLY);
    if(fd == -1){
        printf("Error, Message is :%s\n", strerror(errno));
        return -1;
    }

    struct stat status;
    int err = fstat(fd, &status);
    if(err == -1){
        printf("Error, Message is:%s\n", strerror(errno));
        return -1;
    }
    char file[status.st_size + 1];
    int err2 = read(fd, file, sizeof(file) - 1);
    if (err == -1){
        printf("Error, Message is:%s\n", strerror(errno));
        return -1;
    }
    file[sizeof(file) - 1] = '\0';

    int x;
    int line_len = 0;
    for(x = 0; x < sizeof(file); x++){
        if(file[x] == '\n'){
            line_len++;
        }
    }

    char *p = file;
    while(*p != '\n'){
        p++;
    }

    struct pop_entry data[(line_len -1) * 5];
    for(x = 0; x < (line_len - 1); x +=5){
        int year;
        int manhattan;
        int brooklyn; 
        int queens;
        int bronx;
        int staten_island;

        int match = sscanf(p, "\n%d, %d, %d, %d, %d, %d\n", &year, &manhattan, &brooklyn, &queens, &bronx, &staten_island);
        if(match != 6){
            printf("Error, no match");
            return -1;
        }

        p++;
        while (*p != '\n'){
            p++;
        }
        data[x].year = year;
		data[x].population = manhattan;
		strcpy(data[x].borough, "Manhattan");
		data[x+1].year = year;
		data[x+1].population = brooklyn;
		strcpy(data[x+1].borough, "Brooklyn");
		data[x+2].year = year;
		data[x+2].population = queens;
		strcpy(data[x+2].borough, "Queens");
		data[x+3].year = year;
		data[x+3].population = bronx;
		strcpy(data[x+3].borough, "Bronx");
		data[x+4].year = year;
		data[x+4].population = staten_island;
		strcpy(data[x+4].borough, "Staten Island");
    }
    return write_data(data, sizeof(data));
}
//writes into info.data
int write_data(struct pop_entry* data, int size){
    printf("writing info file\n");
    int f = open("./info.data", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (f == -1) {
		printf("Error, Message is:%s\n", strerror(errno));
		return -1;
	}
	int byte = write(f, data, size);
	if (byte != size){
		printf("Error writing data file: %s\n", strerror(errno));
		return -1;
	}
	printf("Success! wrote %d bytes to file: info.data\n", byte);
	return 0;
}
//reads info.data
struct pop_entry * read_data(int *num){
    int fd = open("./info.data", O_RDONLY);
    if(fd == -1){
		printf("Error, Message is:%s\n", strerror(errno));
		return NULL;
	}

    struct stat status;
    int err = fstat(fd, &status);
	if (err == -1) {
		printf("Error, Message is:%s\n", strerror(errno));
		return NULL;
	}
    
    *num = status.st_size / sizeof(struct pop_entry);
	struct pop_entry *data = malloc(status.st_size);
	err = read(fd, data, status.st_size);
	if (err == -1) {
		printf("Error, Message is:%s\n", strerror(errno));
		return NULL;
	}
	return data;
}
//serves as input
struct pop_entry input() {
	printf("Enter the year, borough, and population: ");
	char answer[1000];
	fgets(answer, sizeof(answer), stdin);
	struct pop_entry result;
	sscanf(answer, "%d %s %d\n", &result.year, result.borough, &result.population);
	return result;
}
//adds data
int add_data(){
	struct pop_entry new_data = input();
	int fd = open("./info.data", O_WRONLY | O_APPEND);
	if (fd == -1) {
		printf("Error, Message is:%s\n", strerror(errno));
		return -1;
	}
	int b = write(fd, &new_data, sizeof(new_data));
	if (b != sizeof(new_data)) {
		printf("Error, Message is:%s\n", strerror(errno));
		return -1;
	}
	printf("Success! Added the data to info.file: year: %d\tborough: %s\tpopulation: %d\n", new_data.year, new_data.borough, new_data.population);
	return 0;
}
//updates the existing data
int update_data(){
    int num = 0;
	struct pop_entry* data = read_data(&num);
	if (data == NULL){
         return -1;
    }
	print_data(num, data);
	char buf[1000];
	printf("update entry: ");
	fgets(buf, sizeof(buf), stdin);
	int ind = 0;
	sscanf(buf, "%d\n", &ind);
	struct pop_entry imp = input();
	data[ind] = imp;
	return write_data(data, num * sizeof(struct pop_entry));
}
//prints the data
void print_data(int num, struct pop_entry data[]) {
	int x;
	for (x = 0; x < num; x++) {
		printf("%d: year: %d\tborough: %s\tpopulation: %d\n", x, data[x].year, data[x].borough, data[x].population);
	}
}

int main(int argc, char *argv[]) {
	if (argc == 2) {
		if (strcmp(argv[1], "read_csv") == 0) {
			return read_csv();
		} else if (strcmp(argv[1], "read_data") == 0) {
			int count = 0;
			struct pop_entry* data = read_data(&count);
			if (data == NULL) return -1;
			print_data(count, data);
			return 0;
		} else if (strcmp(argv[1], "add_data") == 0) {
			return add_data();
		} else if (strcmp(argv[1], "update_data") == 0) {
			return update_data();
		}
	}
	printf("Choose to read_csv, read_data, add_data, or update_data\n");
	return -1;
}