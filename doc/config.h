/**
 * \defgroup config Platform Configuration Parameters
 *
 * These parameters control the build of the Open Files stack.
 *
 * Cmake will use these parameters as it configures and builds all
 * components
 */

/** \{ */
/**
 * \brief
 * Specifies whether the compiler supports 64 bit integers
 *
 * \param ON Compiler supports 64 bit integer
 * \param OFF Compiler does not support 64 bit integers
 */
OFC_64_BIT_INTEGER;
/**
 * \brief
 * Specifies whether the target platform supports 64 bit pointers
 *
 * \param ON platform supports 64 bit pointers
 * \param OFF platform supports 32 bit pointers
 */
OFC_64BIT_POINTER;
/**
 * \brief
 * Specifies that the compiler treats long int as a 64 bit value
 *
 * \param ON Compiler treats long ints as 64 bits (8 bytes)
 * \param OFF Compiler treats long ints as 32 bits (4 bytes)
 */
OFC_LONGINT_64;
/**
 * \brief
 * Include persistent configuration support in the target library.
 *
 * This provides the ability to load and save runtime configuration as
 * an xml file.  XML file location is defined through a platform 
 * environment variable (\see OPEN_FILES_HOME)
 *
 * \param ON Build in persistent configuration support
 * \param OFF Do not build in persistent configuration support
 */
OFC_PERSIST;
/**
 * \brief
 * Build in support for measuring performance of various facilities
 *
 * Facilities measured are:
 * - Application File Read/Write
 * - SMB Client Read/Write (if included)
 * - SMB Server Read/Write (if included)
 * - SMB Session Read/Write (if included)
 *
 * \param ON Build in support for measuring performance
 * \param OFF Do not build in support for measuring performance
 */
OFC_PERF_STATS;
/**
 * \brief
 * Specify whether to build in support for detecting handle leaks
 *
 * \param ON Leak detection for handles is included.
 * \param OFF Leak detection for handles is not included.
 */
OFC_HANDLE_DEBUG;
/**
 * \brief
 * Specify whether to build in support for detecting message leaks
 *
 * Messages are a special construct for encapsulating network I/O
 *
 * \param ON Leak detection for messages is included.
 * \param OFF Leak detection for messages is not included.
 */
OFC_MESSAGE_DEBUG;
/**
 * \brief
 * Specify whether to build in support for detecting app leaks
 *
 * Apps are an Open Files encapsulation for event-driven, asysnchronous
 * processing streams.
 *
 * \param ON Leak detection for apps is included.
 * \param OFF Leak detection for apps is not included.
 */
OFC_APP_DEBUG;
/**
 * \brief
 * Specify whether to build in support for heap leaks
 *
 * \param ON Leak detection for heap allocations is included.
 * \param OFF Leak detection for heap allocations is not included.
 */
OFC_HEAP_DEBUG;
/**
 * \brief
 * Specify whether the default file names are wide characters or not
 *
 * APIs are always provided for both file naming scheme.  A file
 * function typically has three variants:
 *
 * - wide character variant.  A 'W' following the function name.
 * - ascii character variant. A 'A' following the function name.
 * - default character variant.  No function name suffix.
 *
 * This configuration parameter defines what variant the default will
 * use.
 *
 * \param ON Use wide character APIs by default.
 * \param OFF Use ascii character APIs by default.
 */
OFC_UNICODE_API;
/**
 * \brief
 * Define whether call traces should be provided in debug output when
 * available.
 * 
 * Stack traces are available when this parameter is enabled and the
 * associated debug parameter for the following allocations are enabled:
 *
 * - heap allocation dumps
 * - handle allocation dumps
 * - file handle allocation dumps
 * - message allocation dumps
 * 
 * \param ON Build in support for call stack dumps
 * \param OFF Do not build in support for call stack dumps
 */
OFC_STACK_TRACE;
/**
 * \brief
 * Define how the event-driven scheduler should behave (experimental)
 *
 * The Open Files event driven scheduler is designed around the notion of
 * a pre-select and a post-select application callback.  The pre-select
 * callback determines which events to wake up for and the post-select
 * callback processes events which have fired.  This configuration parameter
 * specifies whether the pre-select callback should be called for each
 * application for each scheduler pass.  Calling the pre-select callback
 * on each scheduler pass has a performance impact but can help if
 * events are not getting enabled in certain application states.
 *
 * Generally this configuration option should be disabled.  Enabling this
 * option is only recommended or debugging stalls.
 *
 * \param ON Include a preselect callback on every scheduler pass
 * \param OFF Do not call preselect callbacks on every scheduler pass.
 */
