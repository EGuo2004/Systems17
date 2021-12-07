#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SEMKEY 24602
#define SHMKEY 24603

void check(int val) {
    if (val == -1) {
        printf("error: %s\n", strerror(errno));
        exit(errno);
    }
}

int main() {
    int semd = semget(SEMKEY, 0, 0);
    check(semd);
    int shmd = shmget(SHMKEY, 0, 0);
    check(shmd);
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;
    semop(semd, &sb, 1);

    int *hi = shmat(shmd,0,0);
    check(hi);

    int file = open("temp.txt", O_RDWR | O_APPEND);
    check(file);
    lseek(file,-1 * *hi,SEEK_END);
    char line[4096];
    int temper = read(file,line,*hi);
    check(temper);

    line[*hi]='\0';
    printf("Previous: %s\n",line);
    printf("New line: ");
    fgets(line,4096,stdin);
    *hi = strlen(line);

    file = open("phone.txt", O_WRONLY | O_APPEND);
    check(file);
    temper = write(file,line,strlen(line));
    check(temper);

    shmdt(hi);

    sb.sem_op = 1; 
    semop(semd, &sb, 1); 

    return 0;
}
