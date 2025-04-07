/*
 * cppthread.cpp - Lombo c++ thread demo code for LomboTech
 *
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef __cplusplus
extern "C" {
#endif

#define MOD_NAME	"slib"
#include "common.h"
#include "pthread.h"

class MutexLock;
class Condition;

int num = 0;
MutexLock *mutex;
Condition *cond;

class MutexLock
{
public:
	MutexLock()
	:_isLock(false){
		pthread_mutex_init(&_mutex, NULL);
	}
	~MutexLock() {
		pthread_mutex_destroy(&_mutex);
	}

	pthread_mutex_t *getMutexLocker() {
		return &_mutex;
	}
	void lock() {
		pthread_mutex_lock(&_mutex);
		_isLock = true;

	}
	void unlock() {
		pthread_mutex_unlock(&_mutex);
		_isLock = true;
	}
	bool state() const {
		return _isLock;
	}
private:
	MutexLock(const MutexLock&);
	MutexLock &operator =(const MutexLock&);
private:
	pthread_mutex_t _mutex;
	bool _isLock;
};

class Condition
{
public:
	Condition(MutexLock &mutex)
	:_mutex(mutex){
		pthread_cond_init(&_cond,NULL);
	}

	~Condition() {
		pthread_cond_destroy(&_cond);
	}

	void wait() {
		pthread_cond_wait(&_cond, _mutex.getMutexLocker());
	}
	void notify() {
		pthread_cond_signal(&_cond);
	}
	void allnotify() {
		/* error: 'pthread_cond_brodcast' was not declared in this scope */
		/* pthread_cond_brodcast(&_cond); */
	}

private:
	MutexLock & _mutex;
	pthread_cond_t _cond;
};

#ifdef __cplusplus
}
#endif
