// 
// tsh - A tiny shell program with job control
// 
// <Sam Feig safe9522>
//

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>

#include "globals.h"
#include "jobs.h"
#include "helper-routines.h"

//
// Needed global variable definitions
//

static char prompt[] = "tsh> ";
int verbose = 0;

//
// You need to implement the functions eval, builtin_cmd, do_bgfg,
// waitfg, sigchld_handler, sigstp_handler, sigint_handler
//
// The code below provides the "prototypes" for those functions
// so that earlier code can refer to them. You need to fill in the
// function bodies below.
// 

void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigtstp_handler(int sig);
void sigint_handler(int sig);

//
// main - The shell's main routine 
//
int main(int argc, char **argv) {
  int emit_prompt = 1; // emit prompt (default)

  //
  // Redirect stderr to stdout (so that driver will get all output
  // on the pipe connected to stdout)
  //
  dup2(1, 2);

  /* Parse the command line */
  char c;
  while ((c = getopt(argc, argv, "hvp")) != EOF) {
    switch (c) {
    case 'h':             // print help message
      usage();
      break;
    case 'v':             // emit additional diagnostic info
      verbose = 1;
      break;
    case 'p':             // don't print a prompt
      emit_prompt = 0;  // handy for automatic testing
      break;
    default:
      usage();
    }
  }

  //
  // Install the signal handlers
  //

  //
  // These are the ones you will need to implement
  //
  Signal(SIGINT,  sigint_handler);   // ctrl-c
  Signal(SIGTSTP, sigtstp_handler);  // ctrl-z
  Signal(SIGCHLD, sigchld_handler);  // Terminated or stopped child

  //
  // This one provides a clean way to kill the shell
  //
  Signal(SIGQUIT, sigquit_handler); 

  //
  // Initialize the job list
  //
  initjobs(jobs);

  //
  // Execute the shell's read/eval loop
  //
  for(;;) {
    //
    // Read command line
    //
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }

    char cmdline[MAXLINE];

    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) {
      app_error("fgets error");
    }
    //
    // End of file? (did user type ctrl-d?)
    //
    if (feof(stdin)) {
      fflush(stdout);
      exit(0);
    }

    //
    // Evaluate command line
    //
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
  } 

  exit(0); //control never reaches here
}
  
/////////////////////////////////////////////////////////////////////////////
//
// eval - Evaluate the command line that the user has just typed in
// 
// If the user has requested a built-in command (quit, jobs, bg or fg)
// then execute it immediately. Otherwise, fork a child process and
// run the job in the context of the child. If the job is running in
// the foreground, wait for it to terminate and then return.  Note:
// each child process must have a unique process group ID so that our
// background children don't receive SIGINT (SIGTSTP) from the kernel
// when we type ctrl-c (ctrl-z) at the keyboard.
//
sigset_t mask, prevmask; // Masks for signal blocking
void eval(char *cmdline) {
  /* Parse command line */
  //
  // The 'argv' vector is filled in by the parseline
  // routine below. It provides the arguments needed
  // for the execve() routine, which you'll need to
  // use below to launch a process.
  //
  char *argv[MAXARGS];



  pid_t pid; // Current process id from fork
  sigemptyset(&mask);
  sigaddset(&mask, SIGCHLD);
    
  //
  // The 'bg' variable is TRUE if the job should run
  // in background mode or FALSE if it should run in FG
  //
  int bg = parseline(cmdline, argv); 
  //extern char **environ; // Enviroment vars needed for execve

  if (argv[0] == NULL) {
    return;
  }
  if (!builtin_cmd(argv)) { //If not a built in command, fork()
    sigprocmask(SIG_BLOCK, &mask, &prevmask); //parent blocks child signal temperarily
      
//fork rerutns PID of child process to parrent and 0 to child
    if ((pid = fork()) < 0) { // Child runs user job
      printf("fork(): forking error\n"); //id has to be greater than 0 (-1 = error)
      return;
    }
  
    if (pid == 0) { //Child Process
      setpgid(0,0); //sets pid & pgid to pi of the process
      
      if (execvp(argv[0], argv) < 0) { //no need to pass environ with execvp
        printf("%s: Command not found \n", argv[0]); //first element musy be greater than 0
        exit(0);
      }
      
    } 
    else { 
      if (bg) {
        addjob(jobs, pid, BG, cmdline);   //If bg, add job as bg 
        sigprocmask(SIG_SETMASK, &prevmask, NULL); // Parent unblocks SIGCHLD 
        printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
      } 
      else {  // Parent waits for foreground job to terminate 
        addjob(jobs, pid, FG, cmdline);   // If !bg, add job as fg 
        sigprocmask(SIG_SETMASK, &prevmask, NULL); // Parent unblocks SIGCHLD 
        waitfg(pid);
      }
    }
  }
  return;
}


/////////////////////////////////////////////////////////////////////////////
//
// builtin_cmd - If the user has typed a built-in command then execute
// it immediately. The command name would be in argv[0] and
// is a C string. We've cast this to a C++ string type to simplify
// string comparisons; however, the do_bgfg routine will need 
// to use the argv array as well to look for a job number.
//
int builtin_cmd(char **argv) {
  string cmd(argv[0]);

  if (cmd == "quit") {
    for (int i = 0; i < MAXJOBS; i++) { //kill all jobs before exiting to prevent zombies
        if (jobs[i].pid != 0) {
//             printf("killing %d \n", jobs[i].pid);
            kill(-jobs[i].pid, SIGINT);
        }
    }
    exit(0);
  }
  else if (cmd == "jobs") { // Print jobs
    listjobs(jobs);
    return 1;
  }
  else if (cmd == "bg" || cmd == "fg") { // Switch proc to bg/fg
    do_bgfg(argv);
    return 1;
  }
  else if (cmd == "&") { // Ignore plain &
    return 1; 
  }

  return 0; // Not a builtin command
}

