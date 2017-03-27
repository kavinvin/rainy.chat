import socket
import sys


def connect(host, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))
    while True:
        message = "=" + getMessage()
        s.sendall(message.encode())
        if message == "=/exit":
            s.close()
            return "exited"
    # data = s.recv(256)


def getHost():
    # 52.74.90.15
    try:
        host = sys.argv[1]
    except IndexError:
        print('enter host:', end=' ')
        host = input()
    return host


def getPort():
    try:
        port = int(sys.argv[2])
    except IndexError:
        print('enter port no.:', end=' ')
        port = int(input())
    return port


def getMessage():
    print('enter message:', end=' ')
    message = input()
    return message


data = connect(getHost(), getPort())
print(data)
