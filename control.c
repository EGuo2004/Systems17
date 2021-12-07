#include "control.h"

void check(int val) {
    if (val == -1) {
        printf("error: %s\n", strerror(errno));
        exit(errno);
    }
}

void createsem() {
    int semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    check(semd);

    union semun us;
    us.val = 1;
    int r = semctl(semd, 0, SETVAL, us);
    check(r);

    int shmd = shmget(SHMKEY, sizeof(int), IPC_CREAT | 0644);
    check(shmd);

    int fd = open("temp.txt", O_CREAT | O_TRUNC, 0644);
    check(fd);
}

void deletesem() {
    int semd = semget(SEMKEY,1,0);
    check(semd);
    semctl(semd,IPC_RMID,0);
    int shmd = shmget(SHMKEY,1,0);
    check(shmd);
    shmctl(shmd,IPC_RMID,0);

    int file = open("temp.txt",O_RDONLY);
    check(file);

    struct stat st;
    char* buff = stat("temp.txt", &st);
    buff[st.st_size] = '\0';
    read(file,buff,st.st_size);
    printf("Story:\n%s",buff);
    free(buff);
    close(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("-create or -remove\n");
        return -1;
    }
    if (!strcmp(argv[1], "-create")) {
        return create();
    }
    else if (!strcmp(argv[1], "-remove")) {
        return delete();
    }
    else {
        printf("-create or -remove\n");
        return -1;
    }
}

