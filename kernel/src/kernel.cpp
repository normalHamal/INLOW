#include <stddef.h>
#include <stdint.h>

extern "C" void kernel_main(uint32_t, uintptr_t)
{
		uint8_t* video = (uint8_t*) 0xB8000;
		const char* hello = "Hello Wrold! Form INLOW";
		for (size_t i = 0; i < 23; i++)
		{
				video[2 * i] = hello[i];
				video[2 * i + 1] = 0x07;
		}
}
