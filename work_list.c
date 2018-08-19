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

#include "work_list.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Create a new node
 * initialize the data and next field
 * @param data
 * @param next
 * @return the newly created node
 */
work_list_node_t* work_list_create(char* data, work_list_node_t* next) {
    work_list_node_t* new_node = (work_list_node_t*) malloc(sizeof (work_list_node_t));
    if (new_node == NULL) {
        perror("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->status = PENDING;
    new_node->next = next;

    return new_node;
}

/**
 * Add a new node at the end of the list
 * @param head
 * @param data
 * @return 
 */
work_list_node_t* work_list_append(work_list_node_t* head, char* data) {
    if (head == NULL) {
        return NULL;
    }
    
    /* go to the last node */
    work_list_node_t *cursor = head;
    while (cursor->next != NULL) {
        cursor = cursor->next;
    }

    /* create a new node */
    work_list_node_t* new_node = work_list_create(data, NULL);
    cursor->next = new_node;

    return head;
}