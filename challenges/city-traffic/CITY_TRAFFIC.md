# Traffic Simulator Golang
 Multi thread traffic simulator made in Golang. This program simulates a small portion of a city, where the number of cars and traffic lights can be adjusted, each one of them controlled by a different thread. Cars follow given routes respecting the stop sign or color of the traffic light, cars ahead of them and the direction of the road.  

## Build Requirements
- Go version 1.12 or above as required by the Ebiten videogame library

## How to compile & run

```sh 
$ make test
```

### Menu:

- 1 Start game
- 2 Change options
    - 1 Select number of cars
    - 2 Select number of semaphores
    - 3 Return to main menu
- 3 Exit

Input the number of the action and hit enter after the "Number of option:" text appears on console.

### General rules:

- Max number of cars is 8

- Max number of traffic lights is 7

## Video
- https://youtu.be/4HfR_y1tAhI
