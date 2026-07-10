/* CLIENT */

// utilities, libraries, etc
#include <stdio.h> // printf()
#include <stdlib.h> // exit()
#include <unistd.h> // read(), unlink()
#include <sys/types.h> // pid_t, ssize_t
#include <sys/stat.h> // mkfifo()
#include <fcntl.h> // open(), O_RDONLY
#include <errno.h>  // errno
#include <string.h> // strings

#define FIFO1 "/tmp/fifo.1" 
#define FILE_MODE 0666
#define MAX 1024
#define MAX_RUNS 20

void client(int readfd, int writefd);

int main( int argc, char **argv ) {
    int readfd, writefd; 
    writefd = open(FIFO1, O_WRONLY, 0); 
    client(-1, writefd); 
    close(writefd);
	
    exit(0); 
}

void client(int readfd, int writefd) {
    char client_FIFO1[64], buffer[MAX], parameter[MAX];
    int system_call, parameter_number, selection;
    
    snprintf(client_FIFO1, sizeof(client_FIFO1), "/tmp/fifo.client.%d", getpid());
    if( (mkfifo(client_FIFO1, FILE_MODE) < 0) && (errno != EEXIST) ) { // creates FIFO
        perror("can't create FIFO1");
        exit(1);
    }
    snprintf(buffer, sizeof(buffer), "%d,%d,%d,%zu,%s", getpid(), 1, 0, strlen(client_FIFO1), client_FIFO1);
    write(writefd, buffer, strlen(buffer));
    readfd = open(client_FIFO1, O_RDONLY | O_NONBLOCK, 0); // opens client
    if (readfd == -1) {
        perror("open client_FIFO1");
        unlink(client_FIFO1);
        exit(1);
    }
    
    while (1) {
        printf("\n1 - Send request to server\n2 - Exit\n3 - Shutdown\n4 - Execute CRITICAL Section\nSelect: ");
        scanf("%d", &selection);
        
		if (selection == 2 || selection == 3) {
			system_call = (selection == 2) ? 0 : -1;
            parameter_number = 0;
            parameter[0] = '\0';
            
            // server print
            snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d,%s", getpid(), system_call, parameter_number, 0, parameter);
            write(writefd, buffer, strlen(buffer));
            break;
		} // END IF selection 2 || 3
		
		else if (selection == 4) {
		    char request[256], response[256];
		    
		    // non-critical selection
		    printf("Client %d entering non-critical section\n\n", getpid());
		    for (int i = 0; i < MAX_RUNS; i++) { printf("Client %d: NON-CRITICAL SECTION (%d)\n", getpid(), i+1); sleep(1); }
		    printf("\nClient %d: waiting to get into CRITICAL SECTION\n", getpid());
		    
		    // Wait()
		    snprintf(request, sizeof(request), "%d,%d,%d,%d,%s", getpid(), 6, 0, 0, "");
		    write(writefd, request, strlen(request));
		    
		    // block until server replies
		    ssize_t n;
		    do {
		        n = read(readfd, response, sizeof(response));
		        usleep(10000); } 
		    while (n <=0);
		    response[n] = '\0';
		    printf("Server reply: Client %d entering CRITICAL SECTION\n\n", getpid());
		    
		    if (strncmp(response, "GRANTED", 7) == 0) {
		        
		        // critical section
		        for (int i = 0; i < MAX_RUNS; i++) { printf("Client %d: *** inside CRITICAL SECTION (%d) ***\n", getpid(), i+1); sleep(1); }
		        printf("Client %d: leaving CRITICAL SECTION\n", getpid());
		        
		        // Signal()
		        snprintf(request, sizeof(request), "%d,%d,%d,%d,%s", getpid(), 7, 0, 0, "");
		        write(writefd, request, strlen(request));
		        
		        // release
		        memset(response, 0, sizeof(response));
		        do { n = read(readfd, response, sizeof(response)); usleep(10000); }
		        while (n <= 0);
		        response[n] = '\0';
		        printf("Server reply: Client %d %s CRITICAL SECTION", getpid(), response);
		        
		    } // END IF strncmp(response)
		} // END selection 4
		else if (selection == 1) {            
            while (1) {
                printf("\nSystem Calls\n1 - Connect System Call\n2 - Number\n3 - Text\n4 - Store\n5 - Recall\n6 - Wait Mutex\n7 - Signal Mutex\n0 - Exit\n-1 - Shutdown\n\n");
                printf("What system call? ");
                scanf("%d", &system_call);
                
                switch(system_call) {
                    // shutdown (client & server)
                    case -1: {
                        printf("Shutting down..\n");
                        
                        // shutdown msg from server
                        snprintf(buffer, sizeof(buffer), "%d, %d", getpid(), system_call);
                        write(writefd, buffer, strlen(buffer));
                        unlink(client_FIFO1);
                        exit(1);
                    }
                        
                    // (exit | disconnect) client
                    case 0: {
                        printf("Exiting..\n"); 
                        parameter_number = 0;
                        parameter[0] = '\0';
                        
                        // server print
                        snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d,%s", getpid(), system_call, parameter_number, 0, parameter);
                        write(writefd, buffer, strlen(buffer));
                        exit(0); 
                    }
                        
                    // connect from client to server (automatically done when client runs)
                    case 1: {
                        printf("You're connected already, client %d! (8(l)\n", getpid());
                        continue;
                    }
                        
                    // number
                    case 2: {
                        printf("How many parameters? ");
                        scanf("%d", &parameter_number);
                        printf("Size of each parameter (max: 99): ");
                        size_t parameter_length;
                        scanf("%zu", &parameter_length);
                        
                        while (getchar() != '\n');
                        parameter[0] = '\0';
                        size_t remaining = sizeof(parameter);
                        
                        for (int i = 0; i < parameter_number; i++) {
                            char temp[64];
                            printf("enter parameter %d: ", i+1);
                            fgets(temp, sizeof(temp), stdin);
                            temp[strcspn(temp, "\n")]= '\0';
                            
                            if (strlen(temp) == 0) {
                                fprintf(stderr, "empty parameter. aborting. \n");
                                continue; }
                            size_t len = strlen(temp);
                            if (len > parameter_length) {
                                fprintf(stderr, "Parameter is too long!\n"); 
                                continue; }
                            if (len + 1 > remaining) {
                                fprintf(stderr, "Parameter overflow\n"); 
                                break; }
                            if (i > 0) {
                                strncat(parameter, ",", remaining - 1);
                                remaining -= 1; }
                            strncat(parameter, temp, remaining - 1);
                            remaining -= len;    
                        }
                        
                        //prints from server-side
                        snprintf(buffer, sizeof(buffer), "%d, %d, %d, %zu,  %s", getpid(), system_call, parameter_number, parameter_length, parameter);
                        write(writefd, buffer, strlen(buffer));
                        
                        ssize_t n;
                        do {
                            n = read(readfd, buffer, MAX);
                            usleep(10000); }
                        while (n <= 0);
                        buffer[n] = '\0';
                        printf("Parameter(s) sent to server. %s\n", buffer);
                        break; }
                        
                    // text
                    case 3: {
                        parameter_number = 1;
                        printf("Length for parameter string (max: 999)?: ");
                        size_t parameter_length;
                        scanf("%zu", &parameter_length);
                        
                        while (getchar() != '\n');
                        
                        memset(parameter, 0, sizeof(parameter));
                        memset(buffer, 0, sizeof(buffer));
                        parameter[0] = '\0';
                        size_t remaining = sizeof(parameter);
                        
                        for (int i = 0; i < parameter_number; i++) {
                            char temp[parameter_length + 1];
                            printf("Enter parameter %d: ", i+1);
                            fgets(temp, sizeof(temp), stdin);
                            temp[strcspn(temp, "\n")]= '\0';
                            
                            if (strlen(temp) == 0) {
                                fprintf(stderr, "empty parameter. aborting. \n");
                                continue; }
                            size_t len = strlen(temp);
                            if (len > parameter_length) {
                                fprintf(stderr, "Parameter is too long!\n"); 
                                continue; }
                            if (len + 1 > remaining) {
                                fprintf(stderr, "Parameter overflow\n"); 
                                break; }
                            strncat(parameter, temp, remaining - 1);
                            remaining -= len;    
                        }
                            
                        // prints from server-side
                        snprintf(buffer, sizeof(buffer), "%d, %d, %d, %zu, %s", getpid(), system_call, parameter_number, parameter_length, parameter);
                        write(writefd, buffer, strlen(buffer));
                       
                        memset(buffer, 0, sizeof(buffer));
                        ssize_t n;
                        do {
                            n = read(readfd, buffer, MAX);
                            usleep(10000); }
                        while (n <= 0);
                        buffer[n] = '\0';
                        printf("Parameter sent to server.\n%s", buffer);
                        break; }
                            
                    // store
                    case 4: {
                        printf("How many parameters? ");
                        scanf("%d", &parameter_number);
                        printf("Length of each word parameter (max 99): ");
                        size_t parameter_length;
                        scanf("%zu", &parameter_length);
                        while (getchar() != '\n');
                        
                        memset(parameter, 0, sizeof(parameter));
                        memset(buffer, 0, sizeof(buffer));                        
                        parameter[0] = '\0';
                        size_t remaining = sizeof(parameter);
                        
                        for (int i = 0; i < parameter_number; i++) {
                            char temp[parameter_length + 1];
                            printf("Enter parameter %d: ", i+1);
                            fgets(temp, sizeof(temp), stdin);
                            temp[strcspn(temp, "\n")]= '\0';
                            
                            if (strlen(temp) == 0) {
                                fprintf(stderr, "Empty parameter. Aborting. \n");
                                continue; }
                            size_t len = strlen(temp);
                            if (len > parameter_length) {
                                fprintf(stderr, "Parameter is too long!\n"); 
                                continue; }
                            if (len + 1 > remaining) {
                                fprintf(stderr, "Parameter overflow!\n"); 
                                break; }
                            if ( i > 0) {
                                strncat(parameter, " ", remaining - 1);
                                remaining -= len; 
                            }
                            strncat(parameter, temp, remaining - 1);
                            remaining -= len;    
                        }
                        
                        parameter_length = strlen(parameter);
                        
                        // prints from server-side
                        snprintf(buffer, sizeof(buffer), "%d,%d,%d,%zu,%s", getpid(), system_call, parameter_number, parameter_length, parameter);
                        write(writefd, buffer, strlen(buffer));
                        
                        memset(buffer, 0, sizeof(buffer));
                        ssize_t n;
                        do {
                            n = read(readfd, buffer, MAX);
                            usleep(10000); } 
                        while (n <= 0);
                        buffer[n] = '\0';
                        printf("Sent to server. %s\n", buffer);
                        break; }
                            
                    // recall
                    case 5: {
                        parameter_number = 0;
                        memset(parameter, 0, sizeof(parameter));

                        // prints from server-side
                        snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d,%s", getpid(), system_call, parameter_number, 0, parameter);
                        write(writefd, buffer, strlen(buffer));
                            
                        ssize_t n;
                        do {
                            n = read(readfd, buffer, MAX);
                            usleep(10000); }
                        while (n <= 0);
                        buffer[n] = '\0';
                        
                        printf("Stored parameters: %s\n", buffer);
                        break; }
                    
                    // wait mutex 
                    case 6: 
                        printf("wait mutex client\n");
                        parameter_number = 0;
                        
                        // prints from server-side
                        snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d,%s", getpid(), system_call, parameter_number, 0, parameter);
                        write(writefd, buffer, strlen(buffer));
                        break;
                        
                    // signal mutex 
                    case 7: 
                        printf("signal mutex client\n");
                        parameter_number = 0;
                        
                        // prints from server-side
                        snprintf(buffer, sizeof(buffer), "%d,%d,%d,%d,%s", getpid(), system_call, parameter_number, 0, parameter);
                        write(writefd, buffer, strlen(buffer));
                        break;
                        
                    default: {
                        printf("Invalid System Call. Please try again.\n");
                        break; }
                } // END switch
                
            } // END while #2
            
        } // END else-if selection == 1
        else { printf("Invalid selection o(>.<)o. Please try again!\n"); }
        
    } // END while #1
    
    close(readfd);
    printf("unlinking FIFO: %s\n", client_FIFO1);
    unlink(client_FIFO1);
} // END client()
