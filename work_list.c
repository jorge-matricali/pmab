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
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        print_error("Error creating a new node.");
        exit(0);
    }
    if (!(new_node->data = malloc(sizeof (char) * (strlen(data) + 1)))) {
        print_error("Unable to allocate memory.");
        exit(EXIT_FAILURE);
    }
    strncpy(new_node->data, data, strlen(data));
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

/**
 * remove node from the front of list
 * @param head
 * @return 
 */
work_list_node_t* work_list_remove_front(work_list_node_t* head) {
    if (head == NULL) {
        return NULL;
    }
    work_list_node_t *front = head;
    head = head->next;
    front->next = NULL;

    /* is this the last node in the list */
    if (front == head) {
        head = NULL;
    }

    free(front);
    return head;
}

/**
 * Remove node from the back of the list
 * @param head
 * @return 
 */
work_list_node_t* work_list_remove_back(work_list_node_t* head) {
    if (head == NULL) {
        return NULL;
    }

    work_list_node_t *cursor = head;
    work_list_node_t *back = NULL;
    while (cursor->next != NULL) {
        back = cursor;
        cursor = cursor->next;
    }

    if (back != NULL) {
        back->next = NULL;
    }

    /* if this is the last node in the list */
    if (cursor == head) {
        head = NULL;
    }

    free(cursor);
    return head;
}

/**
 * Remove a node from the list
 * @param head
 * @param nd
 * @return 
 */
work_list_node_t* work_list_remove_any(work_list_node_t* head, work_list_node_t* nd) {
    if (nd == NULL) {
        return NULL;
    }

    /* if the node is the first node */
    if (nd == head) {
        return work_list_remove_front(head);
    }

    /* if the node is the last node */
    if (nd->next == NULL) {
        return work_list_remove_back(head);
    }

    /* if the node is in the middle */
    work_list_node_t* cursor = head;
    while (cursor != NULL) {
        if (cursor->next == nd) {
            break;
        }
        cursor = cursor->next;
    }

    if (cursor != NULL) {
        work_list_node_t* tmp = cursor->next;
        cursor->next = tmp->next;
        tmp->next = NULL;
        free(tmp);
    }

    return head;
}

/**
 * Return the number of elements in the list
 * @param head
 * @return 
 */
int work_list_count(work_list_node_t *head) {
    work_list_node_t *cursor = head;
    int c = 0;

    while (cursor != NULL) {
        c++;
        cursor = cursor->next;
    }

    return c;
}

/**
 * Pick pending taks
 * @param head
 * @return 
 */
work_list_node_t* work_list_search_pending(work_list_node_t* head) {
    work_list_node_t *cursor = head;
    while (cursor != NULL) {
        if (cursor->status == PENDING) {
            cursor->status = PICKED;
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
}