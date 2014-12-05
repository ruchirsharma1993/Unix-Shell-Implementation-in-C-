#include<stdio.h>
#include<string>
#include <unistd.h> 
#include<ctype.h>
#include<vector>
#include<string.h>
#include<fstream>
#include<iostream>
#include<stdlib.h>
#include <fcntl.h>
#include  <sys/types.h>
#include <sys/wait.h>

using namespace std;
char gpwd()
{
	char buf[1000];
	//Its pwd
	if (getcwd(buf, sizeof(buf)) != NULL)
		printf("%s",buf);
	else
		perror("Error:");
		
}
void phist()
{
	//Function to print the history
	printf("in history");
	ifstream in ("history.txt");
	int n=1;
		
	string s;   
	if (in.is_open())
	{
		while (! in.eof() )
		{
			getline (in,s);
			cout<<n<<". "<<s<<"\n";
			n++;
			
		}

		in.close();
	}
}
void writehist(char arr[])
{
	//Function to write the current command into the history file.
	//This function checks for 2000 entries, replaces the first entry if no of lines = 2000 else appends
	
	ifstream in ("history.txt");
	vector<string> v;
	int ch,n=0;
		
	string s;   
	if (in.is_open())
	{
		while (! in.eof() )
		{
			getline (in,s);
			//cout<<"\nRead: "<<s;
	
			v.push_back(s);
			n++;
		}

		in.close();
	}
	
	//printf("n is : %d",n);
	v.push_back(arr);
	
	if(n>=15) //THRESHOLD VALUE
	{
		//n is equal to threshold
		//cout<<"Printing";
		ofstream out("temp.txt",std::fstream::out | std::fstream::app);

		if (out.is_open())
		{
			
			for(int i=1;i<n;i++)
			{
			
				out<<v[i];
				out<<"\n";
		 	}	
		 	//Push Final Element
	 		out << v[n];
	 	
		 	out.close();
		}
		else
		{
			perror("\nFile not opened");
		}
	 	
    		remove("history.txt");
            	rename( "temp.txt","history.txt");
	}
	else
	{
		//printf("\nAppending..");
		ofstream out("history.txt",std::fstream::out | std::fstream::app);

		if (out.is_open())
		{
			out << v[n];
			out << "\n";
			out.close();
		}
	}
	
	
}
int check(char c)
{
	//This functions takes a char as input and checks whether its a digit or +/-. Returns 1 if it is else return -1
	switch(c)
	{
		case '1': return 1;
			break;
		case '2': return 1;
			break;
		case '3': return 1;
			break;
		case '4': return 1;
			break;
		case '5': return 1;
			break;
		case '6': return 1;
			break;
		case '7': return 1;
			break;
		case '8': return 1;
			break;
		case '9': return 1;
			break;
		case '-': return 1;
			break;
		case '+': return 1;
			break;
		case '0': return 1;
			break;
	}
	return -1;
}
int process(char arr[])
{
	
	    // 	printf("in Process: %s",arr);
	     	char ext[100];
	     	strcpy(ext,arr);
		//arr contains the command name, check for available options
			
		//Internal Commands
		if(strcmp(arr,"pwd")==0) //Internal command pwd
		{
			gpwd();
			return 0;
		}
		if(strcmp(arr,"history")==0) //only history
		{
			phist();
			return 0;
		}
		
		//Setting Envt Variable
		if(strstr(arr,"=")!=NULL)
		{
			//It contains =, so set environment variable
			//printf("Inside the function, arr is %s",arr);
			if((strstr(arr,"= ")!=NULL)||(strstr(arr," =")!=NULL))
			{
				//It contains spaces, so envt variable cant be initiaized
				char *start = strtok(arr," ");
				printf("\n%s: Command not found",start);
				return 0;
			}
			
			//Else Initialize envt variable
			char *name = strtok(arr,"=");
			char *val = strtok(NULL,"=");
			//printf("name is: %s val is: %s",name,val);
			int ret = setenv(name,val,1);
			if(ret==-1)
				perror("Error: ");
			return 0;
		}
		
		//Tokenize the String to check cd
		char *start = strtok(arr," ");
		//printf("Start is: %s",start);
		
		if(strcmp(start,"cd")==0)
		{
			//cd
			//printf("\nDetected cd");
			char *path=strtok(NULL," ");
			//printf("\nPath is: %s",path);
			int ret=chdir(path);
			if(ret==-1)
				perror("Error");
			return 0;
		}
		
		//Check for echo
		if(strcmp(start,"echo")==0)
		{
			//Print Envt Variable
			char *name=strtok(NULL," ");
			//printf("\nName is: %s",name);
			if(name[0]=='$')
			{
				name=name+1;
				char *val = getenv(name);
				//printf("\nVal is : %s",val);
				printf("\n%s",val);
			}
			else
			{
				printf("\n");
				while(name!=NULL)
				{
					printf("%s ",name);
					name = strtok(NULL," ");
				}
			}
			return 0;
		}
		//Check for History, !
		if(strcmp(start,"history")==0)
		{
			//History
			//printf("\nDetected history");
			phist();
			return 0;
		}
		if(arr[0]=='!')
		{
			//Hashbang
			//printf("\nDetected Hashbang");
			/*
				!-1
				  !10 or !+10
				  !<string>
			*/
			
			//Read contents of history.txt into an array
			
			ifstream in ("history.txt");
			vector<string> v;
			int ch,n=0;
		
			string s;   
			if (in.is_open())
			{
				while (! in.eof() )
				{
					getline (in,s);
					//cout<<"\nRead: "<<s;
	
					v.push_back(s);
					n++;
				}

				in.close();
			}
			if(arr[1]=='!')
			{
				//Last Command to be reexecuted
				
				string cmd = v[n-1];
				
				char next[100];
				strcpy(next, cmd.c_str());
				
				//printf("\n%s\n",next);
				
				//Write in history.txt
				writehist(next); 
				
				if(strcmp(next,"exit")==0)
					return -1;
					
				//Call process recursively for the command
				process(next);
				return 0;
			}
			int ret=check(arr[1]);
			if(ret==1)
			{
				//Digit
				arr=arr+1;
				int num;
				sscanf(arr, "%d", &num); //Converts char array to int.. :D
				//printf("num is: %d",num);
				if((num>n)||(num+n<0)||(num==0))
				{
					printf("!%d: Event not found",num);
					return 0;
				}
				if(num<0)
				{
					char next[100];
					string cmd =v[n+num];
					strcpy(next, cmd.c_str());
				
					printf("\n%s\n",next);
				
					if(strcmp(next,"exit")==0)
						return -1;
						
					process(next);
					return 0;
				}
				if(num>0)
				{
					char next[100];
					string cmd =v[num];
					strcpy(next, cmd.c_str());
				
					printf("\n%s\n",next);
				
					if(strcmp(next,"exit")==0)
						return -1;
						
					process(next);
					return 0;
				
					
				}
				
			}
			else
			{
				//String
				arr=arr+1;
				int len = strlen(arr);
				//printf("\n%s is of length %d",arr,len);
				for(int j=n-1;j>0;j--)
				{
					string cmd = v[j];
					char next[100];
					strcpy(next, cmd.c_str());
					int len1=strlen(next); //Command to be checked
					//printf("\nChecking %s",next);
					if(len1<len)
					{
						//printf("\nSkip: %s",next);
					}
					else if(len1==len)
					{
						if(strcmp(next,arr)==0)
						{
							if(strcmp(next,"exit")==0)
								return -1;
						
							process(next);
							return 0;
				
						}
					}
					else
					{
						//Check for substring
						//printf("Checking for substring");
						int sflag=0;
						for(int k=0;k<len;k++)
						{
							if(next[k]!=arr[k])
							{
								//printf("\t%c != %c",next[k],arr[k]);
								sflag=1;
								break;
							}
						}
						if(sflag==0)
						{
						
							
							if(strcmp(next,"exit")==0)
								return -1;
						
							process(next);
							return 0;
					
						}
						
					}
				}
				
					printf("%s Event Not Found",arr);
					return 0; 
			}
		}
		
		//Finally Execute the external command here:
		char *args[60];
    		char **next = args;
		//printf("\next is: %s",ext);
		char *temp = strtok(ext, " ");
		while (temp != NULL)
		{
			*next++ = temp;
			printf("%s\n", temp);
			temp = strtok(NULL, " ");
		}
		*next = NULL;
	    

	   //     printf("Its an External Command");
		pid_t pid;
		int s; //To store the status

		if ((pid = fork()) < 0) 
		{
			perror("Error in Forking ");
		}

		else if (pid == 0)  //Child Process Execute
		{
			if (execvp(args[0],args) < 0) 
			{
				perror("Error in Execution");
				
			}
			exit(0);

		}
		else
		{  
			while (wait(&s) != pid);
			
		}
		return 0;
}