OFC_PRESELECT_PASS;
/**
 * \brief
 * Configure the maximum number of 16 bit handles.
 *
 * A 16 bit handle is a construct that allows the mapping of an 16-bit 'id'
 * to a data structure.  These types of ids are useful with certain
 * network protocols.  Modern protocols (ie. smbv2/3) do not require
 * 16 bit handles.
 *
 * This parameter allows the configuration of the maximum number of 16
 * bit handles.
 *
 * \param MAX Max number of handles (string)
 */
OFC_MAX_HANDLE16;
/**
 * \brief
 * Configure the maximum number of file system maps to support.
 *
 * A file system map is an Open Files feature that provides aliases to
 * paths.  An alias can be specified as a windows like device name 
 * (i.e DEV:) or as a server name (i.e. //<server>).  Maps are useful
 * constructs in sophisticated file applications.
 * 
 * \param MAX Max number of maps supported (string)
 */
OFC_MAX_MAPS;
/**
 * \brief
 * Configure the size of the debug trace buffer.
 *
 * Open Files provides a circular trace buffer that the stack itself and
 * applications can push things into through a printf type of call.
 * This parameter specifies the size of that circular buffer.  A value
 * of 0 disables tracing.
 *
 * \param SIZE Size of debug trace buffer
 */
OFC_TRACE_LEN;
/**
 * \brief
 * Sets the maximum sleep that the event-driven scheduler will sleep
 * between scheduler passes.
 *
 * Typically, this value is set to a high value (ex. 10000 ms).  In
 * the event that a wake-up event is missed, the scheduler can still
 * be forced to wake up after the max sleep has expired.  
 *
 * \param MAX Maximum scheduler sleep between passes (ms)
 */
OFC_MAX_SCHED_WAIT;
/**
 * \brief
 * Specify the type of the platform wide character
 *
 * Open Files attempts to use the platform type for wide characters.
 * This setting specifies that type.
 *
 * \param TYPE The wide character type (i.e. wchar_t)
 */
OFC_WCHAR;
/**
 * \brief
 * Specifies whether to use a platform include for the wide char typedef
 *
 * Wide character types are typically not defined by the compiler, rather
 * they are specified by some platform include.  This parameter directs
 * the Open Files build to include a platform include file that defines
 * the wide character type.
 *
 * \param ON Include a wide character include file 
 * \param OFF Wide character include file not necessary
 */
INCLUDE_PLATFORM_WCHAR;
/**
 * \brief
 * Specifies file to include for wide character definition
 *
 * If INCLUDE_PLATFORM_WCHAR is enabled, this parameter defines the
 * name of the file to include. 
 *
 * \param FILE File that defines wide character (eg. wchar.h)
 */
PLATFORM_WCHAR;
/**
 * \brief
 * Specify the type of the platform process id
 *
 * Open Files attempts to use the platform type for process ids
 * This setting specifies that type.
 *
 * \param TYPE The process id type (eg. pid_t)
 */
OFC_PROCESS_ID;
/**
 * \brief
 * Specifies whether to use a platform include for the process id typedef
 *
 * Process ID are typically not defined by the compiler, rather
 * they are specified by some platform include.  This parameter directs
 * the Open Files build to include a platform include file that defines
 * the process id type.
 *
 * \param ON Include a process id include file 
 * \param OFF Process id include file not necessary
 */
INCLUDE_PLATFORM_PROCESS_ID;
/**
 * \brief
 * Specifies file to include for process id definition
 *
 * If INCLUDE_PLATFORM_PROCESS_ID is enabled, this parameter defines the
 * name of the file to include. 
 *
 * \param FILE File that defines process id type (eg. sys/types.h)
 */
PLATFORM_PROCESS_ID;
/**
 * \brief
 * Specify a particular build variant name
 *
 * This variant will be recorded, along with the build version during
 * startup.  It is used for identification purposes only.
 *
 * \param VARIANT Variant name of build
 */
OFC_SHARE_VARIANT;
/**
 * \brief
 * Specifies the default hostname of the deployment.
 *
 * Generally, the hostname of the deployment is specified in the
 * runtime configuration or by the platform itself.  If neither of
 * those methods are available, this will provide a default name.
 *
 * \param NAME Default hostname (eg. localhost)
 */
OFC_DEFAULT_NAME;
/**
 * \brief
 * Specifies the default IPv6 scope
 *
 * IPv6 typically requires a scope to be specified for IPv6 addressing.
 * This parameters specifies that default scope.  An empty string is
 * allowed.
 *
 * \param SCOPE Default Scope (eg. "")
 */
