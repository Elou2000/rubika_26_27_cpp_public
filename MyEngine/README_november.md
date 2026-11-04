# MyEngine

The purpose of this project is to implement a tiny 2D engine using SFML to practice C++, to think about engine architecture, how to solve issues... from scratch. This is the second part of the project, based on the work we did previously that you can find [here](./README_october.md)

[Get back to main README](../README.md)

### Summary

1. [Threshold 0](#threshold-0)
2. [Threshold 1](#threshold-1)
3. [Threshold 2](#threshold-2)
4. [Threshold 3](#threshold-3)
5. [Threshold 4](#threshold-4)
6. [Threshold 5](#threshold-5)

## Threshold 0

During the first part of the project, we have created an ECS with an animation system (that can load, display and animate textures). We need a few more basic bricks to be able to create a simple game. Let's start with the most obvious one: Inputs.

### Step 0

Discussion in class to answer architecture questions, flow questions and feature interrogations such as:
- How should our `Entities` interact with the input system? (pull? callback?)
- Should our `Entities` interact with keys directly or should we add a layer of abstraction?
- Does our input system allow input rebinding?
- If the answer to the previous question is yes (and it is), what should we do to do it?
- When should our input system interfere in our frame?
- Should we support mouse? keyboard (AZERTY, QWERTY)? joystick?
- ...

If everything is clear for you at this stage, you can try to implement the input system on your own. Otherwise, you can follow the steps.

### Step 1

Let's start simple. Create a `InputMgr` as we used to do it and integrate it in the flow of our engine (init, destroy...). This manager is a bit particular because its update must be the first manager to be updated during the frame (because all others depend on it).

### Step 2

Let's continue by creating an abstraction `InputAction` that represents the action triggered by the input. For instance, we won't check if the space bar is pressed but check for the "jump" action. An `InputAction` is defined by its name, a value (between -1 and 1 in case of analog input) and a container of callbacks that must be called on any value update. This abstraction is provided in a header file. It is now up to you to add the following features:
- Create a function that will set the float value and call every callback if necessary.
- Create functions that will allow any system to register (and unregister) to an `InputAction`. A registered system will be called on value change through the callback it provided. To keep it simple, the unregister function will remove all callbacks from the same owner registered to an `InputAction`.

### Step 3

It is now time to link our `InputMgr` to our actions. Create a container that stores all of our `InputAction`. Think about the best container to use depending on our future needs. Furthermore, implement functions to create/remove an `InputAction`. There are two ways to identify an `InputAction`: by name or by hash. One is faster, more optimized but opaque. The other is slower but explicit and easy to use. Think well about how to use those two ways to have "only" qualities. Finally, create functions to register/unregister to a specific `InputAction` from the `InputMgr` (by calling the functions you have implemented in the previous step).

You can now create a few temporary actions in your main function (at the right time in the flow) and use your debugger to check that everything works well.

### Step 4

Let's build the connection between the input and the action. The brute-force solution would be to check each frame the entire keyboard inputs. It works well but almost all inputs would be useless, so let's focus on the ones that are actually used. To do so, create a function in the `InputMgr` to bind an `sf::Keyboard::Key` to an `InputAction` based on its name/hash (once again pick the right one). Keep in mind that a key can trigger multiple actions and the same action can be triggered by different keys. Furthermore, create a function to unbind an action from a key. Finally, in the update function of the `InputMgr`, check the status of each registered key and update the value of its linked `InputAction`.

You now have all the bricks to move an `Entity` using inputs (even if we are going to hard-code a big part):
- Create `InputActions` such as "Up", "Down", "Left", "Right".
- Bind those actions to some keys.
- Create functions that move the entity and register those functions.

### Step 5

Such as the other managers, we will create a debug window for the `InputMgr`. This debug should provide at least the following information and features:
- List of `InputAction` objects:
    - For each, display the current value and all the callbacks registered.
- List of bindings.
- Create new bindings, remove a binding.

### Step 6

Let's move this hard-coded part to an external file so it can be changed without the engine needing to be recompiled. You are free to choose the file format you want, the architecture you want and its location (even if I advise you to put it in the `resources` folder). This file must contain at least:
- The list of all actions used by the game.
- The default bindings used by the game:
    - Don't forget to add a way to identify the type of binding in case later we want to support joysticks, for instance.

## Threshold 1

Let's focus on the last missing part needed to create a very simple game: collision. In this threshold, we will create a collider component that can be used by our ECS.

### Step 0

Discussion in class to answer architecture questions, flow questions and feature-related questions.

If everything is clear for you at this stage, you can try to implement the input system on your own. Otherwise, you can follow the steps.

### Step 1

To keep things simple, we will assume that an `Entity` can only have a single collider (even if this assumption might evolve in the later steps).

So the first thing we have to create is a component called `DynamicColliderComponent` which inherits from `IComponent`. This component will work as a wrapper around the SFML class `sf::FloatRect`, which provides useful functions to determine whether two `sf::FloatRect` objects intersect or not.

Create the necessary functions to manipulate the rectangle, such as size and offset from the `TransformComponent` (this class doesn't support rotation, which is not optimal, but it will be sufficient in our case).

Finally, add this component to your `Entity` and do everything necessary so that this new component is visible and modifiable through the debug window (even if you cannot check the modifications on the screen yet).

### Step 2

At this stage, we have an invisible rectangle around our entity. However, it is hard to use because you have to dig into your debugger to determine whether everything works well.

To fix this issue, we will create a debug feature to display all hitboxes in a scene.

**Everything we are going to add must only be available in debug mode. That means that you have to implement these features inside MACRO blocks.**

Let's add a simple `sf::RectangleShape` to our component and a `Draw` function. Every modification made to the hitbox itself must be replicated on the `sf::RectangleShape`, so both remain consistent. During the collider's `Draw` function, simply draw the rectangle in the right place.

Then, update the global flow of your engine to draw the `Entity`'s collider at the right time in the flow.

Finally, create a debug variable accessible from anywhere which indicates whether the collider must be drawn or not. Register a debug action in the `DebugMgr` so we have a button to toggle it in our debug window. Now that we have an input system, we can also create an input action and a binding for this feature to make it easier to use.

### Step 3

Before implementing the behavior of our collider, we will need another `Entity` to collide with. We could continue creating custom `Entities` in our main function, but things will become messy very quickly. Let's organize that.

Create a `Player` class which inherits from `Entity` and move its setup into its constructor and its `Start` function. Replace its entire creation and setup in the main function with the creation of a `Player` instance.

Create an `Obstacle` class which also inherits from `Entity` and modify the appropriate functions to set up this class with a `TransformComponent`, a `RenderComponent` with a `RectangleDrawable` and finally a `DynamicColliderComponent`.

Finally, instantiate multiple `Obstacles` at different places in our scene so that our `Player` can interact with them.

### Step 4

Let's focus on the main behavior of our collider: detecting collisions. In this threshold, we will keep the implementation deliberately naive so we can optimize it in the next one.

Iterate over every `Entity`, get its collider if it has one and check whether the two hitboxes collide.

This part is pretty straightforward. The real difficulty comes with the following question: What should we do if two entities collide?

There is no universal answer to this question. You might want to move one to avoid an overlap. You might want to 'destroy' one or the other. You might want to trigger an event...

To solve this problem, we will give the responsibility to the `Entity` itself using a callback. This callback must determine what to do with the collision by returning an enum (which we will limit to two possibilities for now: block or do nothing).

Implement everything necessary to set that callback, then use it at the right time in your function and update the `TransformComponent` position according to its result.

Finally, implement this function on the `Player` side to always return `Block` (we will change that in the next threshold).

At this stage, you should be able to move your `Player` and collide perfectly with the `Obstacles`.

## Threshold 2

In this Threshold, we are going to optimize our collision system to make it much more efficient.

### Step 0

Let's start by visualizing how bad our algorithm is. To do so, surround the execution of the `Update` function of each component, especially those of your `Player`, with profiling macros that you can find in `Profiler.h` (as I did in the main function). Using those macros, you can visualize the timings in [PIX](https://devblogs.microsoft.com/pix/download/). The entire project is already configured to work with PIX, so you just need to download it.

Make several captures of your engine as you increase the number of `Obstacles` in your scene (10, 100, 1000, 1000000...).

**Hints**

Follow these guidelines to capture execution information through PIX:

1. Run your game engine.
2. In PIX, select the executable to attach to through the `Select Target Process` window and the `Attach` section.
3. Once you have found the correct executable, click on `Attach` (uncheck the GPU Capture button).
4. PIX is now attached to the executable. You can start a capture by clicking on the `Start Timing Capture` button.
5. After a few seconds, stop the capture by clicking on the same button.

Here is the online [documentation](https://devblogs.microsoft.com/pix/timing-captures-new/) about PIX if you want to know more.

The different events shown in the PIX capture are sent through `PROFILER_EVENT_BEGIN` and `PROFILER_EVENT_MACRO` (as well as their color and text). Feel free to add new events to your code so you can actually see them in your capture.

### Step 1

Discussion in class to answer architecture questions, flow questions, and feature-related questions.

If everything is clear to you at this stage, you can try to implement the input system on your own. Otherwise, you can follow the steps.

### Step 2

Let's create a static tag system to qualify our colliders/entities. To do so, let's create the following flags using defines (we could also use an enum or enum class to do the same thing).

```cpp
#define ENTITY_TAG_NONE			0
#define ENTITY_TAG_STATIC		1 << 0
#define ENTITY_TAG_DYNAMIC		1 << 1
#define ENTITY_TAG_NEUTRAL		1 << 2
#define ENTITY_TAG_ENEMY		1 << 3
#define ENTITY_TAG_PLAYER		1 << 4
```
Each flag **must** equal a different power of two; otherwise, you will get conflicts. Feel free to add new flags or modify these ones according to your needs.

Create a variable large enough to store every combination of flags in your `Entity`, as well as functions to add a new tag (or a combination of tags), remove tag(s), and finally, a `HasTag` function that returns true if the given tag (or combination of tags) is set (we could also add a function that checks if at least one flag of the combination is set). **Those functions must use bitwise operations**.

### Step 3

We can now filter the `Entities` our collider will interact with using those tags. Add a function `SetCollisionFilterMask` to the `DynamicColliderComponent` that will use that data to determine which `Entities` it can collide with. We will add two collision modes using those tags: either at least one of the given flags is required to collide, or the entire combination is required to collide. Add the appropriate setter to the `DynamicColliderComponent` to allow switching between the filter modes and choose one by default. Modify the behavior of the `DynamicColliderComponent` to reflect the two features we have just discussed.

Set the appropriate flags for the `Player` class and the `Obstacle` class.

### Step 4

Filtering dynamic `Entities` is (a bit) faster, but still too slow in our case, even if the `Player` is the only dynamic `Entity`. Indeed, we still look through the entire list of `Entities` to find the right ones, which can take a long time.

Let's add some caches to reduce that time. We are going to add two new containers to the `Scene` to keep track of which `Entities` have the static tag and which ones have the dynamic tag. **Those new containers do not contain the `Entities` themselves, but only a reference/pointer/handle to them. They are not responsible for their creation/destruction, even though they must remain consistent with the entities list**. Modify the `AddEntity` function in the `Scene` to update those new containers. Furthermore, modify the `AddTag`/`RemoveTag` functions in case those flags change.

We now have several lists that we can use to improve our collision system's speed.

### Step 5

We now have good performance when colliding with dynamic `Entities` (because we only have a few), but collisions with static `Entities` are still slow. Let's organize our cache.

Create a `GridEntityCache` class that will store `Entities` in a grid according to their position. Let's start by creating methods to configure the grid as needed (position range, cell size, etc.). Once those parameters are known, we can create the grid accordingly.

Create an `AddEntity` function that will add the `Entity` to the appropriate cells according to its position and the size of its collider. Check every corner and border of the collider to determine all the affected cells. Also create the `RemoveEntity` function.

Create a `GetEntities` function that takes a position as a parameter and returns all the entities inside the same cell as the given position (**do not return a container to avoid copying; instead, modify one provided as a parameter**). Create another `GetEntities` function that takes the collider rectangle instead of a position. This function must return the contents of all cells overlapped by the rectangle (check the borders and corners).

Replace the two containers created during the previous step with the `GridEntityCache` and use it in the collision detection to speed everything up. Add all entities to this container as well to gain some performance when there is no filter.

### Step 6 (Going further)

Depending on our constraints, we could use another organization system. Let's try another one.

Create a `QuadTreeEntityCache` that will store `Entities` in a quad tree according to their position. Let's start by creating methods to configure it as needed (position range, maximum tree depth, maximum number of elements per leaf).

Let's start simple by adding all `Entities` first and then building the tree. Create an `AddEntity` function that will check whether the tree has been created or not. If it has not, store the `Entity` in a temporary container. We will deal with the other case later.

Create the `BuildTree` function that will create the tree based on the previously configured parameters and the entities' positions:

- Split the scene into 4 rectangles using the middle point.
- Count the number of elements in each rectangle. If it is smaller than the maximum number of elements allowed per leaf, stop here. Otherwise, if the maximum tree depth has been reached, stop here. Otherwise, split the rectangle into 4 smaller rectangles and start again.

Create the `AddEntity`/`RemoveEntity` functions that should work with an existing tree.

Create a `GetEntities` function that takes a position as a parameter and returns all the entities inside the same node as the given position (**do not return a container to avoid copying; instead, modify one provided as a parameter**). Create another `GetEntities` function that takes the collider rectangle instead of a position. This function must return the contents of all nodes overlapped by the rectangle (check the borders and corners).

Add a switch (at compilation time or runtime) to choose which method we are going to use to organize our cache.

### Step 7 (Going further)

Should we add a debug visualization to display the grid/quad tree distribution?

Create a custom ImGui component to display it.