int startprocess(char arr[], int i)
{
	   	
	   	//printf("\n\nIn startprocess");
	   	//Handling Redirection
	     	int finput=0,fout=0,ferr=0,app=0;
	     	
		int s_in,s_out,s_err;
	     	char cpy[100];
	     	char *ifile,*efile,*ofile;
	     	strcpy(cpy,arr);
	     	
	     	if(strstr(arr,"<")!=NULL)
	     	{
	     		//Input to be redirected
	     		finput=1;
	     		char *next = strtok(cpy," ");
	     		while(next!=NULL)
	     		{
	     			//printf("\nNext is: %s",next);
	     			if(next[0]=='<')
	     			{
	     				while(next[1]==' ')
	     				{
	     					next=next+1;
	     				}
	     				
	     				char *pr=next+1;
	     				ifile = strtok(pr," ");
	     				printf("\nifile is : %s",ifile);
	     				break;
	     			}
	     			
	     			next = strtok(NULL," ");
	     		}
	     		strcpy(cpy,arr);
	     	
	     	}
	     	
	     	if(strstr(arr,">")!=NULL)
	     	{
	     		//Output to be redirected
	     		fout=1;
	     		char *next = strtok(cpy," ");
	     		while(next!=NULL)
	     		{
	     			if(next[0]=='>')
	     			{
	     				if(next[1]=='>')
	     				{
		     				char *pr=next+2;
		     				ofile = strtok(pr," ");
		    				//printf("\nofile is : %s",ofile);
		  				app=1; //To detect Append Mode >>
	     				}
	     				else
	     				{
	     					char *pr=next+1;
		     				ofile = strtok(pr," ");
		     				//printf("\nofile is : %s",ofile);
		     				
	     				}
	     					break;
	     			}
	     			next = strtok(NULL," ");
	     		}
	     		
	     		
	     		strcpy(cpy,arr);
	     	
	     	}
	     	if(strstr(arr,"2>")!=NULL)
	     	{
	     		//Error to be redirected
	     		ferr=1;
	     		char *next = strtok(cpy," ");
	     		while(next!=NULL)
	     		{
	     			if(next[0]=='2')
	     			{
	     				char *pr=next+2;
		     			efile = strtok(pr," ");
		     			//printf("\nefile is : %s",efile);
		     				break;
	     			}
	     			next = strtok(NULL," ");
	     		}
	     		strcpy(cpy,arr);
	     		
	     	}
		
		//printf("Processing cmd...");
		//Process and extract the command out of the string
		char cmd[100] = "";
		char *tok = strtok(cpy," ");
		while(tok!=NULL)
		{
			if((strstr(tok,">")==NULL)&&(strstr(tok,"<")==NULL)&&(strstr(tok,"2>")==NULL))
			{	strcat(cmd,tok);
				strcat(cmd," ");
			}
			tok  = strtok(NULL," ");
		}
		
		//printf("\n\n\ncmd is: %s",cmd);
		
		//printf("Input file: %s, O File: %s, E File: %s",ifile,ofile,efile);
		int in,out,err;
		if(finput==1)
		{
			in =  open(ifile, O_RDONLY);
			if(in<0)
			{
				perror("Error in opening: ");
			}
			else
			{
				s_in = dup(0);
				close(0);
				dup2(in,0);
			}
		}
		if(fout==1)
		{
			if(app==1)
				 out = open(ofile, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			else
				 out = open(ofile, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			s_out=dup(1);
			close(1);
			dup2(out,1);
		}
		if(ferr==1)
		{
			err = open(efile, O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			s_err=dup(2);
			close(2);
			dup2(err,2);
		}
		
		int ret = process(cmd+i);
		
		
		if(ret==-1) //To handle !! and history exit's
		{
			printf("breaking");
				return -1;
		}
		
		//Restore redirections before next command
		if(finput==1)
		{
			close(in);
			dup2(s_in,0);
		}
		if(fout==1)
		{
			close(out);
			dup2(s_out,1);
		}
		if(ferr==1)
		{
			close(err);
			dup2(s_err,2);
		}
		return 0;
}

int pipework(int in, int out, char arr[],int i)
{
pid_t pid;
int s;

  if ((pid = fork ()) == 0)
    {
      if (in != 0)
        {
          dup2 (in, 0);
          close (in);
        }

      if (out != 1)
        {
          dup2 (out, 1);
          close (out);
        }

    
		startprocess(arr,i);
	}
	
	return pid;

}

int main()
{
	
	char arr[50];
	char *p;

	
	
	//Read the user Input..!!
	while(1)
	{
		printf("\nMy_Shell:");
		gpwd();
		printf("$ ");
		fgets(arr, sizeof(arr), stdin);
		
		//Replace \n by \0 so as to properly mark the EOI
		if ((p = strchr(arr, '\n')) != NULL)
			*p = '\0';
		
		int i=0;
	     	//Remove Leading spaces..
		for (i = 0; isspace(arr[i]); ++i) ;
		
		//Write into history file
		
		if(strcmp(arr+i,"")==0)
		{
			//NULL CASE
			continue;
		}
		
		
		if(strstr(arr,"!")==NULL)             // If entered string not null, then write into history..!!
			writehist(arr+i); 
			
		 
		
		if(strcmp(arr+i,"exit")==0) //+i to handle the leading spaces case
	     	{
	     		//Exiting
	     		printf("bye..\n");
	     		break;
	     	}	
	     	
	     	//Handle Pipelines..!!
	     	if(strstr(arr,"|")!=NULL)
	     	{
	     	
	     		//printf("\nShit it contains pipes.. :/");
	     		char cpy[50],cpy1[50];
	     		strcpy(cpy,arr);
	     		strcpy(cpy1,arr);
	     		
	     		int ccount=0;
	     		int len = strlen(arr);
	     		int v;
	     		for(v=0;v<len;v++)
	     		{
	     			if(arr[v]=='|')
	     				ccount=1+ccount;
	     		}	     		
	     		
	     		ccount++;
	     		char *saved;
	     		char *next = strtok_r(arr,"|",&saved);
	     		pid_t pid;
  			int in, fd[2],u;
	     		int cur_in = dup(0);
	     		int cur_out = dup(1);
	     		in = cur_in;
	     	//	printf("first next is : %s",next);
	     		
	     		for(u=0;u<ccount-1;u++)
	     		{
	     		//	printf("\n\nProcessing: %s",next);
	     			
	     				char *args[60];
			    		char **ext = args;
					char copy[100];
					strcpy(copy,next);
					
					char *temp = strtok(next, " ");
					while (temp != NULL)
					{
						*ext++ = temp;
						//printf("%s\n", temp);
						temp = strtok(NULL, " ");
					}
					*ext = NULL;
		     		
		     			pipe(fd);
		     				
				  	if(fork()==0)
					{
						 dup2(fd[1],1); 
						 execvp(args[0],args);
					}
					
					dup2(fd[0],0);
					close(fd[1]);
		     			
	     			next = strtok_r(NULL,"|",&saved);
	     			
	     		}
	     		//next = strtok(NULL,"|");
	     		//printf("\nlast next is: %s",next);
	     		
	     	
	     			char *args[60];
		    		char **ext = args;
		    		char copy[100];
					strcpy(copy,next);
				char *temp = strtok(next, " ");
				while (temp != NULL)
				{
					*ext++ = temp;
					printf("%s\n", temp);
					temp = strtok(NULL, " ");
				}
				*ext = NULL;
				int s;
				wait(NULL);
		     		
		     		if( (pid = fork())==0)
		     			execvp(args[0],args);	
		     		else
				{  
					while (wait(&s) != pid);
					return 0;
				}
	     	}	
	     	else
	     	{
	     	
	     	
		     	int ret = startprocess(arr,i);
		     	if(ret==-1)
		     		break;
		 }
	     	
	     	
	     	
	     	
	     	
	     	
	     	
	     	
	     	
	}
}
