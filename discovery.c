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

#include "requests.h" /* resource_exists */

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
        perror("ERROR, no such host");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 85; i++) {
        ret = resource_exists(server, port, test_resources[i]);
        if (ret > 0) {
            printf("%s%s OK\n", host, test_resources[i]);
            continue;
        }
        if (ret == 0) {
            printf("%s%s NOT FOUND\n", host, test_resources[i]);
            continue;
        }
        printf("%s%s FAIL\n", host, test_resources[i]);
    }
    
    return 0;
}
