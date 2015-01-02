#if 0
struct entry {
	int name;
	char *str;
};

struct entry pathconf_tab[] = {
	{_PC_FILESIZEBITS,		"FILESIZEBITS"},
	{_PC_LINK_MAX,			"LINK_MAX"},
	{_PC_MAX_CANON,			"MAX_CANON"},
	{_PC_MAX_INPUT,			"MAX_INPUT"},
	{_PC_NAME_MAX,			"NAME_MAX"},
	{_PC_PATH_MAX,			"PATH_MAX"},
	{_PC_PIPE_BUF,			"PIPE_BUF"},
	{_PC_2_SYMLINKS,		"POSIX2_SYMLINKS"},
	{_PC_ALLOC_SIZE_MIN,		"POSIX_ALLOC_SIZE_MIN"},
	{_PC_REC_INCR_XFER_SIZE, 	"POSIX_REC_INCR_XFER_SIZE"},
	{_PC_REC_MAX_XFER_SIZE,		"POSIX_REC_MAX_XFER_SIZE"},
	{_PC_REC_MIN_XFER_SIZE,		"POSIX_REC_MIN_XFER_SIZE"},
	{_PC_REC_XFER_ALIGN,		"POSIX_REC_XFER_ALIGN"},
	{_PC_SYMLINK_MAX,		"SYMLINK_MAX"},
	{_PC_CHOWN_RESTRICTED,		"_POSIX_CHOWN_RESTRICTED"},
	{_PC_NO_TRUNC,			"_POSIX_NO_TRUNC"},
	{_PC_VDISABLE,			"_POSIX_VDISABLE"},
	{_PC_ASYNC_IO,			"_POSIX_ASYNC_IO"},
	{_PC_PRIO_IO,			"_POSIX_PRIO_IO"},
	{_PC_SYNC_IO,			"_POSIX_SYNC_IO"},
	{_PC_TIMESTAMP_RESOLUTION,	"_POSIX_TIMESTAMP_RESOLUTION"}
};

