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
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h> /* sleep */

int main(int argc, char **argv) {
    char *netmask_s;

    if (argc < 2) {
        print_error("usage: %s TARGET");
        exit(EXIT_FAILURE);
    }

    netmask_s = strchr(argv[1], '/');

    if (netmask_s == NULL) {
        print_error("No me pasaste un rango :(");
    } else {
        print_debug("Me pasaste un rango :D");
        discover_webservers_range(argv[1]);
    }

    int cantidad = work_list_count(&g_work_ips);
    print_debug("Cantidad de trabajo en la cola: %d", cantidad);

    if (cantidad == 0) {
        print_debug("Sin trabajo... saliendo...");
        exit(EXIT_SUCCESS);
    }

    int cant_th = 20;
    pthread_t thread_discover_webservers[cant_th];
    pthread_t thread_discover_pma_installations[cant_th];
    g_num_work_list_ips = 0;

    /* create threads that will perform webservers discovery */
    int t;
    for (t = 0; t < cant_th; t++) {
        if (pthread_create(&thread_discover_webservers[t], NULL, &discover_webserver, (void *) &g_work_ips)) {
            print_error("Error creating thread.");
            return 1;
        }
        g_num_work_list_ips++;
    }

    /* create threads that will perform PMA installations discovery */
    int k;
    for (k = 0; k < cant_th; k++) {
        if (pthread_create(&thread_discover_pma_installations[k], NULL, &discover_pma_installations, (void *) &g_work_webservers)) {
            print_error("Error creating thread.");
            return 1;
        }
    }

    for (int j = 0; j < t; j++) {
        /* wait for servers discovery threads to finish */
        if (pthread_join(thread_discover_webservers[j], NULL)) {
            print_error("Error joining thread.");
            return 2;
        }
    }

    for (int j = 0; j < k; j++) {
        /* wait for PMA installations discovery threads to finish */
        if (pthread_join(thread_discover_pma_installations[j], NULL)) {
            print_error("Error joining thread.");
            return 2;
        }
    }

    print_debug("Work finished. Exiting...");
    exit(EXIT_SUCCESS);
}