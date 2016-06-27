#define _BSD_SOURCE
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/types.h>


void write_file(char* name)
{
	printf("%s\n", name);
	
	int BUFFER_SIZE = 64;
	char buffer[BUFFER_SIZE + 1];

	int input_descriptor = open(name, O_RDONLY);
	if(input_descriptor < 0)
	{
		perror("Can't open file\n");
		exit(1);
	}

	int output_descriptor = 1;
	int read_bytes;
	int written_bytes;

	while( (read_bytes = read(input_descriptor,buffer, BUFFER_SIZE)) > 0)
	{
		buffer[read_bytes] = '\0';
		written_bytes = write(output_descriptor, buffer, read_bytes);

		if(written_bytes != read_bytes)
		{
			perror("Can't write\n");
			exit(1);
		}
	}

	if(read_bytes < 0)
	{
		perror("Can't read\n");
		exit(1);
	}
	
	printf("\n");
}


void handle_directory(char* name_dir)
{
    struct dirent **name_list;

    int count = scandir(name_dir, &name_list, NULL, alphasort);

    if (count < 0)
    {
        perror("Scandir\n");
    }
    else
    {
        for(int i = 2; i < count; ++i)
        {
            if(strcmp(name_list[i]->d_name,".") != 0 && strcmp(name_list[i]->d_name, "..") != 0 )
            {
                if(name_list[i]->d_type & DT_DIR)
                {
                    char path[PATH_MAX];
                    int length;
                    printf("%s\n", name_list[i]->d_name);
                    length = snprintf(path, PATH_MAX, "%s/%s", name_dir, name_list[i]->d_name);
                    if(length >= PATH_MAX)
                    {
                        fprintf(stderr, "Path of %s directory is very long\n", path);
                    }
                    else
                    {
                        handle_directory(path);
                    }
                }
            }
        }

        printf("%s\n", name_dir);

        for(int i = 2; i < count; i++)
        {
            write_file(name_list[i]->d_name);			
        }


        while(count--)
        {
            free(name_list[count]);
        }
        free(name_list);
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    if(argc < 1)
    {
        perror("What are you want?\n");
        exit(1);
    }

    if(argv == NULL || argv[1] == NULL)
    {
        perror("argv == NULL");
        exit(1);
    }
	

	handle_directory(argv[1]);

    return 0;
}
