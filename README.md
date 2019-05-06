# GraphicsExam

## Task 1:
- [x] Generate a terrain for the purpose of rendering. You are free to use any algorithm of choice
(e.g., Perlin Noise Generator or Fractional Brownian Motion).
- [ ] Think about what you can do to make the terrain look realistic by using shaping functions,
e.g., (1 - abs(1 - 2x)) to generate creases
- [x] The output should serve as input for the following task.
- [ ] Generate more complex terrain that contains creases and valleys

## Task 2:
- [x] Load the heightmap (either generated or one of the ones provided in the resources - see
below) into a scene and color elevation ranges to resemble a somewhat realistic
representation of altitudinal zones. You can assume that the lowest elevation level is water,
and the highest are mountain tops.

- [x] You will need to provide a free-moving camera, whose orientation is controlled by the
mouse and whose position should be controllable by GUI-based specification. Alternatively,
or in addition, consider to the integration of zooming functionality.
- [x] Illuminate the scene using the phong model. You should be able to position lights using the
GUI.
- [ ] Introduce rain and snowfall by introducing models of raindrops and/or snowflakes.
- [x] Allow dynamic movement of lights, either interactive or automatic (e.g., daytime
cycle)
- [ ] Manage intensity of rain/snow
- [ ] Add Shadows
- [ ] Add additional landscape features (e.g., trees)
- [ ] Render/animate clouds or water surface
- [ ] Use particle systems to represent rain/snow

## Task 3:
- [x] As the last step, add a model of a deer to the scene (model provided in resources - see
below).
- [ ] The model should be navigable along the rendered scene and leave a trail (i.e., by
discolouring the walked path)
- [ ] Allow switching between global camera, third-person and first-person view using the toggle
button ‘t’.
- [ ] The user should be able to control the model using the arrow or wasd keys.
- [ ] Animate the model
- [x] Add additional models
- [ ] Add multiple deer models and perhaps even coordinate behaviour
- [ ] Add realistic physics (e.g., gravity effects)


## Things "working"
* Loading heighmap (Have to ignore some errors to continue)
* Generating heightmap
* Freemoving camera
* Dynamic movment of light
* Dynamic changing light
* Deer is in the scene
* You can add more models via UI
* Height based texturing of terrain

## Things that went to shit
* I was not able to make a new renderer because of our framework
* I load in a plane obj instead of creating one beacuse of not being able to make a new renderer
* I displace terrain in vertex shader making my normals weird, and could not figure out a proper way to calculate them from heightmap
* Heapstack corruption when you load/generate terrain that you can just skip and it works fine (Not sure why it happens)