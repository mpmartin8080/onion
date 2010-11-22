/*
	Onion HTTP server library
	Copyright (C) 2010 David Moreno Montero

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as
	published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <onion/onion.h>
#include <onion/onion_handler.h>
#include <handlers/onion_handler_directory.h>
#include <handlers/onion_handler_static.h>
#include <onion/onion_log.h>

onion *o=NULL;

void free_onion(){
	fprintf(stderr,"\nClosing connections.\n");
	onion_free(o);
	exit(0);
}

int main(int argc, char **argv){
	int port=8080;
	const char *dirname=".";
	int i;
	for (i=1;i<argc;i++){
		if (strcmp(argv[i],"-p")==0){
			port=atoi(argv[++i]);
		}
		else
			dirname=argv[i];
	}

	
	onion_handler *dir=onion_handler_directory(argc==2 ? argv[1] : ".");
	onion_handler_add(dir, onion_handler_static(NULL,"<h1>404 - File not found.</h1>", 404) );
	
	o=onion_new(O_THREADED|O_DETACH_LISTEN);
	onion_set_root_handler(o, dir);
	
	
	onion_set_port(o, port);
	
	signal(SIGINT, free_onion);
	int error=onion_listen(o);
	if (error){
		perror("Cant create the server");
	}
	fprintf(stderr,"Detached. Now sleep 100.\n");
	for (i=0;i<100;i++){
		ONION_INFO("Exiting in %d seconds.",100-i);
		sleep(1);
	}
	
	onion_free(o);
	
	return 0;
}
