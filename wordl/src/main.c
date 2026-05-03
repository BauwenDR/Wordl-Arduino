#include <vma209.h>

#include <wordlist.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>

#define START_TRANSMITION 0x02
#define END_TRANSMITION 0x03
#define REQUEST_ACK 0x06

typedef enum {
  WORD_GAME,
  NUMBER_GAME
} GameType;

typedef enum {
  CORRECT,
  WRONG_LOCATION,
  NOT_PRESENT
} FeedbackCode;

struct GameRound {  //linked list structure for saving rounds sequentially
  const char* wordToGuess;
  uint8_t amountOfAttempts;
  uint8_t wordFound;
  struct GameRound *next;
};

struct GameRound *firsrtRound;

uint8_t syncedToApp = 0;
char lastSerialInput = ' ';

void selectGameType(GameType *type);
const char* getWord(const GameType *type);
uint8_t checkWord(FeedbackCode returnValues[], const char* input, const char* correct);
void getInput(const GameType *type, char *guess);

void addGameRound(const char* wordToGuess, uint8_t attempts, uint8_t wordFound);
void printHistory(void);

ISR(USART_RX_vect) {
  lastSerialInput = UDR0;
  ldFlip(3);
}

//put as many strings as possible in program memory instead of in ram
const char titlePart0[] PROGMEM = ":::       :::  ::::::::  :::::::::  :::::::::  :::        :::::::::::\n";
const char titlePart1[] PROGMEM = ":+:       :+: :+:    :+: :+:    :+: :+:    :+: :+:        :+:        \n";
const char titlePart2[] PROGMEM = "+:+       +:+ +:+    +:+ +:+    +:+ +:+    +:+ +:+        +:+        \n";
const char titlePart3[] PROGMEM = "+#+  +:+  +#+ +#+    +:+ +#++:++#:  +#+    +:+ +#+        +#++:++#   \n";
const char titlePart4[] PROGMEM = "+#+ +#+#+ +#+ +#+    +#+ +#+    +#+ +#+    +#+ +#+        +#+        \n";
const char titlePart5[] PROGMEM = "#+#+# #+#+#  #+#    #+# #+#    #+# #+#    #+# #+#        #+#         \n";
const char titlePart6[] PROGMEM = "###   ###    ########  ###    ### #########  ########## ##########   \n";
const char titlePart7[] PROGMEM = "-----------------------------------------------------------------------------------\n";
const char titlePart8[] PROGMEM = "Klik op knop 2 om verder te gaan.\n";
const char titlePart9[] PROGMEM = "-----------------------------------------------------------------------------------\n";

PGM_P const title_table[] PROGMEM = {
  titlePart0, titlePart1, titlePart2, titlePart3, titlePart4, titlePart5, titlePart6, titlePart7, titlePart8, titlePart9
};

const char descPart00[] PROGMEM = "selecteer een gamemode, woorden (word) of nummers (nbr): \n";
const char descPart01[] PROGMEM = "Klik op knop 1 of 3 om de speelmode te verandere nen op knop 2 om te starten\n";
const char descPart02[] PROGMEM = "-----------------------------------------------------------------------------------\n";
const char descPart03[] PROGMEM = "selecteer het woord of getal teken per teken.\n";
const char descPart04[] PROGMEM = "druk op knop 1 om het teken met 1 te verlagen en op knop 3 om het met 1 te verhogen\n";
const char descPart05[] PROGMEM = "Ga naar het volgende teken door op knop 2 te drukken\n";
const char descPart06[] PROGMEM = "-----------------------------------------------------------------------------------\n";
const char descPart07[] PROGMEM = "De leds na een gok betekenen:\n";
const char descPart08[] PROGMEM = "\taan = teken op juiste plaats.\n";
const char descPart09[] PROGMEM = "\tsnel blinken: teken komt voor maar staat niet op de juiste plaats.\n";
const char descPart10[] PROGMEM = "\ttraag blinken: teken komt niet voor\n";
const char descPart11[] PROGMEM = "Klik vervolgens op knop 2 om opnieuw te raden\n";

PGM_P const desc_table[] PROGMEM = {
  descPart00, descPart01, descPart02, descPart03, descPart04, descPart05, descPart06, descPart07, descPart08, descPart09,
  descPart10, descPart11
};

char buffer[85];

