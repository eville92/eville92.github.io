/* SERVER */

// utilities, libraries, etc
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <unistd.h> // read(), unlink()
#include <sys/types.h> // pid_t, ssize_t
#include <sys/stat.h> // mkfifo()
#include <fcntl.h> // open(), O_RDONLY
#include <errno.h>  // errno
#include <string.h> // strings

// default macros
#define FIFO1 "/tmp/fifo.1" 
#define FILE_MODE 0666
#define MAX 1024
#define MAX_CLIENTS 111

// globals
int wait_queue[MAX_CLIENTS];
int wait_count = 0, mutex_locked = 0, owner_pid = -1;

void logs(int pid, int system_call, int parameter_number, char *parameter_str);
void err_sys(const char *msg) { perror(msg); exit(1); }

typedef struct { int pid, writefd; char stored[MAX]; } client_t;

int main(int argc, char **argv) {
    int readfd, writefd, system_call, parameter_number, pid, size;
    int client_count = 0;
    client_t clients[MAX_CLIENTS];
    char buffer[MAX], parameter_str[MAX];
    ssize_t n;
    
    /* create a FIFO; OK if already exist */    
    if( (mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST) )
        perror("can't create FIFO1");

    readfd = open(FIFO1, O_RDWR);
    if (readfd < 0) { perror("cannot open FIFO :("); }
    while(1) {
        memset(buffer, 0, sizeof(buffer));
        while ((n = read(readfd, buffer, sizeof(buffer))) > 0) {
            buffer[n] = '\0';            
            memset(parameter_str, 0, sizeof(parameter_str));
            sscanf(buffer, "%d,%d,%d,%d,%[^\n]", &pid, &system_call, &parameter_number, &size, parameter_str);
            int client_fd = -1, found = 0;
            char response[MAX];
        
            switch (system_call) {
                // Shutdown
                case -1:
                    printf("shutdown requested by client %d\n", pid);
                    for (int i = 0; i < client_count; i++) { close(clients[i].writefd); }
                    unlink(FIFO1);
                    printf("server FIFO %s removed. exiting.\n", FIFO1);
                    exit(0);
                
                // Exit
                case 0: 
                    for (int i = 0; i < client_count; i++) {
                        if (clients[i].pid == pid) {
                            close(clients[i].writefd);
                            for (int j = i; j < client_count - 1; j++) { clients[j] = clients[j+1]; }
                            client_count--;
                            printf("client %d disconnected.\n\n", pid);
                            found = 1;
                            break;
                        }
                    }
                    if (!found) { fprintf(stderr, "client pid %d not found for exit :(\n", pid); }
                    break; 
                
                // Connect System Call
                case 1:
                    memset(clients[client_count].stored, 0, MAX);
                    writefd = open(parameter_str, O_WRONLY);
                    if (writefd < 0) {
                        fprintf(stderr, "failed to open client FIFO %s\n", parameter_str);
                        break; }

                    if (client_count < MAX_CLIENTS) {
                        clients[client_count].pid = pid;
                        clients[client_count].writefd = writefd;
                        client_count++;
                        printf("Client connected: pid = %d, FIFO = %s\n\n", pid, parameter_str);
                        snprintf(response, sizeof(response), "%d", pid); }
                    else { fprintf(stderr, "too many clients connected\n"); }
                    break;
                    
                // Number
                case 2: 
                    for (int i = 0; i < client_count; i++) {
                        if(clients[i].pid == pid) {
                            client_fd = clients[i].writefd;
                            break; }
                    }
                    if (client_fd == -1) {
                        fprintf(stderr, "client pid %d not found\n", pid);
                        break; }
                    logs(pid, system_call, parameter_number, parameter_str);
                    snprintf(response, sizeof(response), "Server received number: %s\n", parameter_str);
                    write(client_fd, response, strlen(response));
                    fsync(client_fd);
                    
                    break;

                // Text
                case 3: 
                    for (int i = 0; i < client_count; i++) {
                        if (clients[i].pid == pid) {
                            client_fd = clients[i].writefd;
                            break; }
                    }
                    if (client_fd == -1) {
                        fprintf(stderr, "client pid %d not found\n", pid);
                        break; }
                        
                    logs(pid, system_call, parameter_number, parameter_str);
                    snprintf(response, sizeof(response), "Server received text:%s\n", parameter_str);
                    write(client_fd, response, strlen(response));
                    break; 
                    
                // Store
                case 4: 
                    for (int i = 0; i < client_count; i++) {
                        if (clients[i].pid == pid) {
                            char param_copy[MAX];
                            strncpy(param_copy, parameter_str, MAX - 1);
                            param_copy[MAX - 1] = '\0';
                            logs(pid, system_call, parameter_number, param_copy);
                            strncpy(clients[i].stored, parameter_str, MAX - 1);
                            clients[i].stored[MAX - 1] = '\0';
                            snprintf(response, sizeof(response), "stored: %s\n", clients[i].stored);
                            write(clients[i].writefd, response, strlen(response));
                            fsync(clients[i].writefd);
                            found = 1;
                            break;
                        }
                    }
                    if (!found) { fprintf(stderr, "client pid %d not found for store\n", pid); }
                    break;
                    
                // Recall
                case 5: 
                    for (int i = 0; i < client_count; i++) {
                        if (clients[i].pid == pid) {

                            printf("client pid: %d\n", pid);
                            printf("system call requested: %d with no parameters\n", system_call);
                            
                            if (strlen(clients[i].stored) > 0) { snprintf(response, sizeof(response), "%s\n", clients[i].stored); }
                            else { snprintf(response, sizeof(response), "no stored value found :(\n"); }
                            
                            printf("Server found stored parameters: %s\n", response);
                            write(clients[i].writefd, response, strlen(response));
                            fsync(clients[i].writefd);
                            found = 1;
                            break;
                        }
                    }
                    if (!found) { fprintf(stderr, "client pid %d not found for recall :(\n", pid); }
                    break;
                
                // Wait Mutex
                case 6:
                    for (int i = 0; i < client_count; i++) {
                        if(clients[i].pid == pid) { 
                            printf("client pid: %d\n", pid);
                            printf("system call requested: %d with no parameters\n\n", system_call); 
                            
                            if (!mutex_locked) {
                                // grant access
                                mutex_locked = 1;
                                owner_pid = pid;
                                snprintf(response, sizeof(response), "GRANTED to Client %d", pid);
                                write(clients[i].writefd, response, strlen(response));
                            } // END IF !mutex locked
                            else { wait_queue[wait_count++] = pid; }
                            break;
                        } // END IF clients[i]
                    } // END FOR client_count
                    break;
                
                // Signal Mutex
                case 7:
                    for (int i = 0; i < client_count; i++) {
                        if(clients[i].pid == pid) { 
                            printf("client pid: %d\n", pid);
                            printf("system call requested: %d with no parameters\n\n", system_call); 
                            
                            if (mutex_locked && owner_pid == pid) {
                                if (wait_count > 0) {
                                    
                                    // next client in line
                                    int next_pid = wait_queue[0];
                                    for (int j = 0; j < wait_count - 1; j++) { wait_queue[j] = wait_queue[j+1]; }
                                    wait_count--;
                                    owner_pid = next_pid;
                                    
                                    int next_fd = -1;
                                    for (int k = 0; k < client_count; k++) {
                                        if (clients[k].pid == next_pid) { next_fd = clients[k].writefd; break; }
                                    } // END FOR k client count
                                    
                                    if (next_fd >= 0) {
                                        snprintf(response, sizeof(response), "GRANTED to Client %d\n", pid);
                                        write(next_fd, response, strlen(response));
                                    } // END IF next_fd >= 0
                                    else { mutex_locked = 0; owner_pid = -1; }
                                } // END IF wait count > 0
                                
                                // unlock
                                else { mutex_locked = 0; owner_pid = -1; } 
                                
                                // reply to releasing client
                                snprintf(response, sizeof(response), "RELEASED\n\n");
                                write(clients[i].writefd, response, strlen(response));
                            } // END IF mutex locked && owner pid
                            else {
                                snprintf(response, sizeof(response), "DENIED\n\n");
                                write(clients[i].writefd, response, strlen(response)); }
                            break;
                        } // END IF clients[i]
                    } // END FOR client_count
                    break;
                    
                // Unknown call
                default: 
                    fprintf(stderr, "unknown system call: %d\n", system_call);
                    break; 
            } // END switch
        } // END while #2
    } // END while #1
} // END main()

void logs(int pid, int system_call, int parameter_number, char *parameter_str) {
    int parameter_i = 1;
    printf("client pid: %d\n", pid);
    printf("system call requested: %d with %d parameter%s which %s:\n", system_call, parameter_number,
         parameter_number == 1 ? "" : "s", 
         parameter_number == 1 ? "is" : "are");
    if (parameter_number == 1 && strlen(parameter_str) > 0) {
        printf("Parameter = %s\n", parameter_str); }
    else if (parameter_number > 1 && strlen(parameter_str) > 0) {
        char param_copy[MAX];
        strncpy(param_copy, parameter_str, MAX - 1);
        param_copy[MAX - 1] = '\0';
        
        char *token = strtok(param_copy, ",");
        while (token != NULL) {
            printf("Parameter %d = %s ", parameter_i++, token);
            token = strtok(NULL, ","); }
        //printf("\n"); // what for?? lol
    } 
    else { printf("no parameters were provided :("); }
    printf("\n\n");
}
