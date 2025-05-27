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
	char character[MAX_LEN_STR];
}Player;

typedef struct PlayerNode{
	Player player;
	PlayerNode *next;
	PlayerNode *prev;
}PlayerNode;

//Lista de personajes
typedef struct{
    char name[MAX_LEN_STR];
    int victories;
}Characters;

typedef struct CharacterNode{
    Characters character;
    struct CharacterNode *next;
    struct CharacterNode *prev;
}CharacterNode;

//----Funciones-----//
//General
void cleanBuffer();
//Player
Player createPlayer();
PlayerNode* createPlayerNode(Player);
void addPlayer(PlayerNode **);
void savePlayersToFile(PlayerNode *);
void printAllPlayers(PlayerNode *);
void loadFromFile(PlayerNode **);
int countPlayers(PlayerNode *);
void displayTopNPlayers(PlayerNode *, int N);
void showStatistics(PlayerNode *);
/* -------------------------------------------------------------------------- */
/*                                   MAIN                                     */
/* -------------------------------------------------------------------------- */
int main(){
	PlayerNode *headPlayer = NULL;
	CharacterNode *headCharacters = NULL;

    int opcion;
    do {
        printf("\n==== MEN%c TORNEO SUPER SMASH ====\n", 233);
        printf("1. Agregar jugador\n");
        printf("2. Agregar personaje\n");
        printf("3. Asignar personajes aleatoriamente\n");
        printf("4. Registrar partida (sumar victoria)\n");
        printf("5. Mostrar todos los jugadores\n");
        printf("6. Guardar jugadores en archivo\n");
        printf("7. Cargar jugadores desde archivo\n");
        printf("8. Consultar estad%csticas\n", 161);
        printf("0. Salir\n");
        printf("Opci%cn: ", 162);
        if (scanf("%d", &opcion) != 1) opcion = -1;
        cleanBuffer();

        switch (opcion) {
        case 1:
            addPlayer(&headPlayers);
            savePlayersToFile(headPlayers);
            break;
        case 2:
            addCharacter(&headCharacters);
            break;
        case 3:
            randomCharacter(headPlayers, headCharacters);
            break;
        case 4:
            registrarPartida(headPlayers, headCharacters);
            savePlayersToFile(headPlayers);
            break;
        case 5:
            printAllPlayers(headPlayers);
            break;
        case 6:
            savePlayersToFile(headPlayers);
            break;
        case 7:
            loadFromFile(&headPlayers);
            break;
        case 8:
            showStatistics(headPlayers);
            break;
        case 0:
        	savePlayersToFile(headPlayers);
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opci%cn no valida. Intenta de nuevo.\n", 162);
        }
    } while (opcion != 0);
	return 0;
}

/*
......... PLAYER FUNCTIONS ........
*/
Player createPlayer(){
	Player newPlayer;
	printf("\nNombre del jugador: ");
    fgets(newPlayer.name, MAX_LEN_STR, stdin);
    newPlayer.name[strcspn(newPlayer.name, "\n")] = 0;

    printf("Nickname sin espacios: ");
    scanf("%s", newPlayer.nickname);

    printf("Edad: ");
    scanf("%d", &newPlayer.age);

    newPlayer.points = 0;
    newPlayer.id = currentID++;
    newPlayer.character[0] = '\0';
	
	cleanBuffer();
	return newPlayer;
}

PlayerNode* createPlayerNode(Player newPlayer){
    PlayerNode *newPlayerNode;
	newPlayerNode = (PlayerNode*)malloc(
							sizeof(PlayerNode));
    if (!newPlayerNode) {
        perror("Error al reservar memoria para jugador\n");
        exit(EXIT_FAILURE);
    }
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

//-------------------------FUNCIONES SOBRE LOS PERSONAJES-----------------------------------
Characters createCharacter(){
    Characters newChar;
    printf("\nNombre del personaje: ");
    fgets(newChar.name, MAX_LEN_STR, stdin);
    newChar.name[strcspn(newChar.name, "\n")] = 0;
    newChar.victories = 0;
    return newChar;
}

CharacterNode *createCharacterNode(Characters newChar){
    CharacterNode *newNode = (CharacterNode *)malloc(sizeof(CharacterNode));
    if(!newNode){
        perror("Error al reservar memoria para personaje\n");
        exit(EXIT_FAILURE);
    }
    newNode->character = newChar;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void addCharacter(CharacterNode **headChar) {
    Characters newChar = createCharacter();
    CharacterNode *newNode = createCharacterNode(newChar);

    if (*headChar == NULL) {
        *headChar = newNode;
        return;
    }

    CharacterNode *last = *headChar;
    while (last->next != NULL) last = last->next;
    last->next = newNode;
    newNode->prev = last;
}

int countCharacters(CharacterNode *head) {
    int count = 0;
    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}
CharacterNode *getCharacterByIndex(CharacterNode *head, int index) {
    int i = 0;
    while (head != NULL && i < index) {
        head = head->next;
        i++;
    }
    return head;
}
void randomCharacter(PlayerNode *headPlayers, CharacterNode *headCharacters) {
    int nCharacters = countCharacters(headCharacters);
    if (nCharacters == 0) {
        printf("No hay personajes disponibles.\n");
        return;
    }

    /* Contar jugadores */
    int nPlayers = countPlayers(headPlayers);
    if (nPlayers == 0) {
        printf("No hay jugadores registrados.\n");
        return;
    }

    if (nPlayers > nCharacters) {
        printf("Hay mas jugadores (%d) que personajes (%d), no se puede asignar sin repetir.\n", nPlayers, nCharacters);
        return;
    }

    /* Crear arreglo de  ndices y mezclar */
    int *indices = (int *)malloc(nCharacters * sizeof(int));
    if (!indices) {
        perror("Error al reservar memoria para aleatorizacion\n");
        return;
    }
    for (int i = 0; i < nCharacters; i++) indices[i] = i;

    srand((unsigned)time(NULL));
    for (int i = nCharacters - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }

    /* Asignar */
    PlayerNode *player = headPlayers;
    for (int i = 0; i < nPlayers; i++) {
        CharacterNode *charNode = getCharacterByIndex(headCharacters, indices[i]);
        if (charNode) strncpy(player->player.character, charNode->character.name, MAX_LEN_STR);
        player = player->next;
    }
    free(indices);
    printf("Personajes asignados aleatoriamente a los jugadores.\n");
}
