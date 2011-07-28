/*
* Copyright (c) 2005, Bull S.A..  All rights reserved.
* Created by: Sebastien Decugis

* This program is free software; you can redistribute it and/or modify it
* under the terms of version 2 of the GNU General Public License as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it would be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write the Free Software Foundation, Inc., 59
* Temple Place - Suite 330, Boston MA 02111-1307, USA.


* This sample test aims to check the following assertions:
*
* If SA_SIGINFO is set and realtime signals extension is supported, queueable
* signals generated by sigqueue or some other functions are delivered in FIFO
* order.

* The steps are:
* -> Test for RTS extension support
* -> install a handler for SIGRTMAX signal with SA_SIGINFO set.
* -> Mask this signal
* -> Generate the signal several imes with sigqueue and known user values.
* -> unmask the signal
* -> check that the signals are delivered in order.

* The test fails if the signals are not delivered in FIFO order.
*/

/* We are testing conformance to IEEE Std 1003.1, 2003 Edition */
#define _POSIX_C_SOURCE 200112L

/* This test uses some XSI features */
//#define _XOPEN_SOURCE 600

/******************************************************************************/
/*************************** standard includes ********************************/
/******************************************************************************/
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>
#include <errno.h>

/******************************************************************************/
/***************************   Test framework   *******************************/
/******************************************************************************/
#include "testfrmw.h"
#include "testfrmw.c" 
/* This header is responsible for defining the following macros:
 * UNRESOLVED(ret, descr);  
 *    where descr is a description of the error and ret is an int 
 *   (error code for example)
 * FAILED(descr);
 *    where descr is a short text saying why the test has failed.
 * PASSED();
 *    No parameter.
 * 
 * Both three macros shall terminate the calling process.
 * The testcase shall not terminate in any other maneer.
 * 
 * The other file defines the functions
 * void output_init()
 * void output(char * string, ...)
 * 
 * Those may be used to output information.
 */

/******************************************************************************/
/**************************** Configuration ***********************************/
/******************************************************************************/
#ifndef VERBOSE
#define VERBOSE 1
#endif

#define QUEUELENGTH 10

/******************************************************************************/
/***************************    Test case   ***********************************/
/******************************************************************************/

sig_atomic_t latest = 0;

void handler( int sig, siginfo_t *info, void *context )
{
	if ( info->si_signo != SIGRTMAX )
	{
		output( "Received unexpected signal %d\n", info->si_signo );
	}
	else
	{
		latest++;

		if ( latest != info->si_value.sival_int )
		{
			output( "Got signal %d, expected %d!\n", info->si_value.sival_int, latest );
			FAILED( "Wrong signal delivered -- no FIFO order?" );
		}
	}
}

/* main function */
int main()
{
	int ret;
	long rts;

	struct sigaction sa;
	union sigval sv;
	sigset_t mask;

	/* Initialize output */
	output_init();

	/* Test the RTS extension */
	rts = sysconf( _SC_REALTIME_SIGNALS );

	if ( rts < 0L )
	{
		UNTESTED( "This test needs the RTS extension" );
	}

	/* Set the signal handler */
	sa.sa_flags = SA_SIGINFO;

	sa.sa_sigaction = handler;

	ret = sigemptyset( &sa.sa_mask );

	if ( ret != 0 )
	{
		UNRESOLVED( ret, "Failed to empty signal set" );
	}

	/* Install the signal handler for SIGRTMAX */
	ret = sigaction( SIGRTMAX, &sa, 0 );

	if ( ret != 0 )
	{
		UNRESOLVED( ret, "Failed to set signal handler" );
	}

	/* Mask this signal */
	ret = sigemptyset( &mask );

	if ( ret != 0 )
	{
		UNRESOLVED( ret, "An error occured while initializing mask" );
	}

	ret = sigaddset( &mask, SIGRTMAX );

	if ( ret != 0 )
	{
		UNRESOLVED( ret, "Failed to add SIGRTMAX to signal set" );
	}

	ret = sigprocmask( SIG_BLOCK, &mask, NULL );

	if ( ret != 0 )
	{
		UNRESOLVED( ret, "Failed to set process signal mask" );
	}

	/* Now queue the signal to be pending */

	for ( sv.sival_int = 1; sv.sival_int <= QUEUELENGTH; sv.sival_int++ )
	{
		ret = sigqueue( getpid(), SIGRTMAX, sv );

		if ( ret != 0 )
		{
			UNRESOLVED( ret, "Failed to queue the signal" );
		}
	}

	if ( latest != 0 )
	{
		FAILED( "Signal was delivered while masked??" );
	}

	/* And finally unmask the signal so it is delivered */
	ret = sigprocmask( SIG_UNBLOCK, &mask, NULL );

	if ( ret != 0 )
	{
		UNRESOLVED( ret, "Failed to set process signal mask" );
	}

	sched_yield();

	/* Check the signal has been delivered as expected */

	if ( latest != QUEUELENGTH )
	{
		output( "Only %d signal delivered instead of %d\n", latest, QUEUELENGTH );

		if ( latest == 1 )
		{
			UNTESTED( "It seems like SIGRTMAX is not a queuable signal here?" );
		}
	}

	/* Test passed */
#if VERBOSE > 0

	output( "Test passed\n" );

#endif

	PASSED;
}
