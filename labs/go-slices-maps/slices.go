package main

import "golang.org/x/tour/pic"

func Pic(dx, dy int) [][]uint8 {
	var picture[][]uint8
	picture = make([][]uint8, dy)
	for y := range picture {
		picture[y] = make([]uint8, dx)
		for x := range picture[y] {
			//picture[y][x] = uint8((x+y)/2) //blue gradient
			//picture[y][x] = uint8(x*y) //cuadratic pattern
			picture[y][x] = uint8(x^y) //square pattern
		}
	}
	return picture
}

func main() {
	pic.Show(Pic)
}
