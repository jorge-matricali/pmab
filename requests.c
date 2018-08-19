/*
 * Copyright (C) 2018 Jorge Matricali <jorgematricali@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdbool.h>
#include <stdio.h> /* printf, sprintf */
#include <stdlib.h> /* exit */
#include <unistd.h> /* read, write, close */
#include <string.h> /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */
#include <errno.h>

#include "logger.h"

/**
 * Verifica si existe el recurso HTTP
 * @param server
 * @param port
 * @param path
 * @return
 */
int resource_exists(struct hostent *server, int port, const char* path) {
    char *message_fmt = "HEAD %s HTTP/1.0\r\n\r\n";
    int sockfd;
    int bytes;
    int sent;
    int received;
    int total;
    struct sockaddr_in serv_addr;
    char message[1024];
    char response[4096];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        print_error("ERROR opening socket.");
        return errno;
    }

    memset(&serv_addr, 0, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) {
        print_error("ERROR connecting.");
        return -2;
    }

    memset(message, 0, sizeof (message));
    snprintf(message, sizeof message, message_fmt, path);

    /* send the request */
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0) {
            print_error("ERROR writing message to socket.");
            return -3;
        }
        if (bytes == 0) {
            break;
        }
        sent += bytes;
    } while (sent < total);

    /* receive the response */
    memset(response, 0, sizeof (response));
    total = sizeof (response) - 1;
    received = 0;
    do {
        bytes = read(sockfd, response + received, total - received);
        if (bytes < 0) {
            print_error("ERROR reading response from socket.");
            return -4;
        }
        if (bytes == 0) {
            break;
        }
        received += bytes;
    } while (received < total);

    if (received == total) {
        print_error("ERROR storing complete response from socket.");
        return -5;
    }

    /* close the socket */
    close(sockfd);

    /* process response */
    if (strstr(response, "200 OK") != NULL) {
        return 1;
    }

    return 0;
}
