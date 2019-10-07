
Gameplay Idea:

    Both good guys and bad guys move towards food drops. Bad guys move faster and have more range.

    Food diminishes over time (decomposes). Also, good guys and bad guys take an amount of a food drop
    every loop step, they don't take it all at once. Bad guys take faster and have no limit to how much
    they take. Good guys take food more slowly and have a limit of how much they can take, thus two good guys taking
    means less food wasted, more points for the player, and thus incentive to target multiple good guys at
    once.

    When good guys and bad guys don't see any food, they wander randomly, updating their goal positions every 3
    seconds. Once again, the good guys move more slowly.

    If a food drop lands in a tree, it can't be used. There is no point penalty for losing food, but there are limited
    food drops.

    When good guys get food, the player's score goes up by the amount they get. When bad guys get food, the player's score
    goes up by the amount they get.

    Food drops to exactly where the user clicks.

    Good guys and bad guys will not be able to walk through trees.

    Trees obscure both good guys' and bad guys' vision of food.

Graphics Idea:

    Good guys will be colored blue, food colored yellow, bad guys colored red, and trees colored green/orange (fall time!).

    Food will be circles, good guys and bad guys will be triangles. Trees will be circles.

    Trees will be the largest objects, good guys and bad guys will be same size, food will be slightly larger than good guys and bad
    guys.

    Score and (possibly?) level count will be displayed in the window title bar.

Extra Details:

    Line segment-sphere intersection will be done using a dot product and distance compare method.

    There will be a game class that keeps track of objects and draws them.

    There will be a generic abstract object class that all drawn objects will inherit from.