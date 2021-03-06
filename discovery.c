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

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h> /* struct hostent, gethostbyname */
#include <errno.h> /* errno */
#include <fcntl.h>
#include <unistd.h> /* close */
#include <string.h> /* memset, memcpy */

#include "pmab.h"
#include "requests.h" /* resource_exists */
#include "iprange.h"
#include "discovery.h"
#include "work_list.h"
#include "logger.h"

work_list_node_t g_work_ips;
work_list_node_t g_work_webservers;
int g_num_work_list_ips;

const char * const test_resources[85] = {
    "/phpmyAdmin/",
    "/phpmyadmin/",
    "/phpmyadmin2/",
    "/phpmyadmin3/",
    "/phpmyadmin4/",
    "/2phpmyadmin/",
    "/MyAdmin/",
    "/PMA/",
    "/PMA2011/",
    "/PMA2012/",
    "/admin/",
    "/admin/db/",
    "/admin/pMA/",
    "/admin/phpMyAdmin/",
    "/admin/phpmyadmin/",
    "/admin/sqladmin/",
    "/admin/sysadmin/",
    "/admin/web/",
    "/administrator/PMA/",
    "/administrator/admin/",
    "/administrator/db/",
    "/administrator/phpMyAdmin/",
    "/administrator/phpmyadmin/",
    "/administrator/pma/",
    "/administrator/web/",
    "/database/",
    "/db/",
    "/db/db-admin/",
    "/db/dbadmin/",
    "/db/dbweb/",
    "/db/myadmin/",
    "/db/phpMyAdmin-3/",
    "/db/phpMyAdmin/",
    "/db/phpMyAdmin3/",
    "/db/phpmyadmin/",
    "/db/phpmyadmin3/",
    "/db/webadmin/",
    "/db/webdb/",
    "/db/websql/",
    "/dbadmin/",
    "/myadmin/",
    "/mysql-admin/",
    "/mysql/",
    "/mysql/admin/",
    "/mysql/db/",
    "/mysql/dbadmin/",
    "/mysql/mysqlmanager/",
    "/mysql/pMA/",
    "/mysql/pma/",
    "/mysql/sqlmanager/",
    "/mysql/web/",
    "/mysqladmin/",
    "/mysqlmanager/",
    "/php-my-admin/",
    "/php-myadmin/",
    "/phpMyAdmin-3/",
    "/phpMyAdmin/",
    "/phpMyAdmin2/",
    "/phpMyAdmin3/",
    "/phpMyAdmin4/",
    "/phpMyadmin/",
    "/phpmanager/",
    "/phpmy-admin/",
    "/phpmy/",
    "/phppma/",
    "/pma/",
    "/pma2011/",
    "/pma2012/",
    "/program/",
    "/shopdb/",
    "/sql/myadmin/",
    "/sql/php-myadmin/",
    "/sql/phpMyAdmin/",
    "/sql/phpMyAdmin2/",
    "/sql/phpmanager/",
    "/sql/phpmy-admin/",
    "/sql/phpmyadmin2/",
    "/sql/sql-admin/",
    "/sql/sql/",
    "/sql/sqladmin/",
    "/sql/sqlweb/",
    "/sql/webadmin/",
    "/sql/webdb/",
    "/sql/websql/",
    "/sqlmanager/"
};

/**
 * Busca una instalacion de phpMyAdmin en el host dado
 * @param host
 * @param port
 * @return 
 */
int discover_pma_installation(const char *host, int port) {
    int ret;
    struct hostent *server;

    server = gethostbyname(host);
    if (server == NULL) {
        print_error("ERROR, no such host");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 85; i++) {
        ret = resource_exists(server, port, test_resources[i]);
        if (ret > 0) {
            print_debug("%s%s OK", host, test_resources[i]);
            continue;
        }
        if (ret == 0) {
            print_debug("%s%s NOT FOUND", host, test_resources[i]);
            continue;
        }
        print_debug("%s%s FAIL", host, test_resources[i]);
    }

    return 0;
}

/**
 * Scans for an open port
 * @param in
 * @param port
 * @return 
 */
int port_check(struct in_addr in, int port) {
    int sockfd;
    int rval;
    struct sockaddr_in servaddr;
    fd_set fdset;
    struct timeval tv;

    /* Create TCP socket */
    sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1) {
        print_error("ERROR opening socket");
        return errno;
    }
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    memset(&servaddr, 0, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr = in;

    rval = connect(sockfd, (struct sockaddr *) &servaddr, sizeof (servaddr));
    FD_ZERO(&fdset);
    FD_SET(sockfd, &fdset);
    tv.tv_sec = 1; /* 1 second timeout */
    tv.tv_usec = 0;

    if (select(sockfd + 1, NULL, &fdset, NULL, &tv) == 1) {
        int so_error;
        socklen_t len = sizeof so_error;

        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len);

        if (so_error == 0) {
            close(sockfd);
            return 1;
        }
    }

    close(sockfd);
    return 0;
}

/**
 * Discover open webservers in a given CIDR block
 * @param range
 */
void discover_webservers_range(const char *range) {
    struct in_addr in;
    in_addr_t lo, hi;
    network_addr_t netaddr;

    netaddr = str_to_netaddr(range);
    lo = netaddr.addr;
    hi = broadcast(netaddr.addr, netaddr.pfx);

    for (in_addr_t x = lo; x < hi; x++) {
        in.s_addr = htonl(x);
        work_list_append(&g_work_ips, inet_ntoa(in));
    }
}

void *discover_webserver(void *node_void_ptr) {
    work_list_node_t *node_ptr = (work_list_node_t *) node_void_ptr;
    work_list_node_t *node = work_list_search_pending(node_ptr->next);

    do {
        node->status = ACTIVE;

        int ret;
        int port = 80;
        struct in_addr in;

        inet_aton(node->data, &in);
        ret = port_check(in, port);

        if (ret) {
            /* Port open, appending to work list */
            work_list_append(&g_work_webservers, node->data);
        }

        node->status = COMPLETED;
    } while ((node = work_list_search_pending(node->next)));

    print_debug("Exiting webserver discovery thread...");
    g_num_work_list_ips--;
    return NULL;
}

void *discover_pma_installations(void *node_void_ptr) {
    work_list_node_t *node_ptr = (work_list_node_t *) node_void_ptr;
    work_list_node_t *node = NULL;

    for (;;) {
        node = work_list_search_pending(node_ptr->next);
        if (node == NULL) {
            if (g_num_work_list_ips < 1) {
                /* No work is pending and the search threads are finished */
                return NULL;
            }
            /* There are still threads looking for web servers. We'll keep waiting. */
            sleep(1);
        } else {
            print_debug("Starting resources search (%s).", node->data);
            node->status = ACTIVE;
            discover_pma_installation(node->data, 80);
            node->status = COMPLETED;
        }
    };

    return NULL;
}