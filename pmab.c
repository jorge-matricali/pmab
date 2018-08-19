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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    //    discover_webserver("localhost");
    //    discover_pma_installation("localhost", 8000);
}