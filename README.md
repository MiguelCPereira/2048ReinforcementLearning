# 2048 Reinforcement Learning

An AI that learns how to play the game [2048](https://en.wikipedia.org/wiki/2048_(video_game)) through deep reinforcement learning.
The final result ended up falling short of my goal, as I explain in more detail down in the [Results](#results) section, but if for nothing else, this project still showcases the basic concepts of Machine Learning and Neural Networking, without any dependencies to 3rd party frameworks / topic-specific libraries.


![Demo](./Data/Demo.gif)



## Table of Contents
- [Reinforcement Learning](#reinforment-learning)
- [Neural Networking](#neural-networking)
- [Game State and Reward](#game-state-and-reward)
- [Training](#training)
- [Results](#results)
- [Links](#links)



## Reinforcement Learning

For the bot to learn how to play, every time it makes a move, it saves all the information regarding its environment (the pieces in the board before and after the move, the move itself, its reward and if it reached a game over) in a container - its memory. With every said move, it will also update its [neural network](#neural-networking) and, at the end of every game, it will train itself with a random chunk of the moves previously stored in the memory (through the neural network once again). This way, it will start off by making random moves at first, but it will eventually start learning from its past experiences, taking into account the [rewards](#game-state-and-reward) it got from each decision.

To avoid stagnation, this prediction of moves is intervaled with an increasingly smaller chance of random decisions - as, in reinforcement learning, once a supposedly optimal path is found, the bot will stick to it, stopping itself from exploring new random scenarios which might lead, in the long run, to a better score. This random factor starts off as a 50% chance, but it decreases every game until the 100th one, where it caps off at 10%.



## Neural Networking

The neural network is essencially the bot's "brain" - being responsible, as described before, for making the connections between each move and game state and optimizing itself as the [training](#training) goes on to achieve better results.

The way it works pratically is through multiple layers of a parameterizable number of neurons, which create connections and propagate between themselves to transform the given original input (the game state) into the final output (the predicted move). Neurons are, to put it simply, a set of inputs, a set of weights, and an activation function which transform whatever input they're given into a new output, which can then be picked up and processed by the corresponding neuron in the next layer, progressively optimizing the carried value.

In the network used in this project, the first layer has a size of 16, 1 for each house in the board (as I describe better in the [Game State and Reward](#game-state-and-reward)) section) and the final one 4, 1 for each possible move. These 2 correspond to the initial input and final output, and between them there's 256 hidden layers. Each neural layer is stored in a vector as a RowVectorXf (from the [Eigen library](https://eigen.tuxfamily.org/index.php?title=Main_Page)), one more RowVectorXF vector stores the unactivated values of layers, another one stores the error contribution of each neuron, and finally a last matrix vector stores the weigths of each connetion. All of these are updated with every training step.



## Game State and Reward

The way the game state is fed to the network and the way the reward is calculated are 2 crucial factors when it comes to optimizing a Reinforcement Learning bot. As such, I did multiple training sessions with different iterations of these values (as I better describe in the [Results](#results) section) but ended up settling on these:
- The game state represented as an int vector of size 16, storing each of the board's houses info as the log2 value of the piece occupying it (so, intead of 2, 4, 8, 16, it's 1, 2, 3, 4);
- And the reward, the addition of all the log2 values of the collapsed pieces, minus a penalty of -1 for moving and another penalty for making an ilegal move (not changing anything on the board).



## Training

The bot could restrain itself to learn only from the direct reward of each current move, but that would make their behaviour very limited and lacking in foresight - stopping itself from making moves with a worse direct reward, even in exchange for a better future one (for example, not making the most combinations possible in a move if it entails setting up a bigger future combination). To avoid this, the way it actually trains itself is based on the [Q-Learning algorithm](https://en.wikipedia.org/wiki/Q-learning), which calculates the optimal move, if it's not the last one in the game, by adding the current reward to the next move's prediction multiplied by a discount rate (in this case, 0.9), and then propagates this result through the rest of the network.



## Results

After 1150 episodes, in its current state, the bot didn't show much of a significant improvement, which was honestly underwhelming. As much as I iterated with different possible values, the scores average increased only slightly from 749 (at the 30th game) to 855 (at the last one), and the bot seemed to not learn any basic game strategies, like preserving high-value pieces in corners. I tried:
- Changing the learning rate between 0.001f and 0,005f - setting on 0.001f;
- Changing the randomness factor between 50 and 120, as well as capping it at 10% or letting it go to 0% - setting on 100 with a 10% cap;
- Adding from 100 up to 300 hidden layers to the neural network - setting on 256;
- Coding the game state as a vector of 16 ints, one for each square's current value, and still 16 but with the log2 of each value (so 2 becomes 1, 4-2, 8-3, 16-4, etc) - setting on the 2nd option;
- And making the move reward the score, the total value of squares in the board, the total ammount of collapsed pieces, and the sum of the log2 of the collapsed pieces value - setting on the last one.

All the scores from this last unsuccessful train session can be found here:


![Score Graph](https://github.com/MiguelCPereira/2048ReinforcementLearning/blob/main/Data/Training%20Chart.png)



## Links

Theses are some of the articles and tutorials I found to be really helpful during the making of this project. I highly recomment checking them out.

- Simple Neural Networking Explanation - https://becominghuman.ai/artificial-neuron-networks-basics-introduction-to-neural-networks-3082f1dcca8c
- Neural Networking C++ Application - https://www.geeksforgeeks.org/ml-neural-network-implementation-in-c-from-scratch/
- Back Propagation - http://galaxy.agh.edu.pl/~vlsi/AI/backp_t_en/backprop.html
- Reinforcement Learning in 2048 - https://towardsdatascience.com/a-puzzle-for-ai-eb7a3cb8e599
- Python Snake Reinforcement Learning Tutorial - https://www.youtube.com/watch?v=PJl4iabBEz0&t=0s
- Eigen Library Intro - https://www.youtube.com/watch?v=XmtNr1TuO-E&t=123s