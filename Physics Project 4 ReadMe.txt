INFO6022 - Physics 2
Project #4: Soft Body Simulation

Submission by Dylan Kirkby, Brian Cowan, Ivan Parkhomenko

Created in Visual Studio 2019 Community Edition
Solution runs on Platforms and configurations.

Simply open in visual studio and build, it should all be configured with the correct dependencies.

Relevant Controls:
Keyboard:
	Cursor left/right
			Change Selected Entity.

	Movements relative to camera facing.
	Movements only work when controlling a ball type object or free cam.

	W		Move Forward
	S		Move Backward
	Q		Move Left
	E		Move Right
	
	P		Freecam mode
	Q		Move Down (freecam only)
	E		Move Up (freecam only)

	1		View soft bodies as ball nodes
	2		View soft bodies as a mesh
	3		Toggle Wind

Mouse:
	Move		Rotate Camera
	Wheel Up	Zoom Out (Camera starts at max distance)
	Wheel Down	Zoom In

Other controls may have undefined behaviour.

Main External configuration file can be found in:
	Entities	$(SolutionDir)AmethystEngine\data\config\complex\Entities.json
	Soft body used starts at line 88, or ctrl+f "Cloth Prime"


Cloth is configurable in the "Physics" section of the above json entity. It can be quite temperamental
as to what settings work with it. As is the cloth spawns in above the plan, and falls so that the bottom
row rest/collides constantly if wind is turned off.

Implementation is very close to what was went over in class, sharing the same structure overall and much
of the same logic. I opted to add in diagonal springs between each node and make the entire top row static
to increase the stability of the soft body.

It can all be found in Amethyst_Physics_Library and Amethyst_Physics_Wrapper following fairly closely to you
conventions.

Broad Phase:
For a broad phase regarding soft bodies, I check if the bounding boxes of the sphere and soft body intersect at
Amethyst_Physics_Library cWorld.cpp line 302 before proceeding to check the entire soft body if applicable.
I do similarly regarding a plane, using the textbook's check for a box collidin with a plane (lines 358 - 379)
before running a collision check on each node if applicable

Wind:
I instantiate a wind vector in my physics world and have it mutate each frame at the start of the integrate step
with a Gaussian rand, so it doesn't augment too much each step, while capping it to reasonable levels.
I had initially used deltatime as a part of the random function but that led to very slow and uninteresting changes
in the wind vector. Acceleration being cleared each frame makes stronger values seem to work better.
You can turn wind off by tapping 3, which sets the vector to all 0's, and disables the integration step mutator.

Rendering Options:
in Amethyst_Physics_Wrapper cClothComponent.cpp constructor, where I set the springs I also populate a vector of triangles
that hold the indeces of connecting points. These are set to any node that are to the top and right of another node.
See lines 46 - 81 for the spring and triangle indices set up. These are retrieved by the game code via the interface
GetTriangles() function to to be accessed in game code, then the triangle positions made using the GetNodePosition(index, glm::vec3&)
function.
For rendering see AmethystEngine main.cpp, Line 663 - 774!
This will hopefully be broken out into a rendering class sooner rather than later, once we've consolidated more work into
the same code base.
There should be enough commentary there to explain whats going on.
