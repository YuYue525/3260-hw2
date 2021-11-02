CSCI3260 Assignment 2 KeyBoard / Mouse Event

Name: YU Yue
SID: 1155124490

Manipulation:

	Key "Esc": exit
	
	//Lighting control
	Key "W": increase the brightness of directional light
	Key "S": decrease the brightness of directional light
	Key "A": move the directional light to the left
	Key "D": move the directional light to the right
	Key "E": increase the brightness of point light
	Key "Q": decrease the brightness of point light
	Key "space": turn on/off the spot light from the viewer's perspective (like flashlight)

	//Texture control
	Key "1" and "2": to switch two different textures for the penguin
	Key "3" and "4": to switch two different textures for the snowfield
	Key "5" and "6": to switch two different textures for the cat 1
	Key "7" and "8": to switch two different textures for the cat 2
	
	//Object control
	Key "↑": forward movement of the penguin
	Key "↓": backward movement of the penguin
	Key "←": left rotation of the penguin
	Key "→": right rotation of the penguin

	//View control
	Dragging mouse (no press) to the left, right, up and down: rotate the camera accordingly slightly

	Dragging mouse (pressing left button) to the left: rotate the camera in clockwise direction of x-z plane
	Dragging mouse (pressing left button) to the right: rotate the camera in anti-clockwise direction of x-z plane
	Dragging mouse (pressing left button) to the upside:move the camera upward and still look at the objects
 	Dragging mouse (pressing left button) to the downside:move the camera downward and still look at the objects

SOMETHING INTERESTING：
I draw two cats, one street lamp, one bench and some grass to make the whole scene more vivid. The object and texture files are mainly from https://github.com/sinyiwsy/CSCI-3260-Assignment2.
Five point light sources and one spot light source are implemented to show more interesting scene. We can control them with keys "Q", "E" and "space" according to the above.
I also implement the shadow mapping to draw the shadow. Meanwhile, the shadow can move according to the movement of the directional light by pressing keys "A" and "D".

	