int main() {
  initVma209();

  //print wordle logo saves in program memory
  for(int i = 0; i < 10; ++i) {
    strcpy_P(buffer, (PGM_P)pgm_read_word(&(title_table[i])));
    printf(buffer);
  }

  //init usart interupt
  UCSR0B |= (1 << RXCIE0);
  sei();

  //set random seed + try and sync to companion app
  while(!btGetState(1) && lastSerialInput != 's') {
    dpWriteNumber(adGetValue());

    if(syncedToApp) {
      ldEnableAll();
      _delay_ms(125);
      ldDisableAll();
      _delay_ms(125);
    }else {
      if(lastSerialInput == '?') {
        ldEnable(1);
        lastSerialInput = ' ';  //reset and wait for answer
          printf("%c", REQUEST_ACK);
        _delay_ms(500); //wait for response
        if(lastSerialInput == '+') {
          ldDisable(1);
          printf("%c", REQUEST_ACK);
          syncedToApp = 1;  
        } 
      }else {
        ldEnable(0);
        _delay_ms(125);
        ldDisable(0);
        _delay_ms(125);
      }
    }
  }

  lastSerialInput = ' ';
  if(syncedToApp) {
    printf("%c", REQUEST_ACK);
  }

  while(btGetState(1)) _delay_ms(1);

  srand(adGetValue());

  //game init
  GameType gameType = WORD_GAME;
  char *guessedWord = malloc(5);

  //if not synced to app print program instructions
  if(!syncedToApp) {
    for(int i = 0; i < 12; ++i) {
      strcpy_P(buffer, (PGM_P)pgm_read_word(&(desc_table[i])));
      printf(buffer);
    }
  }

  //start game
  while(1) {
    

    selectGameType(&gameType);

    char* wordToGuess = malloc(5);
    strcpy(wordToGuess, getWord(&gameType));

    uint8_t wordFound = 0;
    uint8_t attempts = 0;

    strcpy(guessedWord, (gameType == WORD_GAME ? "AAAA" : "0000"));

    while(!wordFound && attempts < 4) {
      getInput(&gameType, guessedWord);
      attempts++;

      FeedbackCode feedBackCode[4];
      if(checkWord(feedBackCode, guessedWord, wordToGuess)) {
        //correct guess, good job ;)
        wordFound = 1;
      }else {
        //wrong guess
        if(syncedToApp) {
          char *feedbackString = malloc(5);
          strcpy(feedbackString, "cccc");
          for(int i = 0; i < strlen(feedbackString); ++i) {
            if(feedBackCode[i] == WRONG_LOCATION) {
              feedbackString[i] = 'l';
            }else if (feedBackCode[i] == NOT_PRESENT) {
              feedbackString[i] = 'w';
            }
          }
          printf("f%ss", feedbackString);
        }

        while(!btGetState(1) && lastSerialInput != REQUEST_ACK && attempts < 4) {
          dpWriteString(guessedWord);
          _delay_ms(50);
          for(int i = 0; i < strlen(wordToGuess); ++i) {
            if(feedBackCode[i] == NOT_PRESENT) dpClearSegment(i);
          }
          _delay_ms(50);
          for(int i = 0; i < strlen(wordToGuess); ++i) {
            if(feedBackCode[i] == WRONG_LOCATION) dpClearSegment(i);
          }
          _delay_ms(50);
        }
        while(btGetState(1)) _delay_ms(1);
        
        lastSerialInput = ' ';

        if(syncedToApp && attempts < 4) {
          printf("%c", REQUEST_ACK);
        }
      }
    }

    addGameRound(wordToGuess, attempts, wordFound);

    if(wordFound) {
      dpWriteString(" WIN");
      bzPlayTone(5000.0, 100);
      _delay_ms(100);
      bzPlayTone(5000.0, 100);
    }else {
      dpWriteString("FAIL");
      bzPlayTone(100.0, 1000);
      _delay_ms(100);
      bzPlayTone(100.0, 1000);
      _delay_ms(100);
      bzPlayTone(100.0, 1000);
    }

    if(!syncedToApp) {
      printf("-----------------------------------------------------------------------------------\n");
      if(wordFound) {
        printf("je hebt het woord in %d keer geraden!\n", attempts);
      }else {
        printf("het juiste woord was %s.\n", wordToGuess);
      }
    }
        
    if(syncedToApp) {
      lastSerialInput = ' ';
      while(lastSerialInput != REQUEST_ACK) {
        if(wordFound) {
         printf("g");
        }else {
          printf("b");
        }
        _delay_ms(100);
      }
    }
    lastSerialInput = ' ';
      
    while(!btGetState(1) && lastSerialInput != 'm') {
      if(lastSerialInput == '?') {
        lastSerialInput = ' ';
        _delay_ms(50);
        while(lastSerialInput != '+') {
          if(wordFound) {
            printf("%d", attempts);
          }else {
            printf("%s", wordToGuess);
          }
        _delay_ms(100);
        }
        lastSerialInput = ' ';
      }
      _delay_ms(1);
    }
    while(btGetState(1)) _delay_ms(1);
    
    if(syncedToApp) {
      printf("%c", REQUEST_ACK);
    }
  }
}

