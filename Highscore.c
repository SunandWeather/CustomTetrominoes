//
//  highscore.c
//  CustomTetrominoes
//

#include "highscore.h"
#include <SDL2_net/SDL_net.h>
#include <string.h>
#include <stdio.h>

void enter_highscore(int score, char name[], int game_mode)
{
	char mode[24];
	sprintf(mode, "mode=%08d", game_mode);
    char tempMode[24];
    char mode_s[24];
    
    for (int i = 0; i < 9; i++) {
        if (i < 4) {
            tempMode[i] = mode[i + 5];
        } else if (i == 4) {
            tempMode[i] = 'x';
        } else {
            tempMode[i] = mode[i + 4];
        }
    }
    
    sprintf(mode_s, "mode=Tet_%s", tempMode);
    
	char score_s[256];
	sprintf(score_s, "score=%06d", score);
	char name_s[12];
	sprintf(name_s, "name=%s", name);
    
	char buffer[1024];
	memset(buffer, '\0', 1024);
	strcpy(buffer, "POST /game/tetrominoes/highscore.php HTTP/1.1\r\n");
	strcat(buffer, "User-Agent: Tetrominoes-highscore\r\n");
	strcat(buffer, "Host: sunandweather.com\r\n");
	strcat(buffer, "Connection: close\r\n");
	strcat(buffer, "Accept: */*\r\n");
	strcat(buffer, "Content-Type: application/x-www-form-urlencoded\r\n");
	strcat(buffer, "Content-Length: 40\r\n\r\n");
	strcat(buffer, mode_s);
	strcat(buffer, "&");
	strcat(buffer, score_s);
	strcat(buffer, "&");
	strcat(buffer, name_s);
	strcat(buffer, "\r\n\r\n");
    
	IPaddress ip;
	TCPsocket tcp;
	if (SDLNet_ResolveHost(&ip, "www.sunandweather.com", 80) == -1) {
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		return;
	}
    
	tcp = SDLNet_TCP_Open(&ip);
    
	if (!tcp) {
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		return;
	}
    
	int d = SDLNet_TCP_Send(tcp, (void*)buffer, strlen(buffer));
	if (d != strlen(buffer)) {
		fprintf(stderr, "Error while sending highscore via TCP: sent %d chars.\n", d);
	}
    
	/* Wait for the reply */
	while (!SDLNet_TCP_Recv(tcp, buffer, 1024)) {
    }
}

void get_highscore(char* list, int game_mode)
{
//    char mode[24];
//	sprintf(mode, "mode=0");
	char get[24];
	sprintf(get, "get=1");
    
    char mode[24];
	sprintf(mode, "mode=%08d", game_mode);
    char tempMode[24];
    char mode_s[24];
    
    for (int i = 0; i < 9; i++) {
        if (i < 4) {
            tempMode[i] = mode[i + 5];
        } else if (i == 4) {
            tempMode[i] = 'x';
        } else {
            tempMode[i] = mode[i + 4];
        }
    }
    //    sprintf(mode_s, "mode=Tet %s", tempMode);
    sprintf(mode_s, "mode=Tet_%s", tempMode);
    mode_s[18] = '\0';
    
//    printf("%s", mode_s);
    
	char buffer[1024];
	memset(buffer, '\0', 1024);
	strcpy(buffer, "POST /game/tetrominoes/highscore.php HTTP/1.1\r\n");
	strcat(buffer, "User-Agent: Tetrominoes-highscore\r\n");
	strcat(buffer, "Host: sunandweather.com\r\n");
	strcat(buffer, "Connection: close\r\n");
	strcat(buffer, "Accept: */*\r\n");
	strcat(buffer, "Content-Type: application/x-www-form-urlencoded\r\n");
	strcat(buffer, "Content-Length: 24\r\n\r\n");
	strcat(buffer, mode_s);
	strcat(buffer, "&");
	strcat(buffer, get);
	strcat(buffer, "\r\n\r\n");
    
	IPaddress ip;
	TCPsocket tcp;
	if (SDLNet_ResolveHost(&ip, "www.sunandweather.com", 80) == -1) {
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		return;
	}
    
	tcp = SDLNet_TCP_Open(&ip);
    
	if (!tcp) {
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		return;
	}
    
	int d = SDLNet_TCP_Send(tcp, (void*)buffer, strlen(buffer));
	if (d != strlen(buffer)) {
		fprintf(stderr, "Error while sending highscore via TCP: sent %d of %lu chars.\n", d, strlen(buffer));
	}
    
    
    
	/* Wait for the reply */
	while (!SDLNet_TCP_Recv(tcp, buffer, 1024));
    
	/* Trim the buffer to only contain the highscores */
	char* temp;
	temp = strrchr(buffer, ':');
	strcpy(list, &temp[1]);
    
}
