#define _BSD_SOURCE
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#define check_null(x) if(x == NULL){perror("Bad alloc\n"); exit(1);}

//**************************************************************************************
//cat
//**************************************************************************************
void cat(int argc, char** argv)
{
    if(argc < 0)
        perror("Bad arguments\n");

    int BUFFER_SIZE = 64;
    if(argc - 1 >= 0 && !strcmp("-u", argv[argc - 1]))
    {
        BUFFER_SIZE = 1;
        --argc;
    }

    char buffer[BUFFER_SIZE + 1];

    //file == stdin
    if(argc == 0)
    {
        int input_descriptor = 0;
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

    }
    else
    {
        for(int i = 0; i < argc; ++i)
        {
            if(argv[i] == NULL)
            {
                fprintf(stderr,"'%s' not found\n", argv[i]);
            }
            else
            {
               int input_descriptor = open(argv[i], O_RDONLY);
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
            }
        }
    }
}


//**************************************************************************************
//wc
//**************************************************************************************
struct amount
{
    long int ch_amount;     //current amount chars
    long int w_amount;      //current amount words
    long int str_amount;	//current amount strings

};

void wc(int argc, char** argv)
{
    if(argc < 0)
        perror("Bad arguments\n");

    int is_stdin = 0;
    char ch[1];

    //stdin
    if(argc == 0)
    {
        ++argc;
        is_stdin = 1;
    }

    for(int i = 0; i < argc; ++i)
    {
        int input_descriptor = -1;

        if(is_stdin == 1)
        {
            input_descriptor = 0;
        }
        else if(argv[i] == NULL)
        {
            fprintf(stderr,"'%s' not found\n", argv[i]);
        }
        else
        {
           input_descriptor = open(argv[i], O_RDONLY);
        }

        if(input_descriptor < 0)
        {
            perror("Can't open file\n");
            exit(1);
        }

        int read_bytes;
        struct amount am;
        am.str_amount = 0;
        am.w_amount = 0;
        am.ch_amount = 0;
        int word = 0;      	//true when get one or more chars in a row

        while( (read_bytes = read(input_descriptor,ch, 1)) > 0)
        {
            //read all words
            //new string
            if(*ch == '\n')
            {
                if(word == 1)
                {
                    ++am.w_amount;
                    word = 0;
                }
                ++am.str_amount;
                word = 0;
            }
            //add "ch" in "word"
            else if(!isspace(*ch) && *ch != '\n' && *ch != '\t')
            {
                word = 1;
            }
            //add "word" in "all"
            else if(word == 1)
            {
                ++am.w_amount;
                word = 0;
            }
            ++am.ch_amount;
        }

        if(read_bytes < 0)
        {
           perror("Can't read\n");
           exit(1);
        }

        //add last word
        if(word == 1)
        {
            ++am.w_amount;
            word = 0;
        }
        printf("%li\t%li\t%li\n", am.str_amount, am.w_amount, am.ch_amount);
    }

}

//**************************************************************************************
//ls
//**************************************************************************************
char* months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void write_statistic(char *name, struct stat *st)
{
    int mode = st->st_mode;
    char str[] = "----------";

    if(S_ISBLK(mode))str[0]='b';
    if(S_ISCHR(mode))str[0]='c';
    if(S_ISDIR(mode))str[0]='d';
    if(mode & S_IFLNK)str[0]='l';
    if(S_ISBLK(mode))str[0]='p';
    if(S_ISBLK(mode))str[0]='s';
    if(mode & S_IFREG)str[0]='-';
    if(mode & S_IRUSR)str[1]='r';
    if(mode & S_IWUSR)str[2]='w';
    if(mode & S_IXUSR)str[3]='x';
    if(mode & S_IRGRP)str[4]='r';
    if(mode & S_IWGRP)str[5]='w';
    if(mode & S_IXGRP)str[6]='x';
    if(mode & S_IROTH)str[7]='r';
    if(mode & S_IWOTH)str[8]='w';
    if(mode & S_IXOTH)str[9]='x';

    printf("%s %i\t", str, st->st_nlink);

    struct passwd *pw;
    pw = getpwuid(st->st_uid);
    if(pw == NULL)
    {
        perror("Getpwuid\t");
    }
    else
    {
        printf("%s\t", pw->pw_name);
    }

    struct group *grp;
    grp = getgrgid(st->st_gid);
    if(pw == NULL)
    {
        perror("Getpwuid\t");
    }
    else
    {
        printf("%s\t", grp->gr_name);
    }

    printf("%i\t", st->st_size);


    struct tm * ftime = localtime(&st->st_mtime);

    if (ftime == NULL)
    {
        perror("Localtime\t");
    }
    else
    {
        printf("%s %2d %02d:%02d %s\n", months[ftime->tm_mon], ftime->tm_mday, ftime->tm_hour, ftime->tm_min, name);
    }
}

void handle_directory(char* name_dir)
{
    struct dirent **name_list;
    struct stat st;

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
                    printf("%s>\n", name_list[i]->d_name);
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

        printf("%s>\n", name_dir);

        for(int i = 2; i < count; i++)
        {
            char path[PATH_MAX];
            if(stat(path, &st) == -1)
            {
                perror("Stat\t");
            }
            else
            {
                write_statistic(name_list[i]->d_name, &st);
            }
        }


        while(count--)
        {
            free(name_list[count]);
        }
        free(name_list);
        printf("\n");
    }
}

//**************************************************************************************
//main
//**************************************************************************************
int main(int argc, char** argv)
{
    if(argc < 1)
    {
        perror("What are you want to run?\n");
        exit(1);
    }

    if(argv == NULL || argv[1] == NULL)
    {
        perror("argv == NULL");
        exit(1);
    }

    if(!strcmp("cat", argv[1]))
    {
        cat(argc - 2, argv+2);
    }
    else if(!strcmp("grep", argv[1]))
    {
    }
    else if(!strcmp("ls", argv[1]))
    {
        if (argc == 2)
        {
            handle_directory(".");
        }
        else
        {
           for(int i = 2; i < argc; i++)
           {
               handle_directory(argv[i]);
           }
        }
    }
    else if(!strcmp("wc", argv[1]))
    {
        wc(argc - 2, argv+2);
    }
    else
    {
        fprintf(stderr,"Command:'%s' not found\n",argv[1]);
        exit(1);
    }

    return 0;
}
