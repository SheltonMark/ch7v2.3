
#ifndef __PRINTGRADE_H__
#define __PRINTGRADE_H__


#ifdef __cplusplus 
extern "C" 
{
#endif
 int Print(const char *libName, const char *file, int line, int nLevel,  const char *fmt,...);
#ifdef __cplusplus
}
#endif

typedef enum{

		LEVEL_PRINT = 0, //IMPORT
		LEVEL_ERROR = 1,
		LEVEL_WARN = 2,
		LEVEL_INFO = 3,  
		LEVEL_TRACE = 4, //白色
		LEVEL_DEBUG = 5,
		
	 }PRINT_LEVLE;


#ifdef PRINT_MODULE_NAME 	

#define PRINT(fmt,...) Print("", __FILE__, __LINE__, LEVEL_PRINT, fmt, ##__VA_ARGS__);

#define PRINT_ERROR(fmt,...) Print(PRINT_MODULE_NAME, __FILE__, __LINE__, LEVEL_ERROR , fmt, ##__VA_ARGS__)									 			

#define PRINT_WARN(fmt,...)	Print(PRINT_MODULE_NAME, __FILE__, __LINE__, LEVEL_WARN , fmt, ##__VA_ARGS__)

#define PRINT_INFO(fmt,...) Print(PRINT_MODULE_NAME, __FILE__, __LINE__, LEVEL_INFO , fmt, ##__VA_ARGS__)

#define PRINT_TRACE(fmt,...) Print(PRINT_MODULE_NAME, __FILE__, __LINE__, LEVEL_TRACE, fmt, ##__VA_ARGS__)

#define PRINT_DEBUG(fmt,...) Print(PRINT_MODULE_NAME, __FILE__, __LINE__, LEVEL_DEBUG, fmt, ##__VA_ARGS__)


#else

#define PRINT(fmt,...) Print("", __FILE__, __LINE__, LEVEL_PRINT, fmt, ##__VA_ARGS__);

#define PRINT_ERROR(fmt,...) Print("", __FILE__, __LINE__, LEVEL_ERROR , fmt, ##__VA_ARGS__)									 			


#define PRINT_WARN(fmt,...)	Print("", __FILE__, __LINE__, LEVEL_WARN, fmt, ##__VA_ARGS__)


#define PRINT_INFO(fmt,...)	Print("", __FILE__, __LINE__, LEVEL_INFO , fmt, ##__VA_ARGS__)


#define PRINT_TRACE(fmt,...) Print("", __FILE__, __LINE__, LEVEL_TRACE, fmt, ##__VA_ARGS__)


#define PRINT_DEBUG(fmt,...) Print("", __FILE__, __LINE__, LEVEL_DEBUG, fmt, ##__VA_ARGS__)


#endif	



#endif










