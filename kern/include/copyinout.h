#ifndef _COPYINOUT_H_
#define _COPYINOUT_H_

int copyin(const_userptr_t usersrc, void *dest, size_t len);
int copyout(const void *src, userptr_t userdest, size_t len);

#endif // _COPYINOUT_H_
