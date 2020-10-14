package main

import (
	"log"
	"net"
	"time"
	"os"
	"fmt"
	"strings"
)

func main(){
	args := os.Args[1:]
	if len(args) < 1 {
		fmt.Println("Missing parameters\n")
		os.Exit(1)
	}

	for i := 0; i < len(args); i++ {
		placetime := strings.Split(os.Args[i], "=")
		timeZone := city[0]
		port := city[1]
		conn, err := net.Dial("tcp", placetime[1])

		if err != nil{
			log.Fatal(err)
		}

		go makeConn(conn, placetime[1])
	}

	for {

	}
}

func makeConn(conn net.Conn, city string) {
	for {
		data := make([]byte, 11)
		_, err := connection.Read(data)
		if err != nil {
			conn.Close()
			fmt.Printf("%s", err)
			return

		}
		fmt.Printf("%s: %s", city, data)
	}
}
