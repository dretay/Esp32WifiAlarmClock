#include "View.h"
#include "../SystemState.h"

struct wifistatusview
{
	View*(*init)(void);	
};
extern const struct wifistatusview WifiStatusView;