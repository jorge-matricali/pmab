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
 * File:   pmab.h
 * Author: Jorge Matricali <jorgematricali@gmail.com>
 *
 * Created on 18 de agosto de 2018, 23:05
 */

#ifndef PMAB_H
#define PMAB_H

#include "work_list.h"

extern work_list_node_t g_work_ips;
extern work_list_node_t g_work_webservers;
extern int g_num_work_list_ips;

int main(int argc, char** argv);

#endif /* PMAB_H */

