// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"time"
	"strings"
)

//!+broadcaster
type msgChan chan<- string // an outgoing message channel
type client struct {
	channel msgChan
	name string
	ip string
	conn net.Conn
	admin bool
}

var (
	entering = make(chan client)
	leaving  = make(chan msgChan)
	messages = make(chan string)
	clients  = make(map[msgChan]*client) //connected clients
)

func broadcaster() {
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case clientData := <-entering:
			if len(clients) == 0 {
				clientData.admin = true
			}
			clients[clientData.channel] = &clientData

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	var userAvailable = false
	input := bufio.NewScanner(conn)
	input.Scan()
	who := input.Text()
	for _,c := range clients {
		if c.name == who {
			userAvailable = true
			fmt.Fprintln(conn, "Username already taken")
			conn.Close()
			break;
		}
	}

	if !userAvailable {
		ch := make(chan string) // outgoing client messages
		go clientText(conn, ch)
		ip := conn.RemoteAddr().String()

		ch <- serverInfo + "Welcome to the Simple IRC Server"
		ch <- serverInfo + "Your user [" + who + "] is successfully logged"
		fmt.Println(serverInfo + "New connected user [" + who + "]")
		messages <- serverInfo + "New connected user [" + who + "]"

		entering <- client{ch, who, ip, conn, false}

		if clients[ch].admin == true{
			ch <- serverInfo + "Congrats, you were the first user."
			promote(who, ch)
		}

		for input.Scan() { // Incoming client messages
			msg := input.Text()
			if msg != "" {
				if msg[0] == '/' {
					values := strings.Split(msg, " ")
					switch values[0] {

				//USER INPUT (parameter check requiered)
					case "/msg": //msg to user
						if len(values) < 3 {
							ch <- serverInfo + "Incomplete parameters"
						} else {
							var check = false
							for _,p := range clients {
								if p.name == values[1] {
									check = true
									p.channel <- who + " (direct) > " + msg[6+len(p.name):]
									break;
								}
							}
							if !check {
								ch <- serverInfo + "Username not found, message failed."
							}
						}

					case "/user": //display user information
						if len(values) != 2 {
							ch <- serverInfo + "Error, correct usage is: /user <username>"
						} else {
							var check = false
							for _,p := range clients {
								if p.name == values[1] {
									check = true
									ch <- serverInfo + "username: " + p.name + ", IP: " + p.ip
									break;
								}
							}
							if !check {
								ch <- serverInfo + "Username not found, failed to display info"
							}
						}

					case "/kick": //kick any user 
						if clients[ch].admin {
							if len(values) != 2 {
								ch <- serverInfo + "Error, correct usage is: /kick <username>"
							} else {
								var check = false
								for _,p := range clients {
									if p.name == values[1] {
										check = true
										p.channel <- serverInfo + "You're kicked from this channel"
										leaving <- p.channel
										fmt.Println(serverInfo + p.name + " was kicked")
										messages <- serverInfo + p.name + " was kicked from channel"
										p.conn.Close()
										break;
									}
								}
								if !check {
									ch <- serverInfo + "Username not found, failed to kick"
								}
							}
						} else {
							ch <- serverInfo + "Error: only the admin can kick"
						}

				//NO PARAMS
					case "/users": //list all users
						var userNames string
						for _,c := range clients{
							userNames += c.name + ", "
						}
						ch <- serverInfo + userNames[:len(userNames)-2]

					case "/time": //display current time
						location,_ := time.LoadLocation("Local")
						local := location.String()
						if local == "Local"{
							curr,_ := time.LoadLocation("America/Mexico_City")
							local = curr.String()
						}
						ch <- serverInfo + "Local Time: " + local + " " + time.Now().Format("15:04")


					default:
						ch <- serverInfo + "Error, unknown command"
					}
				} else { // Message
					messages <- who + " > " + msg
				}
			}
		}
		if clients[ch] != nil {
			leaving <- ch
			fmt.Println(serverInfo + "[" + who + "] left")
			messages <- serverInfo + "[" + who + "] left channel"
			conn.Close()
		}
	}
}

func clientText(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}

func promote(who string, cli msgChan) {
	fmt.Println(serverInfo + "["+ who + "] was promoted as the channel ADMIN")
	clients[cli].admin = true
	cli <- serverInfo + "You're the new IRC Server ADMIN"
}
//!-handleConn

//!+main

var serverInfo string
func main() {
	if len(os.Args) != 5 {
		log.Fatal("Error in parameters, usage: go run client.go -host [host] -port [port]")
	}
	serverInfo = "irc-server > "
	server := os.Args[2] + ":" + os.Args[4]
	listener, err := net.Listen("tcp", server)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(serverInfo + "Simple IRC Server started at " + server)
	go broadcaster()
	fmt.Println(serverInfo + "Ready for receiving new clients")
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
