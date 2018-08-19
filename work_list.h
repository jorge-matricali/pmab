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
 * File:   work_list.h
 * Author: Jorge Matricali <jorgematricali@gmail.com>
 *
 * Created on 19 de agosto de 2018, 02:19
 */

#ifndef WORK_LIST_H
#define WORK_LIST_H

enum work_status {
    PENDING,
    ACTIVE,
    COMPLETED
};

typedef struct work_list_node {
    char* data;
    enum work_status status;
    struct work_list_node* next;
} work_list_node_t;

typedef void (*callback)(work_list_node_t* data);

/**
 * Create a new node
 * initialize the data and next field
 * @param data
 * @param next
 * @return the newly created node
 */
work_list_node_t* work_list_create(char* data, work_list_node_t* next);

/**
 * Add a new node at the end of the list
 * @param head
 * @param data
 * @return 
 */
work_list_node_t* work_list_append(work_list_node_t* head, char* data);

#endif /* WORK_LIST_H */

