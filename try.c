/*
 * try.c
 *
 *  Created on: Sep 28, 2022
 *      Author: rnd
 */
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#define CLOCKID CLOCK_REALTIME
#define SIG SIGRTMIN

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
				   } while (0)

static void
print_siginfo(siginfo_t *si)
{
timer_t *tidp;
int or;

tidp = si->si_value.sival_ptr;

printf("    sival_ptr = %p; ", si->si_value.sival_ptr);
printf("    sival_ptr = %#jx\n", (uintmax_t) tidp);
#if 0
or = timer_getoverrun(*tidp);
if (or == -1)
   errExit("timer_getoverrun");
else
   printf("    overrun count = %d\n", or);
#endif

}

static void
handler(int sig, siginfo_t si, void *uc)
{
/* Note: calling printf() from a signal handler is not safe
  (and should not be done in production programs), since
  printf() is not async-signal-safe; see signal-safety(7).
  Nevertheless, we use printf() here as a simple way of
  showing that the handler was called. */

printf("Caught signal %d\n", sig);
print_siginfo(&si);
//signal(sig, SIG_IGN);
}

int
main(int argc, char *argv[])
{
	timer_t timerid, jolptimer;
	struct sigevent sev, jev;
	struct itimerspec its, jits;
	long long freq_nanosecs;
	sigset_t mask;
	struct sigaction sa;

	if (argc != 3) {
	   fprintf(stderr, "Usage: %s <sleep-secs> <freq-nanosecs>\n",
			   argv[0]);
	   exit(EXIT_FAILURE);
	}



	printf("Establishing handler for signal %d\n", SIG);

	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = &handler;


	if (sigaction(SIG, &sa, NULL) == -1)
	   errExit("sigaction");



	printf("Blocking signal %d\n", SIG);



	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timerid;

	jev.sigev_notify = SIGEV_SIGNAL;
	jev.sigev_signo = SIG;
	jev.sigev_value.sival_ptr = &jolptimer;

	if (timer_create(CLOCKID, &sev, &timerid) == -1)
	   errExit("timer_create");

	printf("1:timer ID is %#jx\n", (uintmax_t) timerid);

	if (timer_create(CLOCKID, &jev, &jolptimer) == -1)
	   errExit("jolptimer create");

	printf("2. jolptimer ID is %#jx\n", (uintmax_t) jolptimer);



	freq_nanosecs = atoll(argv[2]);
	its.it_value.tv_sec = 4;
	its.it_value.tv_nsec = 0;
	its.it_interval.tv_sec = its.it_value.tv_sec;
	its.it_interval.tv_nsec = its.it_value.tv_nsec;

	jits.it_value.tv_sec = 1;
	jits.it_value.tv_nsec = 0;
	jits.it_interval.tv_sec = jits.it_value.tv_sec;
	jits.it_interval.tv_nsec = jits.it_value.tv_nsec;

	if (timer_settime(timerid, 0, &its, NULL) == -1)
		errExit("timer_settime");

	if (timer_settime(jolptimer, 0, &jits, NULL) == -1)
		errExit("timer_settime");

	/* Sleep for a while; meanwhile, the timer may expire
	  multiple times. */
	while(1){
			printf("hello\n");
			sleep(2);
	}

	printf("Sleeping for %d seconds\n", atoi(argv[1]));
	sleep(atoi(argv[1]));

	/* Unlock the timer signal, so that timer notification
	  can be delivered. */

	printf("Unblocking signal %d\n", SIG);
	//    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)
	//        errExit("sigprocmask");

	exit(EXIT_SUCCESS);
}

