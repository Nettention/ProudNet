
#pragma once

// Following macro defines minimum required platform. 
// Minimum required platform is newest version of Windows, Internet Explorer that includes required function for running application program.
// This macro work when you activate all usable functions at higher than specific version.

// Modify following definition if it targets prior platform rather than following specific platform.
// Please refer MSDN for newest information of specific value that using at other platform.

#ifndef WINVER                          // Assign Windows Vista as minimum required platform.
#define WINVER 0x0600           // Please check reasonable value for other version of Windows.
#endif

#ifndef _WIN32_WINNT            // Assign Windows Vista as minimum required platform.
#define _WIN32_WINNT 0x0600     // Please check reasonable value for other version of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Assign Windows 98 as minimum required platform.
#define _WIN32_WINDOWS 0x0410 // Please check reasonable value for Windows Me or higher.
#endif

#ifndef _WIN32_IE                       // Assign Internet Explorer 7.0 as minimum required platform.
#define _WIN32_IE 0x0700        // Please check reasonable value for other version of IE.
#endif

