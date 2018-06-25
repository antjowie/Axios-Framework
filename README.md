# Axios framework
The Axios framework makes use of SFML. The Axios framework is used for 2D game development. It was actually meant to be a game engine, but I have no experience making a GUI yet. I believe that you should not do two major new subjects at the same time, so a framework it is.

## What does it do
The framework serves a role in the following subjects:
> Progress of these subject are being followed at the [project's page](https://antjowie.github.io/projects/axios-framework/). This list could be outdated. For the latest changes, check the [project's page](https://antjowie.github.io/projects/axios-framework/).
-  **Instance**
    - [X] Manage (de)initialization of modules
    - [X] Game loop
-  **Scene manager**  
    _Scenes are containers that manage objects and allow object to interact with eachother. It also allows object to easilly make new data during runtime. A scene can be a menu or level, basically everything that the user makes in Tiled._  
    - [ ] One/two frame stack
    - [ ] Dynamic runtime stack
    - [ ] Object updating
    - [ ] Object reference handling
    - [ ] Event handling
-  **Objects**  
    _Objects are the building blocks of the game. Objects are build of components_
    - [ ] Object factory
    - [ ] Animation handling
    - [ ] Collision and Physics
    - [ ] Object serialization
-  **Subsystems**  
    _Subsystems are systems that classes can make hooks to. They are updated in every scene. The reason why subsystems are not in the objects is because this way couping between subsystems and objects is not present_
    - [ ] Animation
    - [ ] Rigid body (collision response/physics)
- **Data manager**  
    _Basic data containers for game data that has to be saved_
    - [X] Game keybinding items
    - [X] User config
- **Assets manager**  
    _Game textures, sounds, files that are static and are different depending on the scene._
    - [ ] Sound
    - [ ] Music
    - [ ] Images
    - [ ] Fonts
    - [ ] Asynchronous scene transition
- **Tiled support**  
    _[Tiled](https://www.mapeditor.org/) will be used as the main editor for scenes. With Tiled, all scenes initial state will be made._
    - [ ] Load external map
    - [ ] Export Object types
- **Input handler**
    - [X] Keyboard and mouse
    - [ ] Controller
- **Utilities**
    - [X] JSON parser
    - [ ] SFML key code converter
    - [X] Logger
