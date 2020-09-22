package main

import (
	"golang.org/x/tour/wc"
	"strings"
)

func WordCount(s string) map[string]int {

	res := make(map[string]int)
	word := strings.Fields(s)

	for _, w := range word {
		res[w]++

	}
	return res
}

func main() {
	wc.Test(WordCount)
}
