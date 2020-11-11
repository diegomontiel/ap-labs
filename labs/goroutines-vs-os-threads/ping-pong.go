package main

import(
	"fmt"
	"time"
)
var comms int

func ping(in chan bool, out chan bool){
	for {
		out <- true
		<- in
		comms++
	}
}
func pong(in chan bool, out chan bool){
	for {
		<- in
		comms++
		out <- false
	}
}
func main(){
	ch1 := make(chan bool)
	ch2 := make(chan bool)
	fmt.Println("Welcome to Ping Pong")
	go ping(ch1, ch2)
	go pong(ch2, ch1)
	for i:=0; i<10; i++ {
		time.Sleep(time.Second)
		fmt.Println("Comunications per second:",comms)
		comms = 0
	}
}