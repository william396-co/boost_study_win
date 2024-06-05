
#include "timer_sync.h"
#include "timer_async.h"
#include "binding_arg_completion_handler.h"
#include "memfn_completion_handler.h"
#include "timer_sync_multithread.h"


int main()
{
	//timer1::timer_sync();
	//timer2::timer_async();
	//timer3::binding_arg_completion_handler();
	//timer4::memfn_completion_handler();
	timer5::timer_sync_multithread();

	return 0;
}