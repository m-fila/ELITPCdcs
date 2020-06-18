#!/usr/bin/python3 -u
import socket
import numpy as np
from os import devnull
from subprocess import check_output

port = 2244

FNULL = open(devnull, 'w')
try:
    host = check_output(['hostname', '--all-ip-addresses'],stderr=FNULL).strip()
except:
    host = check_output(['hostname', '--ip-addresses']).strip()
finally:
    print("Listening on {}:{}".format(host.decode(),port))
    FNULL.close();
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((host, port))
# print host , port
s.listen(1)
try:
    while True:
        conn, addr = s.accept()
        print("Connected to ", addr)
    #    print('Connected by', addr)
        while True:
            try:
                data = conn.recv(16)
                if not data:
                    break
                if(data.decode() == 'GET'):
                    conn.sendall("{};{};{};{};{}\r\n".format(*np.random.rand(5)).encode("utf-8"))
            except socket.error:
                #            print "Error Occured."
                break
     #   conn.close()
except KeyboardInterrupt:
    pass;
    #conn.close();
finally:
    pass
    conn.shutdown(socket.SHUT_RDWR)
    conn.close();
