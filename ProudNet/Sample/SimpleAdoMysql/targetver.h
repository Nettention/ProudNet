#pragma once

// Following macro defines minimum required platform. 
// Minimum required platform is newest version of Windows, Internet Explorer that includes required function for running application program.
// This macro work when you activate all usable functions at higher than specific version.


// Modify following definition if it targets prior platform rather than following specific platform.
// Please refer MSDN for newest information of specific value that using at other platform.
#ifndef _WIN32_WINNT            // Assign Windows Vista as minimum required platform.
#define _WIN32_WINNT 0x0600     // Please check reasonable value for other version of Windows.
#endif

