Student: Kirk Saunders
Email: ks825016@ohio.edu
Project: Assignment #1 - Food Drop Game

    For this assignment, I created a 2D food drop game in which the goal is feed all of the good guys
with the fewest amount of drops possible, all the while keeping the food away from the bad guys. The player
wins if they can feed all of the good guys without running out of drops.

    To implement selection in this game, I assigned every rendered object an ID. Then, I encoded this ID as a 24-bit
RGB color, where the red channel is the least significant byte, the green the next byte, and then the blue
the next byte. Thus, this allows 2^24 unique objects to be selectable at once on the screen. The first step to selection
is to clear the back buffer with glClear. Then, to tell what object the user selected, I first render all of the objects
using their selection colors to the back buffer with a completely white background color. Then, after ensuring I have
flushed the gl draw calls, I read the color of the pixel in the back frame buffer where the user clicked. I compare this
pixel color to each object's selection color to figure out which object was selected. I also slightly enlarge objects when
doing the selection draw to allow the user to select objects even when they aren't directly over them when they click.
At the end, I set the background color back to the background color of the game.

    In the game, the selection is used to ensure that the player doesn't drop food on a tree or another food drop,
and to make the good guys move faster when clicked, and the bad guys move slower when clicked. Thus, when the user
clicks in the window, I process the selection and depending on what was selected, perform an action. If a tree or
food drop is selected, I do nothing, as that location is not available for a drop. If a good guy is selected, I speed
it up for a period of time. If a bad guy is selected, I slow it down for a period of time.

    While the user can use selection to perform actions in the game, the good guys and bad guys are performing a different
kind of selection themselves continuously. The units (good guys and bad guys) continuously wander randomly around the game,
but if they happen to see a food drop, they target it. Their vision is obscured by trees, and so is their movement, so their
selection is nontrivial to implement. To implement it, I used some vector projection and distance checking math. If the food
is out of the vision range of the unit, they automatically can't see it. If it is within range, I find the vector projection
of the (tree_pos - unit_pos) vector onto the (food_pos - unit_pos) vector and compare the distance to the tree's center to see
if the tree falls within the unit's vision. This is essentially a simple form of 2D circle raycasting. I believe this
implementation deserves some coolness points because it adds more depth to the game and brings the units to life, while also
fulfilling the "trees should pose as barriers" requirement.

    The movement/animation of game objects around the screen is accomplished by using delta time between each frame to update object
positions, sizes, and rotations based on target values and speeds. Then, to reflect those changes onto the screen, each object passes
a position, size, and rotation uniform variable to the shader so that it is drawn accordingly. The vertex shader then handles scaling,
positioning, rotation, and coloring. Each object's size and position are absolute in relation to monitor size, so even if the window
is resized, the objects stay the same size and position from the center of the window. This is accomplished by sending the window size
as a uniform variable to the vertex shader, which then in turn scales all vertex coordinates based on window size (x coordinate is
divided by windowsize.x/2 and y coordinate is divided by windowsize.y/2). Rotation is achieved by taking the given rotation variable
and creating a 2D rotation transform matrix from it, as described in class, and as I did in my previous assignment.

    All of this game is completely dynamic, which means that the map, number of bad guys, number of good guys, and everything else can
be whatever you want. Thus, the map is generated randomly and can be re-generated using the 'r' key. Thus, the user can keep playing
and generating new games and never have the same experience twice, which adds to the replayability of the game. I believe that deserves
some coolness points.

    All of the objects in the game have slight shading - vertices further from the object's center, origin position are darker than
vertices closer to the origin. This especially gives circles a nice shading. (This is all done in the vertex shader.)

Game features/requirements met:
    - Selection (Selection to confirm drop location is good, selection for unit speedup/slowdown)
    - Scoring system (Score goes up when good guys eat, goes down when bad guys eat)
    - Bad guys intercepting food (if they get to the food, not only does score go down, but they take
                                  the food and little/none is left for good guys)
    - Trees block drops and serve as obstacles for units
    - Players have a limited number of drops to make
    - Mutliple targets (An unlimited number of units can target a food drop at once)

Things I believe I deserve coolness points for:
    - Units being able to "see" food and target it (They're alive!)
    - Trees actually obscuring units' vision
    - The whole game being dynamic and thus able to be randomly generated
    - The selection is future proof (able to support 2^24 objects even though I won't have that many)
    - Being able to speed up or slow down units by selecting them
    - The game (in my opinion) looks nice and clean (I think the shading is a nice touch)
    - I added an extra goal to the game - the goal is to feed all of the good guys until they are all full,
      at which point they disappear.
    - The animations are clean and smooth

If you wish to read more about my design decisions and game details, here was what I wrote as planning for the assignment:

Game details:
    Gameplay Details:
        Both good guys and bad guys move towards food drops. Bad guys move faster and have more range.

        Food diminishes over time (decomposes). Also, good guys and bad guys take an amount of a food drop
        every game step, they don't take it all at once. Bad guys take faster and have no limit to how much
        they take. Good guys take food more slowly and have a limit of how much they can take, thus two good guys taking
        means less food wasted, more points for the player, and thus incentive to target multiple good guys at
        once. Once a good guy gets its fill, it disappears (you have fed that good guy).

        When good guys and bad guys don't see any food, they wander randomly. Once again, the good guys move more slowly.

        Food drops can't be dropped in trees, nor can they be dropped on top of other food drops.

        When good guys get food, the player's score goes up by the amount they get. When bad guys get food, the player's score
        goes up by the amount they get. When a good guy gets its fill, the player's score gets a boost by double
        the amount that the good guy ate.

        Food drops to exactly where the user clicks, the good and bad guys move towards it.

        Good guys and bad guys cannot walk through trees.

        Trees obscure both good guys' and bad guys' vision of food.

        If the player clicks on a good guy, they move faster for a period of time. If the player clicks on a bad guy, they
        move slower for a period of time.

        The game is generated randomly and can be regenerated by pressing the 'r' key.

    Graphics Details:
        Good guys are colored blue, food colored yellow, bad guys colored red, and trees colored green.

        Food is circles, good guys and bad guys are chevron-type shapes, and trees are circles.

        Trees are the largest objects, good guys and bad guys are the same size, food is slightly smaller than good guys and bad
        guys.

        Score and drops left are displayed in the window title bar.