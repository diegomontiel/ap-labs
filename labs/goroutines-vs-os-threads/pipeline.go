package main

import(
	"fmt"
	"time"
)

func routine(n int, returnCH chan time.Time){
	if n > 1 {
		getCH := make(chan time.Time)
		go routine(n - 1, getCH)
		transitTime := <- getCH
		returnCH <- transitTime
		return
	}
	returnCH <- time.Now()
}

func main(){
	var maxStages int
    maxStages=100000
	getCH := make(chan time.Time)
	go routine(maxStages, getCH)
	transitTime := <- getCH
    fmt.Println("Maximum number of pipeline stages   : ", maxStages)
	fmt.Println("Time to transit trough the pipeline : ", time.Since(transitTime))
}