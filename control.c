#include "semaphone.h"
int main(int argc, char *argv[]){
  int semd;
  int *data;
  int shmd;
  if(strcmp(argv[1], "-c") == 0){
    int v, r;
    semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
    if (semd == -1) {
      printf("error %d: %s\n", errno, strerror(errno));
      semd = semget(KEY, 1, 0);
      v = semctl(semd, 0, GETVAL, 0);
      //printf("semctl returned: %d\n", v);
    }
    else {
      union semun us;
      us.val = 1;
      r = semctl(semd, 0, SETVAL, us);
      //printf("semctl returned: %d\n", r);
    }
    printf("semaphore created\n");
    shmd = shmget(KEY, sizeof(int), IPC_CREAT | 0640);
    if (shmd < 0){
      printf("errno %d error: %s\n", errno, strerror(errno));
    }
    data = shmat(shmd, 0, 0);
    *data = 0;
    int err = shmdt(data);
    if (err < 0){
      printf("errno %d error: %s\n", errno, strerror(errno));
    }
    printf("shared memory created\n");
    int fd = open("story.txt", O_CREAT, 0666);
    if (fd < 0){
      printf("errno %d error: %s\n", errno, strerror(errno));
    }
    printf("file created\n");
    close(fd);
    return 0;
  }
  else if (strcmp(argv[1], "-r") == 0){
    printf("Trying to get in...\n");
    semd = semget(KEY, 1, 0);
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    semop(semd, &sb, 1);
    printf("The story so far: \n");
    int fd = open("story.txt", O_RDONLY);
    if (fd < 0){
      printf("errno %d error: %s\n", errno, strerror(errno));
    }
    struct stat stats;
    stat("story.txt", &stats);
    int size = stats.st_size;
    char buff[2000];
    int err = read(fd, buff, 2000);
    if (err < 0){
      printf("errno %d error: %s\n", errno, strerror(errno));
    }
    if (size == 0){
      printf("\n");
    }
    else{
      printf("%s\n", buff);
    }
    close(fd);
    shmdt(data);
    shmctl(shmd, IPC_RMID, 0);
    printf("shared memory removed\n");
    remove("story.txt");
    printf("file removed\n");
    semctl(semd, IPC_RMID, 0);
    printf("semaphore removed\n");
    return 0;
  }
  else if (strcmp(argv[1], "-v") == 0){
    printf("The story so far: \n");
    int fd = open("story.txt", O_RDONLY);
    if (fd < 0){
      printf("errno %d error: %s\n", errno, strerror(errno));
    }
    struct stat stats;
    stat("story.txt", &stats);
    int size = stats.st_size;
    char buff[2000];
    int err = read(fd, buff, 2000);
    if (err < 0){
      printf("errno %d error: %s\n", errno, strerror(errno));
    }
    if (size == 0){
      printf("\n");
    }
    else{
      printf("%s\n", buff);
    }
    close(fd);
    return 0;
  }
  else{
    printf("Wrong input. Use -c to create, -r to remove, -v to view.\n");
    return 0;
  }
}
