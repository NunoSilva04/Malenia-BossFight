/**********************************************************************************
*   File: Player.h
* 
*   Description: 
*       Responsible for all player logic.
* 
*   See: 
*       docs/Game/Player.md 
*       for full documentation of the header
* 
*************************************************************************************/

#ifndef __PLAYER_H__
#define __PLAYER_H__

typedef struct Player_t Player;

Player *create_player(void);
void destroy_player(Player *player);

#endif // __PLAYER_H__