void selectGameType(GameType *type) {
  if(*type == WORD_GAME) {
    dpWriteString("word");
  }else {
    dpWriteString(" nbr");
  }

  lastSerialInput = ' ';
  while(!btGetState(1) && lastSerialInput != 'w' && lastSerialInput != 'n') {
    if(btGetState(0) || btGetState(2)) {
      if(*type == WORD_GAME) {
        *type = NUMBER_GAME;
        dpWriteString(" nbr");
      }else {
        *type = WORD_GAME;
        dpWriteString("word");
      }
      while(btGetState(0) || btGetState(2)) _delay_ms(1);
    }
    _delay_ms(1);

    if(lastSerialInput == 'h') {
      _delay_ms(100);
      ldFlip(2);
      printHistory();
      lastSerialInput = ' ';
    }
  }
  while(btGetState(1)) _delay_ms(1);

  if(lastSerialInput != ' ') {
    if(lastSerialInput == 'w') {
      *type = WORD_GAME;
    }else {
      *type = NUMBER_GAME;
    }
  }

  if(syncedToApp) {
    if(*type == WORD_GAME) {
      printf("w");
    }else {
      printf("n");
    }
  }
}

const char* getWord(const GameType *type) {
  if(*type == WORD_GAME) {
    return words[rand()%AMOUNT_OF_WORDS];
  }else {
    char *numberString;
    uint16_t number = rand()%9000 + 1000;
    sprintf(numberString, "%4d", number);
    return numberString;
  }
}

uint8_t checkWord(FeedbackCode returnValues[4], const char* input, const char* correct) {
  if(strcmp(input, correct) == 0) {
    returnValues[0] = CORRECT;
    returnValues[1] = CORRECT;
    returnValues[2] = CORRECT;
    returnValues[3] = CORRECT;

    return 1;
  }

  for(int i = 0; i < strlen(input); ++i) {
    if(input[i] == correct[i]) {
      returnValues[i] = CORRECT;
    }else if(strchr(correct, input[i])) {
      returnValues[i] = WRONG_LOCATION;
    }else {
      returnValues[i] = NOT_PRESENT;
    }
  }

  return 0;
}

void getInput(const GameType *type, char *guess) {
  //printf(guess);
  dpWriteString(guess);

  for(int i = 0; i < 4; ++i) {
    //get input
    lastSerialInput = ' ';
    while(!btGetState(1) && lastSerialInput == ' ') {

      if(btGetState(0)) {
        guess[i]--;
        if(*type == WORD_GAME && guess[i] < 'A') {
          guess[i] = 'Z';
        } 

        if(*type == NUMBER_GAME && guess[i] < '0') {
          guess[i] = '9';
        }
        while(btGetState(2)) _delay_ms(1);
      }

      if(btGetState(2)) {
        guess[i]++;
        if(*type == WORD_GAME && guess[i] > 'Z') {
          guess[i] = 'A';
        }
        if (*type == NUMBER_GAME && guess[i] > '9') {
          guess[i] = '0';
        }
        while(btGetState(0)) _delay_ms(1);
      }

      dpClearSegment(i);
      _delay_ms(100);
      dpWriteCharToSegment(guess[i], i);
      _delay_ms(100);
    }
    while(btGetState(1)) _delay_ms(1);

    if(lastSerialInput != ' ') {
      guess[i] = lastSerialInput;
      dpWriteCharToSegment(guess[i], i);
      lastSerialInput = ' ';
    }

    if(syncedToApp) {
      printf("%c", guess[i]);
      ldFlip(2);
    }
  }
}

void addGameRound(const char* wordToGuess, uint8_t attempts, uint8_t wordFound) {
  if(firsrtRound == NULL) {
    firsrtRound = (struct GameRound*) malloc(sizeof(struct GameRound));
    firsrtRound->wordToGuess = wordToGuess;
    firsrtRound->amountOfAttempts = attempts;
    firsrtRound->wordFound = wordFound;
    firsrtRound->next = NULL;
  }else {
    struct GameRound *current = firsrtRound;

    while(current->next != NULL) {
      current = current->next;
    }

    current->next = (struct GameRound*) malloc(sizeof(struct GameRound));
    current->next->wordToGuess = wordToGuess;
    current->next->amountOfAttempts = attempts;
    current->next->wordFound = wordFound;
    current->next->next = NULL;
  }
}

void printHistory() {
  struct GameRound *current = firsrtRound;

  printf("%c", START_TRANSMITION);
  while(current != NULL) {
    printf("%4s%1d%1d", current->wordToGuess, current->amountOfAttempts, current->wordFound);
    current = current->next;
  }
  printf("%c", END_TRANSMITION);
}