struct entry sysconf_tab[] = {
	{_SC_AIO_LISTIO_MAX,		"AIO_LISTIO_MAX"},
	{_SC_AIO_MAX,			"AIO_MAX"},
	{_SC_AIO_PRIO_DELTA_MAX,	"AIO_PRIO_DELTA_MAX"},
	{_SC_ARG_MAX,			"ARG_MAX"},
	{_SC_ATEXIT_MAX,		"ATEXIT_MAX"},
	{_SC_BC_BASE_MAX,		"BC_BASE_MAX"},
	{_SC_BC_DIM_MAX,		"BC_DIM_MAX"},
	{_SC_BC_SCALE_MAX,		"BC_SCALE_MAX"},
	{_SC_BC_STRING_MAX,		"BC_STRING_MAX"},
	{_SC_CHILD_MAX,			"CHILD_MAX"},
	{_SC_COLL_WEIGHTS_MAX,		"COLL_WEIGHTS_MAX"},
	{_SC_DELAYTIMER_MAX,		"DELAYTIMER_MAX"},
	{_SC_EXPR_NEST_MAX,		"EXPR_NEST_MAX"},
	{_SC_HOST_NAME_MAX,		"HOST_NAME_MAX"},
	{_SC_IOV_MAX,			"IOV_MAX"},
	{_SC_LINE_MAX,			"LINE_MAX"},
	{_SC_LOGIN_NAME_MAX,		"LOGIN_NAME_MAX"},
	{_SC_NGROUPS_MAX,		"NGROUPS_MAX"},
	{_SC_MQ_OPEN_MAX,		"MQ_OPEN_MAX"},
	{_SC_MQ_PRIO_MAX,		"MQ_PRIO_MAX"},
	{_SC_OPEN_MAX,			"OPEN_MAX"},
	{_SC_PAGE_SIZE,			"PAGE_SIZE"},
	{_SC_PAGESIZE,			"PAGESIZE"},
	{_SC_THREAD_DESTRUCTOR_ITERATIONS,"PTHREAD_DESTRUCTOR_ITERATIONS"},
	{_SC_THREAD_KEYS_MAX,		"PTHREAD_KEYS_MAX"},
	{_SC_THREAD_STACK_MIN,		"PTHREAD_STACK_MIN"},
	{_SC_THREAD_THREADS_MAX,	"PTHREAD_THREADS_MAX"},
	{_SC_RE_DUP_MAX,		"RE_DUP_MAX"},
	{_SC_RTSIG_MAX,			"RTSIG_MAX"},
	{_SC_SEM_NSEMS_MAX,		"SEM_NSEMS_MAX"},
	{_SC_SEM_VALUE_MAX,		"SEM_VALUE_MAX"},
	{_SC_SIGQUEUE_MAX,		"SIGQUEUE_MAX"},
	{_SC_STREAM_MAX,		"STREAM_MAX"},
	{_SC_SYMLOOP_MAX,		"SYMLOOP_MAX"},
	{_SC_TIMER_MAX,			"TIMER_MAX"},
	{_SC_TTY_NAME_MAX,		"TTY_NAME_MAX"},
	{_SC_TZNAME_MAX,		"TZNAME_MAX"},
	{_SC_ADVISORY_INFO,		"_POSIX_BARRIERS"},
	{_SC_ASYNCHRONOUS_IO,		"_POSIX_ASYNCHRONOUS_IO"},
	{_SC_CLOCK_SELECTION,		"_POSIX_CLOCK_SELECTION"},
	{_SC_CPUTIME,			"_POSIX_CPUTIME"},
	{_SC_FSYNC,			"_POSIX_FSYNC"},
	{_SC_IPV6,			"_POSIX_IPV6"},
	{_SC_JOB_CONTROL,		"_POSIX_JOB_CONTROL"},
	{_SC_MAPPED_FILES,		"_POSIX_MAPPED_FILES"},
	{_SC_MEMLOCK,			"_POSIX_MEMLOCK"},
	{_SC_MEMLOCK_RANGE,		"_POSIX_MEMLOCK_RANGE"},
	{_SC_MEMORY_PROTECTION,		"_POSIX_MEMORY_PROTECTION"},
	{_SC_MESSAGE_PASSING,		"_POSIX_MESSAGE_PASSING"},
	{_SC_MONOTONIC_CLOCK,		"_POSIX_MONOTONIC_CLOCK"},
	{_SC_PRIORITIZED_IO,		"_POSIX_PRIORITIZED_IO"},
	{_SC_PRIORITY_SCHEDULING,	"_POSIX_PRIORITY_SCHEDULING"},
	{_SC_RAW_SOCKETS,		"_POSIX_RAW_SOCKETS"},
	{_SC_READER_WRITER_LOCKS,	"_POSIX_READER_WRITER_LOCKS"},
	{_SC_REALTIME_SIGNALS,		"_POSIX_REALTIME_SIGNALS"},
	{_SC_REGEXP,			"_POSIX_REGEXP"},
	{_SC_SAVED_IDS,			"_POSIX_SAVED_IDS"},
	{_SC_SEMAPHORES,		"_POSIX_SEMAPHORES"},
	{_SC_SHARED_MEMORY_OBJECTS,	"_POSIX_SHARED_MEMORY_OBJECTS"},
	{_SC_SHELL,			"_POSIX_SHELL"},
	{_SC_SPAWN,			"_POSIX_SPAWN"},
	{_SC_SPIN_LOCKS,		"_POSIX_SPIN_LOCKS"},
	{_SC_SPORADIC_SERVER,		"_POSIX_SPORADIC_SERVER"},
	{_SC_SS_REPL_MAX,		"_POSIX_SS_REPL_MAX"},
	{_SC_SYNCHRONIZED_IO,		"_POSIX_SYNCHRONIZED_IO"},
	{_SC_THREAD_ATTR_STACKADDR,	"_POSIX_THREAD_ATTR_STACKADDR"},
	{_SC_THREAD_ATTR_STACKSIZE,	"_POSIX_THREAD_ATTR_STACKSIZE"},
	{_SC_THREAD_CPUTIME,		"_POSIX_THREAD_CPUTIME"},
	{_SC_THREAD_PRIO_INHERIT,	"_POSIX_THREAD_PRIO_INHERIT"},
	{_SC_THREAD_PRIO_PROTECT,	"_POSIX_THREAD_PRIO_PROTECT"},
	{_SC_THREAD_PRIORITY_SCHEDULING,"_POSIX_THREAD_PRIORITY_SCHEDULING"},
	{_SC_THREAD_PROCESS_SHARED,	"_POSIX_THREAD_PROCESS_SHARED"},
	{_SC_THREAD_ROBUST_PRIO_INHERIT,"_POSIX_THREAD_ROBUST_PRIO_INHERIT"},
	[_SC_THREAD_ROBUST_PRIO_PROTECT,"_POSIX_THREAD_ROBUST_PRIO_PROTECT"},
	{_SC_THREAD_SAFE_FUNCTIONS,	"_POSIX_THREAD_SAFE_FUNCTIONS"},
	{_SC_THREAD_SPORADIC_SERVER,	"_POSIX_THREAD_SPORADIC_SERVER"},
	{_SC_THREADS,			"_POSIX_THREADS"},
	{_SC_TIMEOUTS,			"_POSIX_TIMEOUTS"},
	{_SC_TIMERS,			"_POSIX_TIMERS"},
	{_SC_TRACE,			"_POSIX_TRACE"},
	{_SC_TRACE_EVENT_FILTER,	"_POSIX_TRACE_EVENT_FILTER"},
	{_SC_TRACE_EVENT_NAME_MAX,	"_POSIX_TRACE_EVENT_NAME_MAX"},
	{_SC_TRACE_INHERIT,		"_POSIX_TRACE_INHERIT"},
	{_SC_TRACE_LOG,			"_POSIX_TRACE_LOG"},
	{_SC_TRACE_NAME_MAX,		"_POSIX_TRACE_NAME_MAX"},
	{_SC_TRACE_SYS_MAX,		"_POSIX_TRACE_SYS_MAX"},
	{_SC_TRACE_USER_EVENT_MAX,	"_POSIX_TRACE_USER_EVENT_MAX"},
	{_SC_VERSION,			"_POSIX_VERSION"},
	{_SC_V7_ILP32_OFF32,		"_POSIX_V7_ILP32_OFF32"},
	{_SC_V7_ILP32_OFFBIG,		"_POSIX_V7_ILP32_OFFBIG"},
	{_SC_V7_LP64_OFF64,		"_POSIX_V7_LP64_OFF64"},
	{_SC_V7_LPBIG_OFFBIG,		"_POSIX_V7_LPBIG_OFFBIG"},
	{_SC_V6_ILP32_OFF32,		"_POSIX_V6_ILP32_OFF32"},
	{_SC_V6_ILP32_OFFBIG,		"_POSIX_V6_ILP32_OFFBIG"},
	{_SC_V6_LP64_OFF64,		"_POSIX_V6_LP64_OFF64"},
	{_SC_2_C_BIND,			"_POSIX2_C_BIND"},
	{_SC_2_C_DEV,			"_POSIX2_C_DEV"},
	{_SC_2_CHAR_TERM,		"_POSIX2_CHAR_TERM"},
	{_SC_2_LOCALEDEF,		"_POSIX2_LOCALEDEF"},
	{_SC_2_PBS,			"_POSIX2_PBS"},
	{_SC_2_PBS_ACCOUNTING,		"_POSIX2_PBS_ACCOUNTING"},
	{_SC_2_PBS_CHECKPOINT,		"_POSIX2_PBS_CHECKPOINT"},
	{_SC_2_PBS_LOCATE,		"_POSIX2_PBS_LOCATE"},
	{_SC_2_PBS_MESSAGE,		"_POSIX2_PBS_MESSAGE"},
	{_SC_2_PBS_TRACK,		"_POSIX2_PBS_TRACK"},
	{_SC_2_SW_DEV,			"_POSIX2_SW_DEV"},
	{_SC_2_UPE,			"_POSIX2_UPE"},
	{_SC_2_VERSION,			"_POSIX2_VERSION"},
	{_SC_XOPEN_CRYPT,		"_XOPEN_CRYPT"},
	{_SC_XOPEN_ENH_I18N,		"_XOPEN_ENH_I18N"},
	{_SC_XOPEN_REALTIME,		"_XOPEN_REALTIME"},
	{_SC_XOPEN_REALTIME_THREADS,	"_XOPEN_REALTIME_THREADS"},
	{_SC_XOPEN_SHM,			"_XOPEN_SHM"},
	{_SC_XOPEN_STREAMS,		"_XOPEN_STREAMS"},
	{_SC_XOPEN_UNIX,		"_XOPEN_UNIX"},
	{_SC_XOPEN_UUCP,		"_XOPEN_UUCP"},
	{_SC_XOPEN_VERSION,		"_XOPEN_VERSION"}
};
#endif
int main()
{
	return 1;
}
