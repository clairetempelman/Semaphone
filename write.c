#include "semaphone.h"

int main(){
  int semd;
  int shmd;
  printf("Trying to get in...\n");
  semd = semget(KEY, 1, 0);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  semop(semd, &sb, 1);
  shmd = shmget(KEY, sizeof(int), 0);
  int *data = shmat(shmd, 0, 0);
  int fd = open("story.txt", O_RDWR);
  struct stat stats;
  stat("story.txt", &stats);
  int size = stats.st_size;
  if (fd < 0){
    printf("errno %d error: %s\n", errno, strerror(errno));
  }
  int err = lseek(fd, size - *data, SEEK_SET);
  if (err < 0){
    printf("errno %d error: %s\n", errno, strerror(errno));
  }
  printf("Last addition: ");
  if (size == 0){
    printf("\n");
  }
  else {
    char buff[*data];
    err = read(fd, buff, *data);
    if (err < 0){
      printf("errno %d error: %s\n", errno, strerror(errno));
    }
    printf("%s\n", buff);
  }
  err = lseek(fd, 0, SEEK_END);
  if (err < 0){
    printf("errno %d error: %s\n", errno, strerror(errno));
  }
  printf("Your addition: ");
  char buff2[2000];
  fgets(buff2, 2000, stdin);
  char * checker;
  if ((checker = strchr(buff2, '\n')) != NULL) {
    *checker = '\0';
  }
  int length = strlen(buff2);
  err = write(fd, buff2, length);
  if (err < 0){
    printf("errno %d error: %s\n", errno, strerror(errno));
  }
  *data = strlen(buff2) + sizeof(" ");
  err = write(fd, " ", sizeof(" "));
  if (err < 0){
    printf("errno %d error: %s\n", errno, strerror(errno));
  }
  shmdt(data);
  close(fd);
  sb.sem_op = 1;
  semop(semd, &sb, 1);
  return 0;
}
