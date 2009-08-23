/*!
 ASLog.h
 
 \file ASLog.h
 
 \brief Interface for ASLog utility class
 
 \author Copyright (c) 2008 by Alan Staniforth
 
 \date 2008-10-12
 
 This abstract class enhances the functionality of NSLog in several ways:
 
 -#	It creates debug logging commands as macros which only compile to functional code
	in debugging builds. In release builds these macros are compiled out.
 -# The basic NSLog status/origin information is enhanced by the addition of source 
	file name and line, (and optionally the function) from which the call comes.
 -# It is possible to switch the logging from the default destination of stderr to a 
	user specified file.
 
 This code is based on a number of sources and ideas.The general idea of using an enhanced,
 NSLog, implemented as macros which could be compiled out came from:
 
 http://borkware.com/rants/agentm/mlog/
 
 Adam Knight then presented the same idea, enhanced a little further and already 
 packaged up in a neat utility class:
 
 http://www.hopelessgeek.com/2005/11/18/better-logging
 
 I started with Adam's class and enhanced the macros as suggested on:
 
 http://powerof2games.com/node/10
 
 to be absolutely sure the macros were compiled out with no side effects. I added
 a "Warning" variant, which would not compile out as suggested in:
 
 http://mooseyard.com/Jens/2007/05/uncle-jenss-coding-tips/
 
 and the idea of reassigning stderr came from:
 
 http://www.cocoadev.com/index.pl?NSLogToFile
 
 I personally added the versions of the macros that do "ordinary" enhanced logging
 and do not get compiled out.
 
 */

#import <Foundation/NSDebug.h>
#import <Cocoa/Cocoa.h>



#pragma mark Macro defintions



/*!
 \name Debug Logging macros. 
 @relates ASLog
 
 Convenience interface to ASLog Debug Logging methods
 
 - Only compiled in when BUILD_WITH_DEBUG_LOGGING is defined.
 - Only fire when either DEBUG_LOG_AUTO_ENABLE is defined or the environment
	variable NSDebugEnabled exists and is set to YES
 
 */
//@{

	/*! \def ASDLogOn
	 @brief Enables debug logging at runtime
	 
	 \def ASDLogOff
	 @brief Disables debug logging at runtime
	 
	 \def ASDNSLog
	 @brief No enhancements to NSLog, but will be compiled out in release builds
	 
	 \def ASDLog
	 @brief NSLog + logs the sourcefile and line number
	 
	 \def ASDFnLog
	 @brief NSLog + logs the sourcefile and line number and calling method
	 */
#ifdef BUILD_WITH_DEBUG_LOGGING
	// BUILD_WITH_DEBUG_LOGGING is defined, compile the macros in
	#define ASDLogOn() do { [ASLog setLogOn:YES]; } while (0)
	#define ASDLogOff() do { [ASLog setLogOn:NO]; } while (0)
	#define ASDNSLog(s, ...) do { [ASLog debugLog:(s),##__VA_ARGS__]; } while (0)
	#define ASDLog(s, ...) do { [ASLog debugLog:__FILE__ lineNumber:__LINE__ format:(s),##__VA_ARGS__]; } while (0)
	#define ASDFnLog(s, ...) do { [ASLog debugLog:__FILE__ lineNumber:__LINE__ function:(char*)__FUNCTION__ format:(s),##__VA_ARGS__]; } while (0)
#else
	// NOOP definitions of the debug logging macros
	#define ASDLogOn() do { (void)sizeof(YES); } while (0)
	#define ASDLogOff() do { (void)sizeof(YES); } while (0)
	#define ASDNSLog(s, ...) do { (void)sizeof(s); } while (0)
	#define ASDLog(s, ...) do { (void)sizeof(s); } while (0)
	#define ASDFnLog(s, ...) do { (void)sizeof(s); } while (0)
#endif

//@} (Debug Logging macros)

/*!
 \name Normal Logging macros.
 @relates ASLog
 
 Convenience interface to ASLog Normal Logging methods
 
 - Still have NSLog enhancements.
 - Not compiled out in release builds.
 
 */
//@{

/*! \def ASFlLog
 @brief NSLog + logs the sourcefile and line number
 */
#define ASFlLog(s, ...) do { [ASLog log:__FILE__ lineNumber:__LINE__ format:(s),##__VA_ARGS__]; } while (0)

/*! \def ASFnLog
 @brief NSLog + logs the sourcefile and line number and calling method
 */
#define ASFnLog(s, ...) do { [ASLog log:__FILE__ lineNumber:__LINE__ function:(char*)__FUNCTION__ format:(s),##__VA_ARGS__]; } while (0)

