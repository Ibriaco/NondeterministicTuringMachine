#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 16384

char cState[5];
char rChar = '_';
char wChar = '_';
char dir;
char nState[5];
long int maxSteps;
char *tape = NULL;
int inList = 0;

typedef struct transition {
    long int currentState;
    char readChar;
    char writtenChar;
    char direction;
    long int nextState;
    struct transition *next;
} Transition;

Transition *transitionPointer = NULL;

typedef struct hash{
    Transition *head;
} HashTable;

HashTable *hashTable = NULL;

typedef struct state{
    long int accState;
    struct state *right;
} State ;

State *myState = NULL;

typedef struct node {
    Transition *t;
    char *string;
    int index;
    struct node *next;
    int arraySize;
} Node;

Node *computationList = NULL;
Node *lastElement = NULL;

Node *insertInList(Transition *tr, char *array, int index, int length){
    if(computationList == NULL){
        computationList = (Node*)malloc(sizeof(Node));
        computationList->t = tr;
        computationList->index = index;
        computationList->string = malloc(sizeof(char)*(lenght+1));
        computationList->next = NULL;
        memcpy(computationList->string, array, (size_t) (lenght + 1));
        computationList->arraySize = lenght;
        lastElement = computationList;
    }
    else{
        Node *temp;
        temp = (Node*)malloc(sizeof(Node));
        temp->t = tr;
        temp->index = index;
        temp->string = malloc(sizeof(char)*(lenght+1));
        temp->next = NULL;
        memcpy(temp->string, array, (size_t) (lenght + 1));
        temp->arraySize = lenght;
        lastElement->next = temp;
        lastElement = lastElement->next;
    }
    inList++;
    return computationList;
}

Node *deleteFromList(){
    Node *temp;
    if(computationList == NULL)
        return NULL;
    temp = computationList;
    computationList = computationList->next;
    free(temp);
    inList--;

    return computationList;

}

void splitInput(char *token, char *input) {
    token = strtok(input, " ");
    strcpy(cState, token);
    token = strtok(NULL, " ");
    rChar = *token;
    token = strtok(NULL, " ");
    wChar = *token;
    token = strtok(NULL, " ");
    dir = *token;
    token = strtok(NULL, " ");
    strcpy(nState, token);
}

Transition *newTransition(char *currentState, char readChar, char writtenChar, char direction, char *nextState) {

    Transition *tr = NULL;
    tr = (Transition*)malloc(sizeof(Transition));

    if (tr == NULL)
        return NULL;

    tr->currentState = strtol(currentState, NULL, 10);
    tr->readChar = readChar;
    tr->writtenChar = writtenChar;
    tr->direction = direction;
    tr->nextState = strtol(nextState, NULL, 10);
    tr->next = NULL;

    return tr;
}


void insertInHashTable(char *input){
    char token[5];
    splitInput(token, input);

    Transition *transition = newTransition(cState, rChar, wChar, dir, nState);
    long int hashIndex = transition->currentState;

    if(!hashTable[hashIndex].head){
        hashTable[hashIndex].head = transition;
        return;
    }

    transition->next = hashTable[hashIndex].head;
    hashTable[hashIndex].head = transition;
}

int isAcceptance(long int value) {
    State *s = myState;
    if(s==NULL)
        return 0;
    int flag = 0;
    while (s != NULL && flag == 0) {
        if (s->accState == value)
            flag = 1;
        else
            s = s->right;
    }
    return flag;
}

void parseAcceptanceStates(char *input){
    State *s = NULL;
    if(myState == NULL)
    {
        State *newState = (State*)malloc(sizeof(State));
        newState->right = NULL;
        newState->accState = strtol(input, NULL, 10);
        myState = newState;
    }
    else{
        s = myState;
        while(s->right != NULL)
            s = s->right;

        State *newState = (State*)malloc(sizeof(State));
        newState->right = NULL;
        newState->accState = strtol(input, NULL, 10);
        s->right = newState;
    }
}

void parseMaxSteps(char *input) {
    maxSteps = strtol(input, NULL, 10);
}

void saveTape(char *input) {
    tape = malloc(sizeof(char)*(strlen(input)+1));
    tape[0] = '\0';
    strcpy(tape, input);
}

void clearInput(){
    char c;
    while ((c = getchar()) != '\n' && c!= EOF) { }
}

