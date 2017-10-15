#ifndef KERNEL_VNODE_H
#define KERNEL_VNODE_H

#include <sys/types.h>

class Vnode
{
	public:
			virtual ssize_t write(const void* buffer, size_t size);
			virtual ~Vnode(){}
};

#endif
