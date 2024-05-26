#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>

#define NUMBER_OF_CORRECT_GUESSES_BEFORE_THE_DIFFICULTY_RISE 3
#define TOTAL_NUMBER_OF_WORDS 10
#define WORD_LENGTH 20
#define DIFICULTY_LEVEL 3
#define MAX_DESCRIPTION_LENGTH 200

const int MAX_SIZE = 20;

enum {
  EASY,
  MEDIUM,
  HARD
};

void swap(char *x, char *y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

void shuffle(char *str, int len, int type) {
  int L = 1, R = len - 2;
  if (type == HARD) {
    --L, ++R; // Including the First and the Last character in the shuffle
  }
  for (int iterate = 0; iterate < (len + 1) / 2; iterate++) {
    int leftIndex = L + (rand() % (R - L + 1));
    int rightIndex = L + (rand() % (R - L + 1));
    while (leftIndex == rightIndex) {
      leftIndex = L + (rand() % (R - L + 1));
      rightIndex = L + (rand() % (R - L + 1));
    }
    swap(str + leftIndex, str + rightIndex);
  }
}

void animatedPrint(int currentDifficulty) {
  char str[MAX_SIZE];
  if (currentDifficulty == EASY) {
    strcpy(str, "EASY");
  } else if (currentDifficulty == MEDIUM) {
    strcpy(str, "MEDIUM");
  } else if (currentDifficulty == HARD) {
    strcpy(str, "HARD");
  }

  int len = strlen(str);
  for (int index = 0; index < len; index++) {
    printf("%c", str[index]);
    Sleep(300);
  }
  return;
}

void animateLevelingUp(int from, int to) {
  int flag = 0;
  while (1) {
    if (flag != 1) {
      printf("Press C to continue playing or E to exit...\n");
    }
    char cc = getch();
    cc = tolower(cc);

    if (cc != 'c' && cc != 'e') {
      flag = 1;
      continue;
    }

    if (cc == 'e') {
      printf("\n\nThanks for playing, Have a good day!!!\n");
      exit(0);
    }

    break;
  }

  system("cls");

  printf("Leveling up from "); animatedPrint(from); printf(" to "); animatedPrint(to);
  printf("\n");
}


int haveBeenUsed[DIFICULTY_LEVEL][TOTAL_NUMBER_OF_WORDS];
char wordList[DIFICULTY_LEVEL][TOTAL_NUMBER_OF_WORDS][WORD_LENGTH];
char description[TOTAL_NUMBER_OF_WORDS][MAX_DESCRIPTION_LENGTH];

void extractWords(int difficulty) {
  FILE *fp = fopen((difficulty == EASY ? "easy.txt" : (difficulty == MEDIUM ? "medium.txt" : "hard.txt")), "r");
  int count = 0;
  while (count < TOTAL_NUMBER_OF_WORDS && fscanf(fp, "%s", wordList[difficulty][count])) {
    int i = 0;
    for (i = 0; i < WORD_LENGTH && wordList[difficulty][count][i] != '\n'; i++) {
      
    }
    wordList[difficulty][count][i] = '\0';
    ++count;
  }
  fclose(fp);
  return;
}

int main() {
  
  for (int currDifficulty = EASY; currDifficulty <= HARD; currDifficulty++) {
    extractWords(currDifficulty);
  }

  FILE *fp = fopen("des.txt", "r");
  for (int i = 0; i < TOTAL_NUMBER_OF_WORDS; i++) {
    fgets(description[i], MAX_DESCRIPTION_LENGTH, fp);

    if (description[i][0] == '\n') {
      --i;
      continue;
    }

    int index = 0;
    while (index < MAX_DESCRIPTION_LENGTH && description[i][index] != '\n') {
      index++;
    }

    if (index < MAX_DESCRIPTION_LENGTH && description[i][index] == '\n') {
      description[i][index] = '\0';
    }
  }

  fclose(fp);
  
  // Initialize the haveBeenUsed array to 0, as NO round has been played yet, hence NO WORD used
  for (int difficulty = EASY; difficulty <= HARD; difficulty++) {
    for (int wordIndex = 0; wordIndex < TOTAL_NUMBER_OF_WORDS; wordIndex++) {
      haveBeenUsed[difficulty][wordIndex] = 0;
    }
  }
  
  srand(time(0));
  
  int difficulty = EASY;
  int correctGuesses = 0;
  
  
  while (1) {
    
    if (correctGuesses == NUMBER_OF_CORRECT_GUESSES_BEFORE_THE_DIFFICULTY_RISE) {
      
      printf("\x1b[33m");
      if (difficulty != HARD) {
        printf("Oh well, You're so good at this that we have decided to raise the difficulty!!!\n\n");
      }
      printf("\x1b[0m");
      
      if (difficulty == EASY) {
        difficulty = MEDIUM;
        animateLevelingUp(EASY, MEDIUM);
      } else if (difficulty == MEDIUM) {
        difficulty = HARD;
        animateLevelingUp(MEDIUM, HARD);
      } else {
        printf("Congratulations!!! You have won...\n");
        printf("\x1b[0m");
        break;
      }
      
      correctGuesses = 0; // Reset the counter for next difficulty raise
    }
    
    char correctWord[MAX_SIZE];
    int size = TOTAL_NUMBER_OF_WORDS;
    int index = rand() % size;
    
    while (haveBeenUsed[difficulty][index] == 1) {
      // 1 meaning the word has been already used in the game
      index = rand() % size;
    }
    
    haveBeenUsed[difficulty][index] = 1;
    strcpy(correctWord, wordList[difficulty][index]);
    
    int correctWordLen = strlen(correctWord);
    
    for (int i = 0; i < correctWordLen; i++) {
      correctWord[i] = tolower(correctWord[i]);
    }
    
    char shuffledWord[MAX_SIZE];
    strcpy(shuffledWord, correctWord);
    
    while (strcmp(shuffledWord, correctWord) == 0) {
      shuffle(shuffledWord, correctWordLen, difficulty);
    }
    
    printf("Here is the word (shuffled): %s\n", shuffledWord);
    if (difficulty == EASY) {
      printf("Description: %s\n\n", description[index]);
    }
    
    
    
    while (1) {
      char guessedWord[MAX_SIZE];
      printf("Enter your guess: ");
      scanf("%s", guessedWord);
      
      int guessedWordLen = strlen(guessedWord);
      
      for (int i = 0; i < guessedWordLen; i++) {
        guessedWord[i] = tolower(guessedWord[i]);
      }
      
      if (guessedWordLen == correctWordLen && strcmp(guessedWord, correctWord) == 0) {
        printf("\x1b[32m");
        printf("\nYour guess was correct!!! Welcome to the NEXT ROUND.\n\n");
        printf("\x1b[0m");
        ++correctGuesses;
        break;
      } else {
        printf("\x1b[31m");
        printf("Incorrect Guess!!! Try again\n");
        printf("\x1b[0m");
      }
    }
  }
  return 0;
}