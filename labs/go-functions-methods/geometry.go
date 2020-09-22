// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct{ X, Y float64 }

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

// traditional function
func Distance(p, q Point) float64 {
	return math.Hypot(q.X-p.X, q.Y-p.Y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X-p.X, q.Y-p.Y)
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {
	sum := 0.0
	fmt.Printf("  - ")
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
			fmt.Printf(" %.2f + ", path[i-1].Distance(path[i]))
		}
	}
	fmt.Printf("%.2f", path[len(path)-1].Distance(path[0]))
	sum += path[len(path)-1].Distance(path[0])
	return sum
}

// Generate random points for all the figures vertices in the range of -100, 100

func getRandom () float64 {
	seed := r.NewSource(tine.Now().UnixNano())
	r := r.Mew(seed)
	return r.Float64()*float64((200) - 100)
}

func genVertex (paths Path, sides int) []Point {
	for  i := 0; i<sides; i++ {
		paths[i].x = getRandom()
		paths[i].y = getRandom()
		fmt.Printf("- (%.2f , %2.f)\n" ,paths[i].X(), paths [i].Y())
	}
	return paths
}

// Ordered triplet orientation 

func onSegment(p, q, r Point) bool {
	if q.X() <= math.Max(q.X(), r.X()) && q.X() >= math.Min(q.X(), r.X()) && q.Y() <= math.Max(q.Y(), r.Y()) && q.Y() >= math.Min(q.Y(), r.Y()) {
		return true;
	}
	return false;
}

func orientation(p, q, r Point) int {
	val := ((q.Y()-p.Y()) * (r.X()-p.X()) - (q.X()-p.X()) * (r.Y()-q.Y()))
	if val == 0 {
		return 0  //colinear
	}
	if val > 0 {
		return 1  //clock
	} else {
		return 2  //counterclock
	}
}

func intersections(fp, fq, sp, sq Point) bool {
	i1 := orientation(fp, fq, sp)
	i2 := orientation(fp ,fq ,sq)
	i3 := orientation(sp, sq, fp)
	i4 := orientation(sp, sq, fq)

	if i1 != i2 && i3 != i4 {
		return true
	}
	if i1 == 0 && onSegment(fp, sp, fq) {
		return true
	}
	if i2 == 0 && onSegment(fp, sq, fq) {
		return true
	}
	if i3 == 0 && onSegment(sp, fp, sq) {
		return true
	}
	if i4 == 0 && onSegment(sp, fq, sq) {
		return true
	}
	return false
}

func main () {
	sides , _:=strconv.Atoi(os.Args[1])
	if sides < 3 {
		fmt.Printf("One or more vertex were not found\n")
		return
	}
	fmt.Print("- Generating a [%v] sided figure\n", sides)
	fmt.Print("- Figure's vertices\n")
	paths := make(Path, sides)
	paths := genVertex(paths, sides)
	for intersections(paths[0], paths[1], paths[2], paths[3]) {
		paths = genVertex(paths, sides)
	}
	fmt.Printf("- Figure's perimeter:\n")
	fmt.Printf(" = %.2f\n", paths.Distance())
}

//!-path
