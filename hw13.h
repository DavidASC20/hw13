//struct
struct pop_entry {
	int year;
	int population;
	char borough[15];
};
//main functions
int read_csv();
struct pop_entry *read_data(int *num);
int add_data();
int update_data();

//helper functions
struct pop_entry input();
void print_data(int num, struct pop_entry data[]);
int write_data(struct pop_entry* data, int size);