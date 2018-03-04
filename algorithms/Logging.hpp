#pragma once

#ifdef _DEBUG
#define LOG_DEBUG(args) { \
args \
}
#else
#define LOG_DEBUG(args) // Empty to delete args.
#endif
