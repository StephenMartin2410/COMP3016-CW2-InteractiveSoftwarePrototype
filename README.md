# Markdown Documentation
# Video Link - 
## How to use software
- Open the given CW2.exe file in the project folder.
  ![image](https://github.com/StephenMartin2410/COMP3016-CW2-InteractiveSoftwarePrototype/assets/91669127/1fba0572-599d-4348-85b9-c62ed261c0c1)

- You can use WASD to move around the plane.
- You can also use space to move the camera up.
- Pressing right control makes the camera move down.
- You can also use the mouse to move the camera view around.
## Project starting point
My starting point of this project was using the shader files and main.cpp file used in the lab 9 model loading task.
## Achievements
- Loads 3 models into the program that use obj and mtl files to map textures onto themselves.
  ![image](https://github.com/StephenMartin2410/COMP3016-CW2-InteractiveSoftwarePrototype/assets/91669127/a9faaa89-530d-42cc-a8da-4662c8565c5a)

- Allows the camera to move in every axis.
- Uses rand values to randomly generate the height of the building objects that are on the plane.
- Uses rand values to randomly generate the distance between certain axis of the buildings.
- Uses rand values to randomly generate the rotation of the buildings.
  ![image](https://github.com/StephenMartin2410/COMP3016-CW2-InteractiveSoftwarePrototype/assets/91669127/83dec73f-5654-44b0-99b6-fee8090912cf)

- Outputs a quad used to highlight the textured signature model.
- Outputs the signature.
  ![image](https://github.com/StephenMartin2410/COMP3016-CW2-InteractiveSoftwarePrototype/assets/91669127/64181d7f-03e0-444d-8a64-ed6445bbf2b4)

## Class usage
This project uses the model class to load the obj files and gather the textures to correctly render the objects into the view space. The model class loads the model using assimp, processes the mesh and textures of the objects and then allows the class to be used to draw the obj object.
## Program loop
This prototype uses a while loop to process the users input (which is used to move the camera or close the window), sets the colour of the display window (used to simulate the sky), sets the view of the application (the perspective the user sees), sets the scale of all of the model objects, sets the location of the models (translation), outputs a quad, and then displays the signature infront of it. 
## Inspiration
I got the inspiration for this prototype from talking about the randomly generated projects in one of the lectures. The idea was originally going to be using a noise map to randomly generate the height of buildings in a city but I retracted that idea to them the height generated using rand values within a range instead.
## Improvements that could be made
- The model loading for the buildings could use instancing instead of the current for loop inplementation, this would mean that instead of going through a loop to render all of the buildings they could all be rendered with one call. This would increase the performance of the call and also decrease the load on the cpu.
- Noise map random generation. I could use a noise map to randomly generate the height of buildings instead of random numbers, this could give me more control over the generation and be used as another feature of the prototype.
- The ground plane could be procedurally generated so that it stretches out instead of being one object, this would increase the size of the map and make the project look closer to a game than an output of models.
- The prototype could incorporate object/camera collision. I'm not entirely sure how to go about this but it would be interesting to implement in the future.
- Lighting. It would be cool to add a sun object into the environment that shines light onto all of the models.
- Shadows. Adding shadows onto all of the buildings with different heights so that there is a unique shadow map every render would be an interesting addition to the prototype.
