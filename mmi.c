/**********************************************************************
 *Copyright (C) 2015 Marcus Pedersén
 *
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program. If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

/*
 * The program shows information about a user
 */

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

int main(int argc, char *argv[])
{
  char *prog, *uname, **grmem, hname[HOST_NAME_MAX+1];
  struct passwd *user;
  struct group *grp, *groups;

  prog = argv[0];
  
  if(argc == 1){
    if((uname = getlogin()) == NULL){
      fprintf(stderr, "%s: Can not get username\n", prog);
      exit(1);
    }
  }
  else if(argc == 2)
    uname = argv[1];
  else{
    fprintf(stderr, "%s: Takes zero or one argument\n", prog);
    exit(1);
  }

  if((user=getpwnam(uname)) == NULL){
    fprintf(stderr, "%s: Can not get user information\n", prog);
    exit(1);
  }

  if((grp=getgrgid(user->pw_gid)) == NULL){
    fprintf(stderr, "%s: Can not get group information\n", prog);
    exit(1);
  }

  if(gethostname(hname, HOST_NAME_MAX+1)){
    fprintf(stderr, "%s: Can not get hostname\n", prog);
    exit(2);
  }

  printf("On host:          %s\n", hname);
  printf("Username:         %s\n", user->pw_name);
  printf("User ID:          %d\n\n", user->pw_uid);
  printf("Groupname:        %s\n", grp->gr_name);
  printf("Group ID:         %d\n", user->pw_gid);
  printf("Members of group:\n");

  grmem = grp->gr_mem;
  while(*grmem)
    printf("                  %s\n", *grmem++);

  printf("User is member of groups:\n");

  while((groups = getgrent())){
    grmem = groups->gr_mem;
    while(*grmem){
      if(!strcmp(*grmem++, user->pw_name)){
	printf("                  %d:%s\n", groups->gr_gid, groups->gr_name);
	break;
      }
    }
  }

  printf("\n");

  printf("Home directory:   %s\n", user->pw_dir);
  printf("Shell:            %s\n", user->pw_shell);

  exit(0);
}
