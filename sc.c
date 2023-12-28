// Silicon Chant
// a toolbox for magic on the command line
// typical attempts at computer magic use the computer as a tool like a deck or candle.
// this is an attempt to make the computer do magic itself.
// a computer sigil is not an image file depicting a sigil. computers don't know what an image is.
// a computer sigil is the only thing a computer understands: a binary string, partitioned into bytes.

// known bugs:
// - null bytes can appear as part of a sigil, but because sigils are stored as char*s they get terminated early if this happens

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>

int interrupt_handler(int v);
int loop();
char* make_sigil(char* phrase);
void chant_sigil(char* sigil, char* kind);
void stack_chant(char* sigil);
void geomantic_signs(char* sigil);
int get_nth_bit(char character, int n);
void print_help(char* topic);
void print_general_help();
void make_lowercase(char* sigil);

char* current_sigil = "(none)";

int main() {
  return loop();
}

int loop() {
  while (1) {
    printf("sc ¬ ");
    char buffer[100];
    char command[100];
    fgets(buffer, 100, stdin);
    sscanf(buffer, "%[^\n]", command); // take only until before newline
    make_lowercase(command);
    if (strncmp(command, "sigil ", 6) == 0) {
      current_sigil = make_sigil(command + 6);
      printf("current sigil: %s\n", current_sigil);
    } else if (strncmp(command, "sigil", 5) == 0) {
      printf("current sigil: %s\n", current_sigil);
    } else if (strncmp(command, "chant ", 6) == 0) {
      chant_sigil(current_sigil, command + 6);
    } else if (strncmp(command, "geomantic", 9) == 0) {
      geomantic_signs(current_sigil);
    } else if (strncmp(command, "help ", 5) == 0) {
      print_help(command + 5);
    } else if (strncmp(command, "help", 4) == 0) {
      print_general_help();
    } else if (strncmp(command, "end", 3) == 0) {
      return 0;
    } else {
      printf("command not recognised: %s\n", command);
    }
  }
}

char* make_sigil(char* phrase) {
  // make a square out of the bytes in the string, filling any excess with nulls
  // then iteratively xnor the lines together
  // eg "hello world"
  // h e l l
  // o   w o
  // r l d 0
  // 0 0 0 0
  // we use xnor because if the input is plain seven-bit ascii, just xor leaves the high bit 0 always
  // with xnor the high bit instead depends on whether the side length of the square is even or odd, allowing xascii characters to appear

  // find the minimal side length of the square
  int length = 0;
  int phrase_length = strlen(phrase);
  while (length * length < phrase_length) {
    length++;
  }

  char* result = malloc(length);
  for (int i = 0; i < length; i++) {
    result[i] = 0;
  }

  // repeated xor
  int pointer = 0;
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length; j++) {
      if (pointer >= phrase_length) {
        result[j] = result[j] ^ 0; // no effect
      } else {
        result[j] = ~(result[j] ^ phrase[pointer]);
        pointer++;
      }
    }
  }

  return result;
}

