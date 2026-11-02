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
