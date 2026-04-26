# Game.h
Class responsible for all of the game logic, creation/destruction of assets and creation/destruction of graphics.

## Member Types
None.

## Member Variables
### Instance Variables
| Members | Type | Access |Description | Default Value |
|--------|------|------|-------------|-------------|
| `input ` | `Input *` | Private | Pointer to [Input](../Engine/input.md) class. | `None` |
|`window` | `Window *` | Private | Pointer to [Window](../Engine/window.md.md) class. | `None` |
|`gfx` | `Graphics` | Private |Instance of [Graphics](../Engine/Graphics.md)s class. | `None` |
|`player` | `Player *` | Private |Instance of [Player](../Game/Player.md)s structure. | `None` |

### Static Variables
None.

## Member functions
### Instance Functions

#### Function: `explicit Game(Input *input_class, Window *window_class) noexcept` 
- **Access:** `Public` 
- **Parameters:**  
    - `Input *input_class` - Pointer to the input class.
    - `Window *window_class` - Pointer to the window class.
- **Return:**  `None`   
- **Description**: Initializes Game class instance and retrieves the window and input class pointers.

#### Function: `~Game()` 
- **Access:** `Public` 
- **Parameters:**  `None`  
- **Return:**  `None`   
- **Description**: Destroys the Game class instance.

#### Function: `Game(const Game &other) = delete` 
- **Access:** `Public` 
- **Parameters:**  `const Game &other` - Other game class object.  
- **Return:**  `None`   
- **Description**: Disables copy constructor.

#### Function: `Game &operator=(const Game &other) = delete` 
- **Access:** `Public` 
- **Parameters:**  `const Game &other` - Other game class object.  
- **Return:**  `None`   
- **Description**: Disables copy assignment.

#### Function: `Game(Game &&other) = delete` 
- **Access:** `Public` 
- **Parameters:**  `Game &&other` - Other game class object.   
- **Return:**  `None`   
- **Description**: Disables move constructor.

#### Function: `Game &operator=(Game &&other) = delete` 
- **Access:** `Public` 
- **Parameters:**  `Game &&other` - Other game class object.  
- **Return:**  `None`   
- **Description**: Disables move assignment.

#### Function: `bool initialize_game(void)` 
- **Access:** `Public` 
- **Parameters:**  `void`  
- **Return:**  `bool`   
- **Description**: Initializes the game as well as the graphics and returns true if it succeeds. Returns false otherwise.

#### Function: `bool should_render_game(void)` 
- **Access:** `Public` 
- **Parameters:**  `void`  
- **Return:**  `bool`   
- **Description**: Gets a vector of all of the messages related to the Game. If any of the messages are of the type [Keyboard_Escape](../Engine/input.md#typedef-enum-events-event) it will return false and stops rendering the game. Returns true and continues rendering the game otherwise.

#### Function: `void update_game(double frame_time)` 
- **Access:** `Public` 
- **Parameters:**  `double frame_time` - The time of the frame in seconds.  
- **Return:**  `void`   
- **Description**: Updates the game.

#### Function: `void render_game(int fps)` 
- **Access:** `Public` 
- **Parameters:**  `int fps` - Number of frames per second.  
- **Return:**  `void`   
- **Description**: Renders the game along with all of it's assets.

#### Function: `void close_game(void)` 
- **Access:** `Public` 
- **Parameters:**  `void`  
- **Return:**  `void`   
- **Description**: Responsible for closing the game as well as closing the graphics.

### Static Functions
None.

## Dependencies
- [Graphics](../Engine/Graphics.md)
- [input](../Engine/input.md.md)
- [window](../Engine/window.md.md)
- [Player](../Game/Player.md)