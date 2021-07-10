#Oware Game

##GAME INSTRUCTIONS

The oware game was implemented by the following rules:

- There are 6 houses for each players. Each house starts with 4 seeds 
and player one can choose an house from 1 to 6 and player two can choose an house from 7 to 12.

- The seeds of house six are parsed throw the following houses, starting at house 7. By the same way of thought, the 
seeds from house 12 are parsed starting at the house 1.

- A player can only harvest seeds when the last seed from the house he wanted to parse lands on a house
with 1 or 2 seeds.
If this condition happens, then if the previous house has 2 or 3 seeds it will be harvested to. This will be repeated from the house before 
the last house and so on until there are no houses that match this conditions.

- A player has to leave options of playing for the oponent, that is, if the play he wants to do will block the oponent to play, 
then he can't choose that move. In other way of saying, a player can set all of his houses with 0 seeds but cannot set all oponent houses with 0 seeds.

- The game ends when a player reaches 24+ points, when the both players reach 24 points or when players introduce input 0.

##HOW TO RUN
In order to run this game, you just need to run the "Source.exe" file.