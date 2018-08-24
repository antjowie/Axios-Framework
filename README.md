# The Axios framework
[The Axios framework](https://antjowie.github.io/projects/axios-framework/) (sends you to the projects main page) is a 2D data-driven game framework. It is made with extensive use of [SFML](https://www.sfml-dev.org/).

> I write about the development of this project. [Click here](https://antjowie.github.io/projects/axios-framework/) if you are interested.

It loads files from [Tiled](https://www.mapeditor.org/), A 2D map editor. The interaction between objects' are programmed in C++. All the objects are placed with 
This framework will serve as my foundation for my upcoming games.

## Why did it started this?
I started this project because I wanted to create a foundation to make my games. I'm aiming to make my own game engine, so by doing this, I should get a better understanding of it. I am working on this project while reading the books [Game Engine Architecture](http://www.gameenginebook.com/) and [C++ Primer](https://www.amazon.com/Primer-5th-Stanley-B-Lippman/dp/0321714113). You could see this project as my project for these books.

## What must it do?
The most important thing about this game engine is that I want to make sure that it is data-driven. An object should be easily manipulatable via text files or tools and do not need work inside of the source code. The reason why I'm pushing this so much is that I want to work on my skills to support external tools to make development easier.  
It also makes it easier to develop games with my friends. So the most important things about this project are:

* You must be able to make levels using external tools
* You must be able to modify object during runtime
* The only need for programming is to define object behavior and level loading

The last one is because I'm not sure how people will order their levels. For example, if I were to make a dungeon crawler with semi-random rooms. One file wouldn't suffice. It would be easier to separate a room layout in its own file and pick some files randomly. 

I'm also looking into supporting multiplayer and joysticks.

## Dependencies
This project is basically a higher level library around SFML, but it eases game development. There are a lot of practices that I'm putting into this engine. I'll have a list of resources that I'm using at this time. It may change later on during the development of this project. Here are the resources and dependencies that have helped me very much during development.

#### Resources
* [Game Engine Architecture](http://www.gameenginebook.com/)
* [C++ Primer](https://www.amazon.com/Primer-5th-Stanley-B-Lippman/dp/0321714113)
* [Game Programming Patterns](http://gameprogrammingpatterns.com/contents.html)
* [SFML Documentation 2.5.0](https://www.sfml-dev.org/documentation/2.5.0/)
* [Gaffer On Gamers](https://gafferongames.com/)
* [Metanet Software Collision Response](http://www.metanetsoftware.com/technique/tutorialA.html)

#### Libraries / dependencies
* [SFML](https://www.sfml-dev.org)
* [Json for Modern C++](https://github.com/nlohmann/json)

If you are interested in the development of this project, be sure to go to the [project's page](https://antjowie.github.io/projects/axios-framework/).