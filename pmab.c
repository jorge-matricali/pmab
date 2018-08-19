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

#include "discovery.h"
#include "iprange.h"
#include "work_list.h"
#include "pmab.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h> /* sleep */

int main(int argc, char **argv) {
    char *netmask_s;

    if (argc < 2) {
        perror("usage: %s TARGET");
        exit(EXIT_FAILURE);
    }

    netmask_s = strchr(argv[1], '/');

    if (netmask_s == NULL) {
        printf("No me pasaste un rango :(\n");
    } else {
        printf("Me pasaste un rango :D\n");
        discover_webservers(argv[1]);
    }

    int cantidad = work_list_count(&work_ips);
    printf("Cantidad de trabajo en la cola: %d\n", cantidad);

    if (cantidad == 0) {
        printf("Sin trabajo... saliendo...\n");
        exit(EXIT_SUCCESS);
    }

    int cant_th = 20;
    pthread_t thread_discover_webservers[cant_th];
    // pthread_t thread_discover_pma_installations;

    /* create a thread which performs webservers discovery */
    int t;
    for (t = 0; t < cant_th; t++) {
        if (pthread_create(&thread_discover_webservers[t], NULL, &discover_webserver, (void *) &work_ips)) {
            fprintf(stderr, "Error creating thread\n");
            return 1;
        }
    }

    for (int j = 0; j < t; j++) {
        /* wait for the second thread to finish */
        if (pthread_join(thread_discover_webservers[j], NULL)) {
            fprintf(stderr, "Error joining thread\n");
            return 2;
        }
    }

    printf("Todos los thread su murieron... saliendo...\n");
    exit(EXIT_SUCCESS);

    //    discover_webserver("localhost");
    //    discover_pma_installation("localhost", 8000);
}