void chant_sigil(char* sigil, char* kind) {
  // chanting consists of meditating on a sigil by repeatedly processing it

  if (strcmp(kind, "heap") == 0) {
    // - heap chant -
    // the heap chant works by writing the sigil repeatdly to the heap
    // this chant ends in a moment of rupture/gnosis when the heap memory runs out
    int length = strlen(sigil);
    while (1) {
      char* place = malloc(length);
      strncpy(place, sigil, length); // note use of strncpy to avoid null-terminating, we focus on only the sigil without processing nulls
    }
  } else if (strcmp(kind, "stack") == 0) {
    // - stack chant -
    // the stack chant works by recursively generating stack frames, each of which has a copy of the sigil in its locals
    // this chant ends in a moment of rupture/gnosis when the stack memory runs out
    stack_chant(sigil);
  } else if (strcmp(kind, "stdout") == 0) {
    // - stdout chant -
    while (1) {
      printf("%s", sigil);
    }
  } else if (strcmp(kind, "stderr") == 0) {
    // - stderr chant -
    // the stdout chant works by writing the sigil repeatedly to stderr
    // this chant ends when the user cancels it or kills the program
    // the utility of this chant is to pipe the chant output on to another process while retaining the ability to interact with the sc interface
    while (1) {
      fprintf(stderr, "%s", sigil);
    }
  } else if (strncmp(kind, "disk", 4) == 0) {
    // - disk chant -
    // the disk chant works by writing the sigil repeatedly to a given filepath, probably but not necessarily a block device
    // this chant ends in a moment of rupture/gnosis when the device being written to runs out of space
    // it could be used to consecrate a new disk, or make a portable usb drive into a talisman
    FILE* fptr = fopen(kind + 5, "w");
    if (fptr == NULL) {
      printf("could not open device: %s\n", kind + 5);
    } else {
      printf("WARNING: if %s is a location on a disk, all space on the disk will be filled. if it is a block device, the filesystem will be overwritten. are you sure you want to do this? if you know what you're doing, enter 'commence'.\n¬ ", kind + 5);
      char buffer[10];
      fgets(buffer, 10, stdin);
      if (strncmp(buffer, "commence", 8) == 0) {
        while (1) {
          fprintf(fptr, "%s", sigil);
        }
      } else {
        printf("chant aborted\n");
      }
    }
  } else if (strncmp(kind, "net", 3) == 0) {
    // - net chant -
    // the net chant works by sending the sigil repeatedly in udp packets to the specified address
    // this chant ends when the user cancels it or kills the program
    struct sockaddr_in servaddr;
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
      printf("could not open socket\n");
    } else {
      memset(&servaddr, 0, sizeof(servaddr));
      servaddr.sin_family = AF_INET; // IPv4
      servaddr.sin_addr.s_addr = inet_addr(kind + 4);
      servaddr.sin_port = htons(888); // use port 888
      int try_connect = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
      if (try_connect < 0) {
        printf("could not connect socket (is %s a valid ip address?)", kind + 4);
      } else {
        while (1) {
          sendto(sockfd, (const char *)sigil, strlen(sigil), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        }
      }
    }
  } else {
    printf("chant type not recognised: %s\n", kind);
  }
}

void stack_chant(char* sigil) {
  char local_sigil[strlen(sigil)];
  strcpy(local_sigil, sigil);
  stack_chant(local_sigil);
}

void geomantic_signs(char* sigil) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < strlen(sigil); j++) {
      if (get_nth_bit(sigil[j], 0 + i)) { printf("# #  "); } else { printf(" #   "); }
      if (get_nth_bit(sigil[j], 4 + i)) { printf("# #  "); } else { printf(" #   "); }
    }
    printf("\n");
  }
}

int get_nth_bit(char character, int n) {
  return (character >> n) & 0x01;
}

void print_help(char* topic) {
  if (strcmp(topic, "sigil") == 0) {
    printf("- sigil -\n");
    printf("the sigil command sigilises a phrase and sets the current working sigil to the result.\n");
    printf("a computer sigil is not an image file depicting a sigil. computers don't know what an image is.\n");
    printf("a computer sigil is the only thing a computer understands: a binary string, partitioned into bytes.\n");
    printf("the sigilising algorithm is as follows:\n");
    printf("the input bytes are arranged into a square, padded with nulls if necessary.\n");
    printf("then the rows of the square are iteratively bitwise XNORed to produce a sigil the length of one row.\n");
  } else if (strcmp(topic, "chant") == 0) {
    printf("- chant -\n");
    printf("chanting consists of the computer meditating on a sigil by repeatedly processing it.\n");
    printf("the following kinds of chant are provided:\n");
    printf("heap\t: the sigil is written to the heap until a moment of rupture/gnosis occurs when the heap memory runs out\n");
    printf("stack\t: the sigil is recursively written to the stack until a moment of rupture/gnosis occurs when the stack memory runs out\n");
    printf("stdout\t: the sigil is written to stdout until the program is cancelled.\n");
    printf("stderr\t: the sigil is written to stderr instead. if stderr is piped onwards, eg to aplay, you can still use the sc interface.\n");
    printf("disk <path>\t: takes a block device as an argument, to which the sigil is written until space runs out.\n");
    printf("net <addr>\t: takes an ip address as an argument, to which the sigil is sent as a stream of udp packets on port 888.\n");
  } else {
    printf("unrecognised help topic: %s\n", topic);
    print_general_help();
  }
}

void print_general_help() {
  printf("- general help -\n");
  printf("sigil\t\t: report the current sigil\n");
  printf("sigil <string>\t: sigilise the given string and set the current sigil accordingly\n");
  printf("chant <type>\t: chant a sigil in the given manner\n");
  printf("geomantic\t: interpret the current sigil as a series of four-bit geomantic symbols\n");
  printf("end\t\t: close sc\n");
  printf("help\t\t: print this text\n");
  printf("help <command>\t: give more in-depth help about a specific option\n");
}

// https://stackoverflow.com/questions/2661766
void make_lowercase(char* target) {
  for(int i = 0; target[i]; i++){
    target[i] = tolower(target[i]);
  }
}
