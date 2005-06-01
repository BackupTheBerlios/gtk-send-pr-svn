/*
Copyright (c) 2003, Miguel Mendez. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer. 
	* Redistributions in binary form must reproduce the above copyright notice,
	this list of conditions and the following disclaimer in the documentation 
	and/or other materials provided with the distribution. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

$Id$

*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pwd.h>

#include "gtk-send-pr.h"
#include "user_settings.h"

void 
save_settings(USER_PROFILE *my_profile)
{

	char *homedir;
	char temp[1024];
	
	char temp_name[1024];
	char temp_email[1024];
	char temp_org[1024];
	char temp_smtp[1024];
	char temp_geometry[1024];
	
	int i,j;
	FILE *f;
	
	struct stat sb;
	
	homedir=getenv("HOME");
	if(homedir==NULL) return;
	snprintf(temp,1024,"%s/%s",homedir,".gtk-send-pr");
	
	i=stat(temp, &sb);
	if(i==-1) {
	
		j=mkdir(temp,0700);
	
	} else if((sb.st_mode & S_IFDIR)!=S_IFDIR){
	
		j=unlink(temp);
		if(j!=-1) {
		
			j=mkdir(temp,0700);
		
		}
	
	if(j==-1) return;
	
	}
	
	snprintf(temp_name,1024,"%s/name",temp);
	snprintf(temp_email,1024,"%s/email",temp);
	snprintf(temp_org,1024,"%s/organization",temp);
	snprintf(temp_smtp,1024,"%s/smtp",temp);
	snprintf(temp_geometry,1024,"%s/geometry",temp);
	
	
	f=fopen(temp_name,"w");
	if(f==NULL) return;
	fprintf(f,my_profile->name);
	fclose(f);
	
	f=fopen(temp_email,"w");
	if(f==NULL) return;
	fprintf(f,my_profile->email);
	fclose(f);
	
	
	f=fopen(temp_org,"w");
	if(f==NULL) return;
	fprintf(f,my_profile->org);
	fclose(f);
	
	f=fopen(temp_smtp,"w");
	if(f==NULL) return;
	fprintf(f,my_profile->smtp);
	fclose(f);
	
	f=fopen(temp_geometry,"w");
	if(f==NULL) return;
	fprintf(f,"%i,%i",my_profile->geom_x,my_profile->geom_y);
	fclose(f);
	
	return;

}

void 
load_settings(USER_PROFILE *my_profile)
{
	int i;
	struct passwd *pr_user;
	struct utsname pr_uname;
	char *homedir;
	char temp[1024];
	char temp_name[1024];
	char temp_email[1024];
	char temp_org[1024];
	char temp_smtp[1024];
	char temp_geometry[1024];
	int fd;
	FILE *f;
	struct stat sb;
	
		
	i=uname(&pr_uname);
		if(i==-1) {
			perror("uname()");
			exit(EXIT_FAILURE);
		}

	/* Set the size */

	
	homedir=getenv("HOME");
	assert(homedir!=NULL);
	
	snprintf(temp,1024,"%s/%s",homedir,".gtk-send-pr");
	i=stat(temp, &sb);
	if(i==-1) {	
		
		snprintf(temp,1024,"%s/%s",homedir,".gtk-send-pr-rc");
		fd=open(temp,O_RDONLY,0);
		if(fd!=-1) {
		read(fd,my_profile,sizeof(USER_PROFILE));
		close(fd);
		unlink(temp);
		
		} else  {
		
			/* Let's see who am I */
			pr_user=getpwuid(getuid());

			my_profile->geom_x=400;
			my_profile->geom_y=480;
			strncpy(my_profile->name,pr_user->pw_gecos,255);
			strncpy(my_profile->email,pr_user->pw_name,255);
			sprintf(my_profile->org," ");
			sprintf(my_profile->smtp,"FILL THIS!!!");
		}
		
	} else {
	
		
	snprintf(temp_name,1024,"%s/name",temp);
	snprintf(temp_email,1024,"%s/email",temp);
	snprintf(temp_org,1024,"%s/organization",temp);
	snprintf(temp_smtp,1024,"%s/smtp",temp);
	snprintf(temp_geometry,1024,"%s/geometry",temp);
	
	fd=open(temp_name,O_RDONLY,0);
	if(fd==-1) return;
	read(fd,my_profile->name,sizeof(my_profile->name));
	close(fd);
	
	fd=open(temp_email,O_RDONLY,0);
	if(fd==-1) return;
	read(fd,my_profile->email,sizeof(my_profile->email));
	close(fd);
	
	
	fd=open(temp_org,O_RDONLY,0);
	if(fd==-1) return;
	read(fd,my_profile->org,sizeof(my_profile->org));
	close(fd);
	
	fd=open(temp_smtp,O_RDONLY,0);
	if(fd==-1) return;
	read(fd,my_profile->smtp,sizeof(my_profile->smtp));
	close(fd);
	
	f=fopen(temp_geometry,"r");
	if(f==NULL) return;
	fscanf(f,"%i,%i",&my_profile->geom_x,&my_profile->geom_y);
	fclose(f);	

	}
	
	return;
	
}
