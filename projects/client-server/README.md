# Client/Server - Scanner & Recursive Descent Parser

## Description
Two programs, client and server, are run concurrently. The programs run independently and can perform simple IPC using FIFOs and named pipes to pass information back and forth in a client/server fashion.

## Instructions
Programs must be compiled before running.
1. Compile `server.c` and `client.c` via:
   ```bash
   gcc server.c -o server
   gcc client.c -o client
   ```
2. Run `server` first via:
   ```bash
   ./server
   ```
3. Run `client` next via:
   ```bash
   ./client
   ```
4. Follow the client's menu from there. 

## Project Structure
```
hawks/
│
├── hawks.c
├── input.txt
├── test-cases/
│   ├── script1.txt
│   ├── script2.txt
│   ├── script3.txt
│   ├── script4.txt
│   ├── script5.txt
│   ├── script6.txt
│   ├── script11.txt
│   ├── script12.txt
│   ├── script13.txt
│   ├── script14.txt
│   └── script15.txt
└── README.md
```

## Notes
- The server program can handle multiple clients (max 111). Simply run the `client` program again to start another connection.