//@} (Normal Logging macros)


/*!
 \name Warning Logging macros.
 @relates ASLog
 
 Convenience interface to ASLog Warning methods
 
 - Still have NSLog enhancements.
 - Not compiled out in release builds.
 - Obvious in a busy log as every line contains "WARNING"
 */
//@{

/*! \def ASNSWarn
 @brief NSLog + "WARNING"
 */
#define ASNSWarn(s, ...) do { [ASLog warn:(s),##__VA_ARGS__]; } while (0)

/*! \def ASWarn
 @brief NSLog + "WARNING" + logs the sourcefile and line number
 */
#define ASWarn(s, ...) do { [ASLog warn:__FILE__ lineNumber:__LINE__ format:(s),##__VA_ARGS__]; } while (0)

/*! \def ASFnWarn
 @brief NSLog + "WARNING" + logs the sourcefile and line number and calling method
 */
#define ASFnWarn(s, ...) do { [ASLog warn:__FILE__ lineNumber:__LINE__ function:(char*)__FUNCTION__ format:(s),##__VA_ARGS__]; } while (0)

//@} (Warning Logging macros)



#pragma mark Class interface

/*!
 \brief A better NSLog...
 
 
 This abstract class enhances the functionality of NSLog in several ways:
 
 -#	It creates debug logging commands as macros which only compile to functional code
 in debugging builds. In release builds these macros are compiled out.
 -# The basic NSLog status/origin information is enhanced by the addition of source 
 file name and line, (and optionally the function) from which the call comes.
 -# It is possible to switch the logging from the default destination of stderr to a 
 user specified file.
 
 To enable logging, set the environment variable NSDebugEnabled to YES in the 
 "Variables to be set in the environment" section of the "Arguments" tab of the 
 "Edit Active Target..." window.
 This is imported in Foundation/NSDebug.h as a flag for other debugging tools. 
 (The necessary header is imported in this class's header)
 
 */

@interface ASLog : NSObject {
}

/*!
 \name Debug Logging methods. 
 - Only fire when either DEBUG_LOG_AUTO_ENABLE is defined or the environment
 variable NSDebugEnabled exists and is set to YES
 */
//@{

//! @brief Plain NSLog, but will be compiled out in release builds
+ (void)debugLog:(NSString *)format, ...;

//! @brief NSLog, also logs source file and line number
+ (void)debugLog:(char *)sourceFile lineNumber:(int)lineNumber format:(NSString *)format, ...;

//! @brief NSLog, also logs source file, line number and calling method
+ (void)debugLog:(char *)sourceFile lineNumber:(int)lineNumber function:(char *)functionName format:(NSString *)format, ...;

//@} (Debug Logging methods)

/*!
 \name Enhanced Normal Logging methods. 
 - Only fire when either DEBUG_LOG_AUTO_ENABLE is defined or the environment
 variable NSDebugEnabled exists and is set to YES
 */
//@{

//! @brief NSLog, also logs source file and line number
+ (void)log:(char *)sourceFile lineNumber:(int)lineNumber format:(NSString *)format, ...;

//! @brief NSLog, also logs source file, line number and calling method
+ (void)log:(char *)sourceFile lineNumber:(int)lineNumber function:(char *)functionName format:(NSString *)format, ...;

//@} (Enhanced Normal Logging methods)

/*!
 \name WARNING Logging methods. 
 - Only fire when either DEBUG_LOG_AUTO_ENABLE is defined or the environment
 variable NSDebugEnabled exists and is set to YES
 */
//@{

//! @brief Plain NSLog, but adds "WARNING"
+ (void)warn:(NSString *)format, ...;

//! @brief NSLog, adds "WARNING" and also logs source file and line number
+ (void)warn:(char *)sourceFile lineNumber:(int)lineNumber format:(NSString *)format, ...;

//! @brief NSLog, adds "WARNING" and also logs source file, line number and calling method
+ (void)warn:(char *)sourceFile lineNumber:(int)lineNumber function:(char *)functionName format:(NSString *)format, ...;

//@} (WARNING Logging methods)

/*!
 \name Control methods. 
 - Used to enable/disable logging for debugging methods and to redirect log output
 */
//@{

//! @brief Enables/Disables logging at runtime for the debug logging methods
+ (void)setLogOn: (BOOL) logOn;

//! @brief Switches stderr to logging to a user specified file
+ (void)switchLoggingToFile:(NSString *)filePath;

//! @brief Switches stderr back to logging to default output stream
+ (void)restoreStdErr;

//@} (Control methods)

@end