OFC_DEFAULT_SCOPE;
/**
 * \brief
 * Specify the default system description.
 *
 * Typically the system description is specified during runtime.  In the
 * event that is not provided, the default description will be used.
 *
 * \param DESCR Default System Description.
 */
OFC_DEFAULT_DESCR;
/**
 * \brief
 * Specify the default network domain.
 *
 * Typically the network domain is specified during runtime.  In the event
 * that that is not provided, the default domain will be used.
 *
 * \param DOMAIN Default network domain
 */
OFC_DEFAULT_DOMAIN;
/**
 * \brief
 * Specify whether Open Files stack should be initialized on library
 * load or through an explicit call.
 *
 * This typically requires compiler and loader support.  
 *
 * \param ON Open Files is initialized on library load
 * \param OFF Open Files is initialized through explicit call
 */
INIT_ON_LOAD;
/**
 * \brief
 * Compiler construct for specifying load routines
 *
 * If INIT_ON_LOAD is specified, the initialization routines need to
 * contain an attribute which identifies it as a constructor.  This
 * parameter allows the format of that attribute to be specified.
 *
 * \param LOAD Compiler attribute for constructor (eg. __attribute__((constructor)))
 */
OFC_LOAD;
/**
 * \brief
 * Compiler construct for specifying unload routines
 *
 * If INIT_ON_LOAD is specified, the destructive routines need to
 * contain an attribute which identifies it as a destructor.  This
 * parameter allows the format of that attribute to be specified.
 *
 * \param UNLOAD Compiler attribute for destructor (eg. __attribute__((destructor)))
 */
OFC_UNLOAD;
/**
 * \brief
 * Configure the core platform handler to use
 *
 * This is used by cmake to pull in the correct platform library.  Standard
 * platforms are:
 *
 * - darwin
 * - windows
 * - android
 * - linux
 * - threadx
 * - nucleus
 *
 * Additional platforms can be added by providing a platform support library
 * and by modifying the CMakeLists.txt file to build and include the correct
 * platform library
 *
 * \param PLATFORM Platform Library to include (eg. darwin)
 */
OFC_IMPL;
/**
 * \brief
 * Configure the heap handler to use
 *
 * This is used by cmake to pull in the correct heap library.  Standard
 * heaps are:
 *
 * - binheap: A statically allocated heap with advanced debugging
 * - cheap: A standard passthrough for malloc/free type heap.
 *
 * Additional heaps can be added by providing a heap support library
 * and by modifying the CMakeLists.txt file to build and include the correct
 * heap library
 *
 * \param HEAP Heap Algorithm to use (eg. binheap)
 */
OFC_HEAP_IMPL;
/**
 * \brief
 * Specify that IPv4 interfaces should be found.
 *
 * Open Files will perform automatic network interface discovery.  This
 * is on by default and can be configured during runtime.  If automatic
 * interface discovery is enabled, this parameter determines if IPv4 
 * interaces and addresses are discovered.
 *
 * \param ON IPv4 interfaces and addresses are discovered
 * \param OFF IPv4 interfaces and addresses are ignored
 */
OFC_DISCOVER_IPV4;
/**
 * \brief
 * Specify that IPv6 interfaces should be found.
 *
 * Open Files will perform automatic network interface discovery.  This
 * is on by default and can be configured during runtime.  If automatic
 * interface discovery is enabled, this parameter determines if IPv6 
 * interaces and addresses are discovered.
 *
 * \param ON IPv6 interfaces and addresses are discovered
 * \param OFF IPv6 interfaces and addresses are ignored
 */
OFC_DISCOVER_IPV6;
/**
 * \brief
 * Specify whether a darwin file system handler should be included.
 *
 * Open Files supports multiple file system handlers.  Specifying this
 * parameter will include the darwin file system.
 *
 * \param ON Include the darwin file system
 * \param OFF Disable the darwin file system
 */
OFC_FS_DARWIN;
/**
 * \brief
 * Specify whether a bookmark file system handler should be included.
 *
 * Open Files supports multiple file system handlers.  Specifying this
 * parameter will include the bookmark file system.  A bookmark file
 * system works hand in hand with file system maps.  
 *
 * \param ON Include the bookmark file system
 * \param OFF Disable the bookmark file system
 */
OFC_FS_BOOKMARKS;
/**
 * \brief
 * Specify the test path for the darwin file system test
 *
 * The Darwin file system test requires a path to a test directory.  This
 * parameter provides that path.
 *
 * \param PATH Path to darwin test path (eg. /tmp/openfiles/test)
 */
OFC_TEST_FS_DARWIN_PATH;




