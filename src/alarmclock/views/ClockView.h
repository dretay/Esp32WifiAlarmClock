#include "View.h"
#include "../SystemState.h"
#include <time.h>


struct clockview
{
	View*(*init)(void);	
};
extern const struct clockview ClockView;