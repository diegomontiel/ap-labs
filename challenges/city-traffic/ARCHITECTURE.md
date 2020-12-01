## Description
Multi thread traffic simulator made in Golang with the help of **Ebiten module** ([Ebiten module link](https://ebiten.org/examples/)), we used 3 different threads one controlling the funcionality of the cars, another controlling the functionality of the traffic lights and one controlling all the game functionality we can think this thread as the game controller or game engine.
The number of cars and traffic lights can be changed in the beginning of the simulationa and cars stop when traffic lights become red
## Structs
We use 5 different structs for our simulator<br/>
<br/>
**semaphores**: Struct representing a sigle traffic light<br/>
<br/>
**car**: Struct representing a single car<br/>
<br/>
**route**: Struct representing the route a car will follow<br/>
<br/>
**trafficLightController**: Struct representing a single traffic light controller, it is used to draw the trffic lights in the map and also drawing the status of the traffic light<br/>

![alt text](https://raw.githubusercontent.com/obravocedillo/Traffic-Simulator-Golang/main/TrafficSimulatorStructs.png)

## Main Flow
Main flow of our traffic simulator
![alt text](https://github.com/obravocedillo/Traffic-Simulator-Golang/raw/main/TrafficSimulatorDIagram.png)

