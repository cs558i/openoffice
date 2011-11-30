/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#include "system.h"

#include <osl/mutex.h>
#include <osl/diagnose.h>

#include <pthread.h>
#include <stdlib.h>

#if defined LINUX /* bad hack */
int pthread_mutexattr_setkind_np(pthread_mutexattr_t *, int);
#define pthread_mutexattr_settype pthread_mutexattr_setkind_np
#define PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP
#endif

/*
	Implementation notes:
	oslMutex hides a pointer to the oslMutexImpl structure, which
	ist needed to manage recursive locks on a mutex.
	
*/

typedef struct _oslMutexImpl
{
	pthread_mutex_t	mutex;
} oslMutexImpl;


/*****************************************************************************/
/* osl_createMutex */
/*****************************************************************************/
oslMutex SAL_CALL osl_createMutex() 
{
	oslMutexImpl* pMutex = (oslMutexImpl*) malloc(sizeof(oslMutexImpl));
    pthread_mutexattr_t aMutexAttr;
    int nRet=0;
    
	OSL_ASSERT(pMutex);

	if ( pMutex == 0 )
	{
		return 0;
	}
        
    pthread_mutexattr_init(&aMutexAttr);

    nRet = pthread_mutexattr_settype(&aMutexAttr, PTHREAD_MUTEX_RECURSIVE);
    
    nRet = pthread_mutex_init(&(pMutex->mutex), &aMutexAttr);
	if ( nRet != 0 )
	{
	    OSL_TRACE("osl_createMutex : mutex init failed. Errno: %d; %s\n",  
                  nRet, strerror(nRet));
        
	    free(pMutex);
		pMutex = 0;
	}

    pthread_mutexattr_destroy(&aMutexAttr);
    
	return (oslMutex) pMutex;
}

/*****************************************************************************/
/* osl_destroyMutex */
/*****************************************************************************/
void SAL_CALL osl_destroyMutex(oslMutex Mutex)
{
	oslMutexImpl* pMutex = (oslMutexImpl*) Mutex;

    OSL_ASSERT(pMutex);
    
	if ( pMutex != 0 )
	{
        int nRet=0;
        
	    nRet = pthread_mutex_destroy(&(pMutex->mutex));
        if ( nRet != 0 )
        {
            OSL_TRACE("osl_destroyMutex : mutex destroy failed. Errno: %d; %s\n",  
                      nRet, strerror(nRet));
        }

		free(pMutex);
	}
    
    return;
}

/*****************************************************************************/
/* osl_acquireMutex */
/*****************************************************************************/
sal_Bool SAL_CALL osl_acquireMutex(oslMutex Mutex) 
{
	oslMutexImpl* pMutex = (oslMutexImpl*) Mutex;
    
    OSL_ASSERT(pMutex);

	if ( pMutex != 0 )
	{
        int nRet=0;
        
        nRet = pthread_mutex_lock(&(pMutex->mutex));
        if ( nRet != 0 )
        {
            OSL_TRACE("osl_acquireMutex : mutex lock failed. Errno: %d; %s\n",  
                      nRet, strerror(nRet));
			return sal_False;
        }
		return sal_True;		
	}

    /* not initialized */
    return sal_False;
}

/*****************************************************************************/
/* osl_tryToAcquireMutex */
/*****************************************************************************/
sal_Bool SAL_CALL osl_tryToAcquireMutex(oslMutex Mutex) 
{
	oslMutexImpl* pMutex = (oslMutexImpl*) Mutex;

    OSL_ASSERT(pMutex);

	if ( pMutex ) 
	{
		int nRet = 0;
        nRet = pthread_mutex_trylock(&(pMutex->mutex));
        if ( nRet != 0  )
            return sal_False;
        
		return sal_True;
	}
    
    /* not initialized */
    return sal_False;
}

/*****************************************************************************/
/* osl_releaseMutex */
/*****************************************************************************/
sal_Bool SAL_CALL osl_releaseMutex(oslMutex Mutex) 
{
	oslMutexImpl* pMutex = (oslMutexImpl*) Mutex;
    
    OSL_ASSERT(pMutex);

	if ( pMutex ) 
	{
        int nRet=0;
        nRet = pthread_mutex_unlock(&(pMutex->mutex));
        if ( nRet != 0 )
        {
            OSL_TRACE("osl_releaseMutex : mutex unlock failed. Errno: %d; %s\n",  
                      nRet, strerror(nRet));
			return sal_False;
        }
            
        return sal_True;
	} 

    /* not initialized */
    return sal_False;
}

/*****************************************************************************/
/* osl_getGlobalMutex */
/*****************************************************************************/

static oslMutexImpl globalMutexImpl;

static void globalMutexInitImpl(void) {
    pthread_mutexattr_t attr;
    if (pthread_mutexattr_init(&attr) != 0 ||
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) ||
        pthread_mutex_init(&globalMutexImpl.mutex, &attr) != 0 ||
        pthread_mutexattr_destroy(&attr) != 0)
    {
        abort();
    }
}

oslMutex * SAL_CALL osl_getGlobalMutex() 
{
	/* necessary to get a "oslMutex *" */
	static oslMutex globalMutex = (oslMutex) &globalMutexImpl;

    static pthread_once_t once = PTHREAD_ONCE_INIT;
    if (pthread_once(&once, &globalMutexInitImpl) != 0) {
        abort();
    }
    
	return &globalMutex;
}
