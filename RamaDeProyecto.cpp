#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LEN_STR 50

typedef struct{
	int id;
	char name[MAX_LEN_STR];
	char nickname[MAX_LEN_STR];
	int age;
	int points;
}Player;

typedef struct PlayerNode{
	Player player;
	PlayerNode *next;
	PlayerNode *prev;
}PlayerNode;

//Lista de personajes
typedef struct {
    char name[MAX_LEN_STR];
    int victories;
} Characters;

typedef struct CharacterNode {
    Characters character;
    struct CharacterNode *next;
    struct CharacterNode *prev;
} CharacterNode;

//Prototypes
//General
void cleanBuffer();
//Player
Player createPlayer();
PlayerNode* createPlayerNode(Player);
void addPlayer(PlayerNode**);
void savePlayersToFile(PlayerNode*);
void printAllPlayers(PlayerNode*);
void loadFromFile(PlayerNode**);
/*
......... MAIN ....................
*/
int main(){
	PlayerNode *headPlayer = NULL;
	addPlayer(&headPlayer);
	addPlayer(&headPlayer);
	addPlayer(&headPlayer);
	savePlayersToFile(headPlayer);
	loadFromFile(&headPlayer);
	printAllPlayers(headPlayer);
	return 0;
}

/*
......... PLAYER FUNCTIONS ........
*/
Player createPlayer(){
	Player newPlayer;
	newPlayer.id = 0; // change this line
	
	printf("\nDame el nombre del jugador:");
	fgets(newPlayer.name, MAX_LEN_STR, stdin);
	newPlayer.name[strcspn(newPlayer.name, "\n")] = 0;
	printf("\nDame el nickname sin espacios: ");
	scanf("%s", newPlayer.nickname);
	printf("\nDame edad: ");
	scanf("%d", &newPlayer.age);
	newPlayer.points = 0;
	
	cleanBuffer();
	return newPlayer;
}

PlayerNode* createPlayerNode(Player newPlayer){
    PlayerNode *newPlayerNode;
	newPlayerNode = (PlayerNode*)malloc(
							sizeof(PlayerNode));
    newPlayerNode->player = newPlayer;
    newPlayerNode->next = NULL;
    newPlayerNode->prev = NULL;
    return newPlayerNode;
}

void addPlayer(PlayerNode **headPlayer){
	Player newPlayer = createPlayer();
    PlayerNode *newPlayerNode = createPlayerNode(
									newPlayer);
    if(*headPlayer==NULL){
        *headPlayer = newPlayerNode;
        return;
    }
    PlayerNode *lastNode = *headPlayer;
    while(lastNode->next != NULL){
        lastNode = lastNode->next;
    }
    lastNode->next = newPlayerNode;
    newPlayerNode->prev = lastNode;
}

/*
Clean buffer
*/
void cleanBuffer(){
	int c;
	while ((c = getchar()) != '\n' 
							&& c != EOF); 
}

/*
Save the players to a file with | as the separator.
*/
void savePlayersToFile(PlayerNode *headPlayer) {
    FILE* file = fopen("players.txt", "w");
    if (file == NULL) {
        perror("Error en archivo\n");
        return;
    }

    PlayerNode* current = headPlayer;
    while (current != NULL) {
        fprintf(file, "%d|%s|%s|%d|%d\n", 
			current->player.id,
			current->player.name,
			current->player.nickname,
			current->player.age,
			current->player.points);
        current = current->next;
    }

    fclose(file);
    printf("Jugadores guardados en 'players.txt'\n");
}

void printAllPlayers(PlayerNode *headPlayer){
	PlayerNode* current = headPlayer;
    while (current != NULL) {
    	printf("\n----------JUGADOR:---------\n");
        printf("Id: %d\n",current->player.id);
        printf("Nombre: %s\n",
						current->player.name);
		printf("Nickname: %s\n",
						current->player.nickname);
		printf("Edad: %d\n",
						current->player.age);
		printf("Puntos: %d\n",
						current->player.points);
        current = current->next;
    }
}
void loadFromFile(PlayerNode **headPlayer) {
    FILE* file = fopen("players.txt", "r");
    if (file == NULL) {
        perror("The file 'players.txt' does not exist. A new one will be created when players are saved.\n");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        PlayerNode* newPlayer = (PlayerNode*)malloc(sizeof(PlayerNode));
        if (newPlayer == NULL) {
            printf("Error: Could not allocate memory while loading players.\n");
            fclose(file);
            return;
        }

        // Parse the line from the file
        if (sscanf(line, "%d|%s|%s|%d|%d\n",
            &newPlayer->player.id,
			newPlayer->player.name,
			newPlayer->player.nickname,
			&newPlayer->player.age,
			&newPlayer->player.points) != 5) {
            printf("Warning: Invalid format in line: %s", line);
            free(newPlayer);
            continue;
        }

        // Add the question to the list
        newPlayer->next = *headPlayer;
        *headPlayer = newPlayer;
    }

    fclose(file);
	printf("Jugadores cargados correctamente.\n");
}
