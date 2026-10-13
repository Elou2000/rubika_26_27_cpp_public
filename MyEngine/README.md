# MyEngine

The purpose of this project is to implement a tiny 2D engine using SFML to practice C++, to think about engine architecture, how to solve issues... from scratch. 

[Get back to main README](../README.md)

### Summary

1. [Threshold 0](#threshold-0)
2. [Threshold 1](#threshold-1)
2. [Threshold 2](#threshold-2)
2. [Threshold 3](#threshold-3)

### Resources

Here are some useful links:
- [SFML doc](https://www.sfml-dev.org/documentation/3.0.2/)
- [ImGui](https://pthom.github.io/imgui_manual_online/)

## General notes

The implementation of this project is totally free. You can add any method/member/... you want/need to solve issues. However, if you want to change the way the system works, please ask me first so that the entire class has the same architecture.

A `Logger.h` file has been added. It provides a (small) working log system. Logs might be lifesaving and absolutely useless at the same time. They provide users/developers with information about the various systems. But to be useful, logs must be added during the development and provide useful/self-explanatory information.

As you can see in `Profiler.h` (and used in `Main.cpp`), you can use macro to create markers that can be displayed in [PIX](https://devblogs.microsoft.com/pix/download/). This is a useful tool that can help you track costly processes... But such as Logs, it is simpler to add markers during the development (otherwise, when you really need it, it will be empty). 

Try thinking about edge cases during all your development. By doing that, you will create a robust system and limit the number of bugs you will create.

## Threshold 0

The purpose of this threshold is to become familiar with the given code of the project. In its base version, the project compiles and works (even if it draws nothing). Three configurations are provided :
- Debug : `_USE_IMGUI` and `_USE_PROFILER` are defined. No optimization. This is the configuration you will work on 
- RelWithDebugInfo : `_USE_PROFILER` is defined.
- Release : `_USE_IMGUI` and `_USE_PROFILER` are not defined. Everything is optimized.

Also take a look at he provided classes `DebugMgr` (which enables you to register a custom debug) and `Logger` (which enables you to log messages). Those two classes will be used throughout the project.

## Threshold 1

As mentionned earlier, the purpose of this project is to create a very simple 2D engine that could run a simple game (Vampire Survivor, Binding of Isaac...) from a very naive perspective to a more complex one. To do so, various systems will be developed alongside this lesson : Entity Component System, texture manager, animation system, multi-threading, editors... depending on where we want to go.

Let's start very simple to familiarize yourself with the environment.

### Step 0

Our engines will be based on entities. Everything will derevied from the `Entity` class. It will later be composed of different components that will provide it with new behaviors but we will deal with that later. The header to implement the `Entity` class is provided. Add everything necessary to be able to instantiate it. A `TransformComponent` member is part of the `Entity` class. This class is provided to avoid matrix issues during the development. For now, it is a standalone class but this will change with the development of the ECS.

At the end of this step, you should be able to create a `Entity` variable in your main function.

### Step 1

At this stage, multiple entities can be instantiated but they are not linked to anything. We need to group them into a `Scene`, so they get meaning. The header is provided to give you the different signatures of the method you must implement. However, you may need to modify it to match the following requirments:
- Store different entities
- Methods to add/remove entities to/from the pool
- Entities must follow the life cycle of the `Scene`

At this end of this step, you shoud be able to create a `Scene` variable in your main function and use methods you have created to populate it with entities.

### Step 2

Create now a new manager `SceneMgr` in the same way as I did for the `DebugMgr` for instance. Here is the list of what you must implement:
- Store multiple `Scene`.
- Store the information about which `Scene` is active (the active scene is the one currently played).
- A method to add a new scene. Two scenes cannot have the same name so you will need to handle that case for instance.
- A method to select the active scene by name. Be careful of the life cycle of the previous active scene if you change it.
- Life cycle methods

At this end of this step, you should be able to use your `SceneMgr` variable inside `gData` to manipulate multiples scenes.

### Step 3

At this stage, everything should be in place... But you have no way to test it and no way to "see" if it is working. That is why we have a `DebugMgr`. Change your `SceneMgr` so it can draw its own debug window (**hint**: You might take a look at the `Logger` class). This debug window must allow you to :
- See the various scene added to the manager
- Know which scene is currently active
- Inspect each scene to see its content (name, entity...)
- Buttons to switch scene, add entity...

At this end of this step, you should be able to manipulate your `SceneMgr`, `Scene` and `Entity` directly from the debug window.

**hint** : We are using ImGui to draw those debug.

## Threshold 2

At this stage, we have invisible entites inside a scene. The next step is logically to draw a texture at the right location. But to do so, we have to load the texture (and its metadata) first. That is the purpose of this threshold.

### Step 0

A `TextureMgr` file has been added to the solution and contains the signature of its core features. First, add everything needed to be able to instantiate this manager and add an instance in `Globals.h` as you did for the others managers.

### Step 1

Our engine will only load textures with metadata (same path and same filename but using a `xml` extension). Those metadata contain every information about the content of the texture. For instance, a texture file could contain lots of different textures in the same file (which is often the case in 2D engine) or the different sprites of an animation... Parsing the `xml` file will allow us to know those information. This also allows us to be data driven. Indeed, artists, designers... can change the texture, the animations... without having to recompile our engine (which is a good habit). Our metadata file will look like 
```xml
<Animations>
    <Animation Name="Head_Down">
        <X>0</X>
        <Y>0</Y>
        <SizeX>28</SizeX>
        <SizeY>25</SizeY>
        <OffsetX>12</OffsetX>
        <OffsetY>0</OffsetY>
        <SpriteNum>1</SpriteNum>
        <SpritesOnLine>2</SpritesOnLine>
        <Reverted>0</Reverted>
    </Animation>
    <Animation Name="Head_Up">
        <!-- ... -->
    </Animation>
    <!-- ... -->
</Animations>
```
or 
```xml
<Backgrounds>
    <Tile Name="Corner_Top_Left">
        <X>0</X>
        <Y>0</Y>
        <SizeX>52</SizeX>
        <SizeY>52</SizeY>
        <RevertedX>0</RevertedX>
        <RevertedY>0</RevertedY>
    </Tile>
    <Tile Name="Corner_Top_Right">
        <!-- ... -->
    </Tile>
    <!-- ... -->
</Backgrounds>
```
To parse the `xml` file, `rapidxml`  is part of the project and can be used by including 
```cpp
// The main include
#include <rapidxml/rapidxml.hpp>

// Might be helpful
#include <rapidxml/rapidxml_utils.hpp>
```

All those information will be stored in a `TextureData` structure.

There are two methods that you must implement. Keep in mind that it is not because there is only two methods that you cannot create others.
```cpp
	/*
	 * Loads the texture using the given path, including its metadata.
	 * Obviously, this function will also store the loaded data
	 * If errors occur during the loading process, this function must return false.
	 * Return true otherwise.
	*/
	bool LoadTexture(const std::filesystem::path& path);

    /*
    * Returns the corresponding TextureData using the name of the texture. Returns the default TextureData in case of an unknown name
    */
    const sTextureData& GetTextureData(const std::string& name) const;
```

Here are some hints:
- Don't forget to log every error, warning or information. It will help you later
- Split your `LoadTexture` code into several small functions (a function, a task). It will help you to read/understand your code.
- Start by checking if you can load the texture. Does the file exist? Does the metadata file exist?...
- Loading a file into a SFML texture is easy and documented. Check SFML documentation
- Here is the [doc of rapidxml](https://rapidxml.sourceforge.net/manual.html).
- Use the content of `rapidxml/rapidxml_utils.hpp` to open the file (much simpler that using fopen)
- The lists of types that you should use:
  * rapidxml::file<> 
  * rapidxml::xml_document 
  * rapidxml::xml_node<> 
  * rapidxml::xml_attribute<>
- The list of functions that you should use:
  * parse<0> 
  * first_node 
  * first_attribute 
  * value 
  * value_size
  * next_sibling

### Step 2

It may be a good idea to create a default texture that can be used if the required one does not exist. Create a dedicated `TextureData` to this default texture that we will be returned by `GetTextureData` in case of a wrong name.

### Step 3

As we did in the previous threshold, we will need a way to visualize what it has been loaded. To do so, let's create a debug window dedicated to the `TextureMgr`

Here are useful information that we want to visualize:
- The loaded texture (check for SFML ImGui)
- All information stored in `TextureData`

## Threshold 3

During Threshold 1, we have created entities with a transform. During Threshold 2, we have created a `TextureMgr` capable of loading textures and metadata. The logical next step would be to combine the two together to add textures to our entities like we did for the `TransformComponent` (hard coded). But this approach is not future-proof and may lead to nonsense. For instance, an entity might not need to draw something. So, we will need to add those components dynamically. That is what we are going to do in the following steps. As a first step, create a component capable of drawing.

### Step 0

The first thing to do is to create the base class of all our components. A `IComponent` has been added to the project which contains the minimal methods and members that we will need. Depending on the language an interface may or may not contain members. C++ allows it (because interface does not really exist in C++) so we are going to use this approach to avoid having to create an intermediate class. Let's keep things simple! That is why there is a `Entity& ParentEntity` inside `IComponent` (keep in mind that a reference must always be valid which is why it must be initialized inside the [constructor initializer list](https://en.cppreference.com/cpp/language/constructor)).

In this step, you have to change the `TransformComponent` class to make it inherit from `IComponent`. If you do it correctly, you should be able to instantiate an `Entity` as you did before.

### Step 1

Let's continue with another component, a `RenderComponent` which will draw shape(s) (`IDrawable`) in the scene. 

In this step, you have to create this component (even if it is an empty component for now) and add it (temporarily) to your `Entity` (in the same way we did for the`TransformComponent`). If you can instantiate your `Entity` with those changes, then you can move to the next step.

### Step 2

To draw things on the screen, SFML uses the base class [IDrawable](https://www.sfml-dev.org/documentation/3.1.0/classsf_1_1Drawable.html). We will create a wrapper around this interface and its children so we can abstract away our dependency on SFML (imagine we decide tomorrow to use OpenGL to render our scene, we don't want to modify our entire engine, just the behavior of our wrappers). 

To do so, a `IDrawable` class has been added to the project. You now have to create a class for two of the SFML class : `sf::Sprite` (which will be responsible for drawing animated texture) and `sf::RectangleShape` (which will be responsible for drawing static one). Each of your wrappers will contain the corresponding SFML member and will provide functions to interact with it (keep them empty for now, we will complete them later according to our needs).

There is no real way to test your implementation at this stage, so move on to the next one.

### Step 3

At this stage, we will (temporarily) hard code lots of things to be able to test our system:
- Already done during step 1, add a `RenderComponent` to your `Entity`
- Add a `Rectangle` drawable to your `RenderComponent`
- Select a `fillColor` and set the `WorldTransform` in the `Rectangle` init so it can be seen on screen
- Modify the needed life cycle functions of your engine so it takes into account this new component (Start, Update, Draw, Destroy)
- Finally in the main function before the main loop, create a `Scene` with an `Entity` and enable it using the `SceneMgr`

If the treshold 1 and threshold 3 have been done right, you should see the rectangle you have created on the scene.