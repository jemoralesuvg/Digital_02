
#define RCGCGPIO (*((unsigned int *) 0x400FE608U))
#define GPIOF_DIR (*((unsigned int *) 0x40025400U))
#define GPIOF_DEN (*((unsigned int *) 0x4002551CU))
#define GPIOF_DATA (*((unsigned int *) 0x400253FCU))

int delay;

int main(){
	RCGCGPIO = 0x20U;
	GPIOF_DIR = 0x0EU;
	GPIOF_DEN = 0x0EU;
	
	while(1){
		delay = 0;
		while(delay < 1000000){
			GPIOF_DATA = 0x0EU;
			++delay;
		}
		
		delay = 0;
		
		while(delay < 1000000){
			GPIOF_DATA = 0x00U;
			++delay;
		}
		
	}
	
}