char* rightShift(char *array, int *lenght){
    char *newArray = malloc(sizeof(char)*(*lenght+257));
    for(int i=0;i<256;i++)
        newArray[i] = '_';
    newArray[256]='\0';
    strcat(newArray,array);
    free(array);
    array = malloc(sizeof(char)*(*lenght+256+1));
    //strcpy(array,newArray);
    memcpy(array, newArray, (size_t) (*lenght + 257));
    free(newArray);
    *lenght = *lenght+256;
    return array;
}

char* leftShift(char *array, int *length){
    array = realloc(array, sizeof(char)*(*lenght+257));
    strcat(array, "________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________");
    *lenght = *lenght+256;
    return array;

}

void moveToEnd(){
    if(computationList == NULL || computationList->next == NULL)
        return;

    lastElement->next = computationList;
    computationList = computationList->next;
    lastElement = lastElement->next;
    lastElement->next = NULL;
}

void compute(char *input){
    saveTape(input);
    int i = 0;
    char result = '0';
    long int counter = 0;
    transitionPointer = hashTable[0].head;
    Transition *ptr = transitionPointer;
    Transition *temp;
    int tempSize = (int)strlen(tape);
    while(ptr!=NULL){
        if(ptr->readChar == tape[i])
            computationList = insertInList(ptr, tape, i, tempSize);

        ptr = ptr->next;
    }
    if(computationList == NULL){
        printf("0\n");
        return;
    }
    free(tape);
    while(computationList!=NULL) {
        int cycle = inList;
        for(int j=cycle; j>0; j--){
            if(isAcceptance(computationList->t->nextState)!=0){
                printf("1\n");
                while(computationList!=NULL) {
                    free(computationList->string);
                    computationList = deleteFromList();
                }
                return;
            }
            if(counter > maxSteps){
                printf("U\n");
                while(computationList!=NULL) {
                    free(computationList->string);
                    computationList = deleteFromList();
                }
                return;
            }
            else if(computationList->t->currentState == computationList->t->nextState && inList==1 && computationList->t->direction == 'S'){
                free(computationList->string);
                computationList = deleteFromList();
                printf("U\n");
                return;
            }
            else if(computationList->t->currentState == computationList->t->nextState && computationList->t->readChar == '_' && (computationList->t->direction == 'R'||computationList->t->direction == 'L')){
                result = 'U';
                free(computationList->string);
                computationList = deleteFromList();
            }
            else if(computationList->t->currentState == computationList->t->nextState && computationList->t->direction == 'S'){
                result = 'U';
                free(computationList->string);
                computationList = deleteFromList();
            }
            else{
                char direction = computationList->t->direction;
                computationList->string[computationList->index] = computationList->t->writtenChar;
                switch (direction) {
                    case 'R':
                        computationList->index++;
                        if (computationList->index == computationList->arraySize)
                            computationList->string = leftShift(computationList->string, &computationList->arraySize);
                        break;
                    case 'L':
                        computationList->index--;
                        if (computationList->index == -1) {
                            computationList->string = rightShift(computationList->string, &computationList->arraySize);
                            computationList->index = 255;
                        }
                        break;
                    default:
                        break;
                }

                temp = hashTable[computationList->t->nextState].head;
                int f = 0;
                while (temp != NULL) {
                    if (f == 0 && temp->readChar == computationList->string[computationList->index]){
                        computationList->t = temp;
                        f = 1;
                    }
                    else if (f!=0 && temp->readChar == computationList->string[computationList->index])
                        computationList = insertInList(temp, computationList->string, computationList->index, computationList->arraySize);

                    temp = temp->next;
                }
                if(f==0) {
                    free(computationList->string);
                    computationList = deleteFromList();
                }
                else
                    moveToEnd();
            }
        }
        counter++;
    }
    printf("%c\n", result);
    while(computationList != NULL) {
        free(computationList->string);
        computationList = deleteFromList();
    }
}

int main(void){
    char input[50];
    char *string = NULL;
    hashTable = (HashTable*)calloc(MAX, sizeof(HashTable));
    scanf("%s", input);
    getchar();
    scanf("%[^\n]s", input);
    while (strcmp(input, "acc") != 0) {
        insertInHashTable(input);
        getchar();
        scanf("%[^\n]s", input);
    }
    getchar();
    scanf("%[^\n]s", input);
    while (strcmp(input, "max") != 0) {
        parseAcceptanceStates(input);
        getchar();
        scanf("%[^\n]s", input);
    }

    getchar();

    if (strcmp(input, "run") != 0) {
        scanf("%[^\n]s", input);
        getchar();
        parseMaxSteps(input);
        clearInput();
    }

    while (scanf("%ms", &string) != EOF) {
        compute(string);
        free(string);
        if(feof(stdin))
            return 0;
    }
    return 0;
}