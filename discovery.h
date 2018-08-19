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

/* 
 * File:   discovery.h
 * Author: Jorge Matricali <jorgematricali@gmail.com>
 *
 * Created on 18 de agosto de 2018, 19:25
 */

#ifndef DISCOVERY_H
#define DISCOVERY_H

#include <arpa/inet.h> /* in_addr_t */

int discover_pma_installation(const char* host, int port);
void discover_webservers_range(const char *range);
void *discover_webserver(void *node_void_ptr);
void *discover_pma_installations(void *node_void_ptr);
int port_check(struct in_addr in, int port);
extern const char * const test_resources[85];

#endif /* DISCOVERY_H */

