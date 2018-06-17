# Axios framework
The Axios framework makes use of [SFML](https://www.sfml-dev.org/). The Axios framework is used for 2D game development. It was actually meant to be a game engine, but I have no experience making a GUI yet. I don't want to focus on to many things at the same time because I still have a lot to learn, so a framework it is. 

## What does it do
The framework serves a role in the following subjects:
> If you want to read more about these subjects or follow their progression, go to the [projects page](https://antjowie.github.io/projects/axios-framework/)
- **Game instance**
    - Manage data paths
    - Manage user settings
- **Game loop**
    - Update Input handler
    - Render
- **Scene manager**  
    _Scenes are containers that contain objects and sometimes static level data. A section can be the main menu, level, options menu, loading screen etc_  
    - One frame stack
    - Dynamic object management (add and remove objects during runtime)
    - Object updating
    - Object references 
    - Event handling
- **Objects**  
    _Objects are the building blocks of the game. Objects are build of components_
    - Object factory
    - Animation handling
    - Collision and Physics
    - Object serialization
- **Assets management**  
    _Basic data containers for game assets_
    - Sound
    - Music
    - Images
    - Fonts
    - Asynchronous
- **String hashing (will probably be done by another library)**
- **Tiled support**  
    _[Tiled](https://www.mapeditor.org/) will be used as the main editor for scenes. With Tiled, all scenes initial state will be made._
- **Input handler**
    - Keyboard and mouse
    - Controller