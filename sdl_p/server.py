import socket
import os

s = socket.socket()

host = '127.0.1.0'
port = 12000

s.connect(('', port))

content = "HEY"

s.send(content.encode("utf-8"))