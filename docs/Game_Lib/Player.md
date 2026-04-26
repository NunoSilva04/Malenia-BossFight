# Player.h
Responsible for the creation, destruction and all logic associated with the player.

## Member Types
### `typedef struct Player_t Player`
Opaque type of the structure `Player_t`. 

## Member Variables
None.

## Member functions
### Instance Functions
#### Function: `Player *create_player(void)` 
- **Access:** `Public` 
- **Parameters:** `void` 
- **Return:**  `Player *`   
- **Description:** Initializes the player as well as loading all of it's assets to memory. Returns a valid `Player` pointer in the case of success and `NULL` if it fails.

#### Function: `void destroy_player(Player *player)` 
- **Access:** `Public` 
- **Parameters:** `[in]Player *player` - Valid pointer to a `Player` structure.
- **Return:**  `void`   
- **Description:** Destroys the player. Must be a valid pointer.

## Dependencies
None.