/////////////////////////////////////////////////////////////////////////////
//
// do_bgfg - Execute the builtin bg and fg commands
//
void do_bgfg(char **argv) {
  struct job_t *jobp = NULL;
    
  /* Ignore command if no argument */
  if (argv[1] == NULL) {
    printf("%s command requires PID or %%jobid argument\n", argv[0]);
    return;
  }
    
  /* Parse the required PID or %JID arg */
  if (isdigit(argv[1][0])) {
    pid_t pid = atoi(argv[1]);
    if (!(jobp = getjobpid(jobs, pid))) {
      printf("(%d): No such process\n", pid);
      return;
    }
  }
  else if (argv[1][0] == '%') {
    int jid = atoi(&argv[1][1]);
    if (!(jobp = getjobjid(jobs, jid))) {
      printf("%s: No such job\n", argv[1]);
      return;
    }
  }	    
  else {
    printf("%s: argument must be a PID or %%jobid\n", argv[0]);
    return;
  }

  //
  // You need to complete rest. At this point,
  // the variable 'jobp' is the job pointer
  // for the job ID specified as an argument.
  //
  // Your actions will depend on the specified command
  // so we've converted argv[0] to a string (cmd) for
  // your benefit.

  string cmd(argv[0]);
  kill(-jobp->pid, SIGCONT); // Send SIGCONT to job with == pid
  if(cmd == "fg"){
    jobp->state = FG; // changes jobs pid to FG
    waitfg(jobp->pid); // Waits for job completion bc FG
  } 
  else {
    jobp->state = BG; //change job pid to BG  not wait becuase background
    printf("[%d] (%d) %s", pid2jid(jobp->pid), jobp->pid, jobp->cmdline);
  } 

  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// waitfg - Block until process pid is no longer the foreground process
//
void waitfg(pid_t pid) {
    
  while(fgpid(jobs) == pid) { //runs a endless loop till fg job is done then exits
    sleep(1); //prevents checking loop every itteration //Waste and unsafe
//     sigprocmask(SIG_BLOCK, &mask, &prevmask);
//     sigsuspend(&mask);
//     sigprocmask(SIG_UNBLOCK, &mask, NULL);
  } 
    
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// Signal handlers
//


/////////////////////////////////////////////////////////////////////////////
//
// sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
//     a child job terminates (becomes a zombie), or stops because it
//     received a SIGSTOP or SIGTSTP signal. The handler reaps all
//     available zombie children, but doesn't wait for any other
//     currently running children to terminate.  
//
void sigchld_handler(int sig) {
    pid_t pid;
    int status;
    int olderrno = errno;
    sigset_t mask, prevmask; // Masks for signal blocking
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, &prevmask); //block signals to prevent race conditions
    
  while((pid = waitpid(WAIT_ANY, &status, WNOHANG | WUNTRACED)) > 0) {
    //While there are children still out there to kill
    //WAIT_ANY = -1, set of all child processes of parent
    //WNOHANG = if waitpid should return immediately instead of waiting, if there no child ready
    //WUNTRACED = status of stopped and terminated children
      
    //WNOHANG | WUNTRACED = Return immediately, with a return value of 0, if none of the children in the wait set has stopped or terminated, or with a return value equal to the PID of one of the stopped or terminated children.
      
    if(WIFSIGNALED(status)) { //exited because it raised a signal that caused it to exit //WTERMSIG determines which signal causes child to exit
      string s = "Job [" + to_string(pid2jid(pid)) + "] (" + to_string(pid) + ") termined by signal " + to_string(WTERMSIG(status)) + " \n";
      write(STDOUT_FILENO, s.c_str(), s.size()); //Printing with printf is unsafe because it is not async safe
      
    } 
    else if (WIFSTOPPED(status)) { //if the child process was stopped by delivery of a signal/is currently stopped //WSTOPSIG tells which signal causes child to stop
      string s = "Job [" + to_string(pid2jid(pid)) + "] (" + to_string(pid) + ") stopped by signal " + to_string(WSTOPSIG(status)) + " \n";
      write(STDOUT_FILENO, s.c_str(), s.size()); //Printing with printf is unsafe because it is not async safe
      getjobpid(jobs, pid)->state = ST;
      return;
    }
      
    deletejob(jobs, pid); 
    sigprocmask(SIG_SETMASK, &prevmask, NULL);
  } 
   errno = olderrno;
    return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigint_handler - The kernel sends a SIGINT to the shell whenver the
//    user types ctrl-c at the keyboard.  Catch it and send it along
//    to the foreground job.  
//
void sigint_handler(int sig) {
  pid_t fgPid = fgpid(jobs);
  if (fgPid != 0) { //If foreground process exists
    kill(-fgPid, SIGINT); // Negative pid kills all processes in group
  }
  return;
}

/////////////////////////////////////////////////////////////////////////////
//
// sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
//     the user types ctrl-z at the keyboard. Catch it and suspend the
//     foreground job by sending it a SIGTSTP.  
//
void sigtstp_handler(int sig) {
  pid_t fgPid = fgpid(jobs);
  if (fgPid != 0) {
    kill(-fgPid, SIGTSTP);
  }
  return;
}

/*********************
 * End signal handlers
 *********************/




