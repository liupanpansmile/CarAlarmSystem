#ifndef  _MACRO_DEFINE
#define  _MACRO_DEFINE
//============basic===================


#ifdef DEBUG

#define log_error(log_fmt, log_arg...) \
        oa_log(LL_ERROR,   "[%s:%d][%s] " log_fmt "\n", \
                     __FILE__, __LINE__, __FUNCTION__, ##log_arg);

#define log_warn(log_fmt, log_arg...) \
        oa_log(LL_WARNING,   "[%s:%d][%s] " log_fmt "\n", \
                     __FILE__, __LINE__, __FUNCTION__, ##log_arg);

#define log_info(log_fmt, log_arg...) \
        oa_log(LL_INFO,   "[%s:%d][%s] " log_fmt "\n", \
                     __FILE__, __LINE__, __FUNCTION__, ##log_arg);

#define log_trace(log_fmt, log_arg...) \
        oa_log(LL_TRACE,   "[%s:%d][%s] " log_fmt "\n", \
                     __FILE__, __LINE__, __FUNCTION__, ##log_arg);

#define log_debug(log_fmt, log_arg...) \
        oa_log(LL_DEBUG,   "[%s:%d][%s] " log_fmt "\n", \
                     __FILE__, __LINE__, __FUNCTION__, ##log_arg);

#else

#define log_error(log_fmt, log_arg...)			\
        oa_log(LL_ERROR,   "[%s:%d] " log_fmt "\n",		\
                     __FILE__, __LINE__, ##log_arg);

#define log_warn(log_fmt, log_arg...)			\
        oa_log(LL_WARNING,   "[%s:%d] " log_fmt "\n",	\
                     __FILE__, __LINE__, ##log_arg);

#define log_info(log_fmt, log_arg...)			\
        oa_log(LL_INFO,   "[%s:%d] " log_fmt "\n",		\
                     __FILE__, __LINE__, ##log_arg);

#define log_trace(log_fmt, log_arg...)			\
        oa_log(LL_TRACE,   "[%s:%d] " log_fmt "\n",		\
                     __FILE__, __LINE__, ##log_arg);

#define log_debug(log_fmt, log_arg...)			\
        oa_log(LL_DEBUG,   "[%s:%d] " log_fmt "\n",		\
                     __FILE__, __LINE__, ##log_arg);

#endif

#define SAFE_CHECK(p) \
	do{ \
	if(!p) return;  \
	} while (0)

#define SAFE_CHECK_RET(p,r) \
	do{ \
		if(!p) \
		{return r;} \
	} while (0)

#endif
