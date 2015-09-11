#ifndef _TOOLS_H_
#define _TOOLS_H_
#include <unistd.h>
#include <string>
inline std::string GetProcPath()
{
	char buf[1024] = { 0 };
    size_t n = readlink("/proc/self/exe" , buf , sizeof(buf));
    if (n < 1024)
    {
    	buf[n] = 0;
    	for (int i = n; i >= 0; --i)
    	{
    		if (buf[i] == '/')
    		{
    			buf[i] = 0;
    			break;
    		}
    	}
    	return std::string(buf);
    }
    return "";
}

#endif
