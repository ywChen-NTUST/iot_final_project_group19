import socket
from modules.command import commandHandler
# Define the server's IP address and port
HOST = '0.0.0.0'
PORT = 12345

VERBOSE = False

# Create a socket object
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the IP address and port
server_socket.bind((HOST, PORT))

# Listen for incoming connections
server_socket.listen()

print(f"Server is listening on {HOST}:{PORT}")

server_socket.settimeout(0.5)

while True:
    # Accept a client connection
    try:
        client_socket, client_address = server_socket.accept()
        if VERBOSE:
            print(f"Accepted connection from {client_address[0]}:{client_address[1]}")

        # Receive data from the client
        data = client_socket.recv(1024)
        if VERBOSE:
            print(f"Received data: {data}")

        # Process the data (add your logic here)
        response = commandHandler(data)

        # Send a response back to the client
        # response = "Hello from the server!"
        if VERBOSE:
            print(f"Send data: {response + bytes([0])}")
        client_socket.send(response + b"\x00")

        # Close the client connection
        client_socket.close()
    except socket.timeout:
        # print("Timeout")
        pass
    except KeyboardInterrupt:
        print("\nClosing server socket...")
        server_socket.close()
        print("Server socket closed.")
        break

