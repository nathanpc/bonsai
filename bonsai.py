#!/usr/bin/env python

import socket
import sys

sock = None


# Create a IPv4 TCP socket
def create_socket(sock):
	try:
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	except socket.error, msg:
		print "Failed to create socket. (" + str(msg[0]) + ") " + msg[1]
		sys.exit()

# Open the socket to the world
def open_socket(sock, port = 80):
	try:
		sock.bind((socket.gethostname(), port))
		sock.listen(50)  # Define max connections
	except socket.error, msg:
		print "Failed to open socket. (" + str(msg[0]) + ") " + msg[1]
		sys.exit()
	
	print "bonsai now listening on port " + port



if __name__ == "__main__":
	create_socket(sock)
	open_socket(sock, 8430)