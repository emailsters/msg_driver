#ifndef _CLASS_MACROS_H_
#define _CLASS_MACROS_H_

#define DISALLOW_COPY_AND_ASSIGN(class_name) \
private: \
class_name(const class_name&); \
const class_name& operator=(const class_name&) \

#endif
